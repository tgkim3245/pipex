# pipex 테스터 실패 원인 분석

`42_pipex_tester`의 `bash run.sh --show-valgrind` 결과를 바탕으로, 실패한 항목들의 실제 원인을 코드 레벨에서 확인한 결과입니다. **코드는 수정하지 않았고, 원인 분석만 진행했습니다.**

---

## 1. `make bonus` 룰이 없어서 빌드 실패

```
make: Entering directory '/home/taegokim/pipex'
make: *** No rule to make target 'bonus'.  Stop.
```

- 원인: [Makefile](Makefile)에 `bonus:` 타겟이 정의되어 있지 않음. 이 프로젝트는 mandatory/bonus를 분리하지 않고 하나의 바이너리에서 heredoc·멀티커맨드를 모두 지원하는 구조라, 실제로는 이미 빌드된 `pipex` 바이너리로 보너스 테스트(#26~28)까지 통과는 하고 있지만, `make bonus` 자체는 매번 에러로 끝남.
- 왜 문제인가: 42 채점 관례상 `make bonus`가 실패하면 보너스 파트 전체가 0점 처리되는 경우가 많음. 실제 동작 여부와 무관하게 **빌드 룰 자체의 부재**가 평가 리스크.
- 관련 코드: [Makefile](Makefile) (전체에 `bonus` 타겟 없음)

---

## 2. 빈 문자열 커맨드("")를 주면 세그폴트 (테스트 #24, #25)

```
#24: "infiles/basic.txt" "" "cat -e" "outfiles/outfile"     ------- FATAL ERROR -------
#25: "infiles/basic.txt" "cat -e" "" "outfiles/outfile"     ------- FATAL ERROR -------
```

재현:
```
$ ./pipex /tmp/basic.txt "" "cat -e" /tmp/out.txt
Segmentation fault (exit code 139)
```

- 원인 경로:
  1. [parser.c](src/parser.c)의 `file_parser_parse`/`heredoc_parser_parse`는 각 커맨드 문자열이 빈 문자열인지 검증하지 않음.
  2. [cmd.c:56-58](src/cmd.c#L56-L58)에서 `ft_split(parsed->commands[idx], ' ')`를 호출하는데, 빈 문자열을 넣으면 [ft_split.c](ft_printf/libft/ft_split.c)는 단어 개수 0인 배열, 즉 `argv = {NULL}`을 반환함(포인터 자체는 NULL이 아니라서 `if (!this->argv)` 체크를 통과함).
  3. 이후 `this->argv[0]`(=NULL)을 그대로 [util/create_cmd_path.c:57](util/create_cmd_path.c#L57)의 `create_cmd_path(cmd_name, envp)`에 넘기고, 거기서 `ft_strchr(cmd_name, '/')`를 호출.
  4. [ft_strchr.c](ft_printf/libft/ft_strchr.c)는 `s`가 NULL인지 검사하지 않고 바로 `*p`(=`*NULL`)를 역참조 → 세그폴트.
- 결론: "빈 커맨드 문자열"에 대한 검증이 파서/`cmd_init` 어디에도 없어서, `argv[0] == NULL` 상태로 경로 탐색 로직까지 흘러들어가 크래시.

---

## 3. outfile을 열 수 없을 때 exit code가 실제 bash와 다름 (테스트 #14, #15, #16)

```
TEST 14: ./pipex "..." "cat -e" "cat -e" "outfiles/outfile_without_permissions"
Your exit status: 0
Orig exit status: 1
```

재현:
```
$ chmod 000 /tmp/out_noperm.txt
$ ./pipex /tmp/basic.txt "cat -e" "cat -e" /tmp/out_noperm.txt
/tmp/out_noperm.txt: Permission denied
$ echo $?
0     # 원본 bash라면 1
```

- 원인: [src/fd_factory.c:24-32](src/fd_factory.c#L24-L32) `create_fd_out_impl`이 `open()` 실패 시 에러만 출력하고 **`/dev/null`로 조용히 대체**해서 파이프라인을 정상 진행시킴. 그 결과 마지막 커맨드(`cat -e`)는 `/dev/null`에 정상적으로 쓰고 exit 0으로 끝나버림.
- [README.md:178](README.md#L178)에 "outfile 생성 불가 → 커맨드들은 정상 실행되지만 마지막 출력은 버려짐, exit code는 cmd2 기준"이라고 **의도된 설계**로 문서화되어 있음. 하지만 실제 bash는 출력 리다이렉션(`>`)이 실패하면 그 커맨드 자체를 실행하지 않고 셸이 에러 exit code(1)를 반환하므로, 이 설계 자체가 bash 동작과 어긋남.
- 결론: 의도적 폴백이지만 채점 기준(bash와 동일한 exit code)에는 맞지 않는 설계.

---

## 4. heredoc 모드에서 outfile을 append가 아니라 truncate로 열고 있음 (테스트 #28)

```
TEST 28: ./pipex "here_doc" "EOF" "cat -e" "cat -e" "outfiles/outfile"
cat -e << EOF | cat -e >> outfiles/outfile_tester
```
tester가 비교에 사용하는 원본 명령이 `>>`(append)인 반면, 우리 출력은 outfile에 기존 내용이 없는 상태(=truncate)로 나옴.

- 원인: [src/fd_factory.c:24-32](src/fd_factory.c#L24-L32) `create_fd_out_impl`은 `this->input_type`(TYPE_FILE / TYPE_HEREDOC)을 전혀 확인하지 않고 항상 `O_TRUNC`로 연다.
- pipex bonus 스펙(및 이 테스터)의 관례: `here_doc` 모드에서는 outfile을 `>>`(append, `O_APPEND`)로 열어야 하고, 일반 파일 입력 모드에서만 `>`(truncate)를 사용해야 함. 이 분기가 코드에 없음.

---

## 5. fork된 자식 프로세스에서 메모리 정리가 안 돼서 valgrind 리크로 잡힘

### 5-1. execve 실패(command not found) 시 자식이 정리 없이 `_exit` (테스트 #17, #18)

```
TEST 17: nonexistingcommand → LEAKS [KO]
==...== ERROR SUMMARY: 6 errors from 6 contexts (suppressed: 0 from 0)
```
tester가 실제로 쓰는 플래그(`--errors-for-leak-kinds=all`, [42_pipex_tester/test.sh:141](42_pipex_tester/test.sh#L141))로 재현하면 그대로 6 errors / 266 bytes가 재현됨.

- 원인: [src/cmd.c:33-38](src/cmd.c#L33-L38) `run_impl`의 자식 프로세스 분기에서 `execve` 실패 시 `report_error` 후 바로 `_exit(127)`을 호출. `fork()` 직후이므로 부모가 만들어둔 모든 힙 할당(각 `t_cmd.argv`/`t_cmd.path`, `t_pipe_mgr.pipes`, `t_cmd_mgr.cmds` 등)이 자식 메모리 공간에도 그대로 복사돼 있는데, 이걸 하나도 해제하지 않고 종료함.
- valgrind 관점에서 "definitely lost"는 아니지만(`_exit` 직후 커널이 회수), `--errors-for-leak-kinds=all`을 쓰는 테스터 기준으로는 "still reachable"도 에러로 집계되어 실패 처리됨.
- 실제 leak 스택트레이스 (재현 결과):
  - `cmd_init`이 만든 `path`(`ft_strdup`/`create_cmd_path`) - [src/cmd.c:63](src/cmd.c#L63), [util/create_cmd_path.c](util/create_cmd_path.c)
  - `cmd_init`이 만든 `argv`(`ft_split`) - [src/cmd.c:57](src/cmd.c#L57)
  - `pipe_mgr_init`의 `pipes` 배열(`ft_calloc`) - [src/pipe_mgr.c:50](src/pipe_mgr.c#L50)
  - `cmd_mgr_init`의 `cmds` 배열(`ft_calloc`) - [src/cmd_mgr.c:74](src/cmd_mgr.c#L74)

### 5-2. heredoc 종료 후 get_next_line의 static 버퍼가 해제되지 않음 (테스트 #28)

```
1 bytes in 1 blocks are still reachable in loss record 1 of 1
   by trim_leftover (get_next_line.c:77)
   by get_next_line (get_next_line.c:101)
   by write_heredoc (fd_factory_heredoc_impl.c:42)
```

- 원인: [get_next_line.c:90-103](get_next_line/get_next_line.c#L90-L103)의 `leftover`는 함수 내부 `static` 변수라서, "다음 호출에서 leftover가 비어있으면 free 후 NULL 반환"하는 방식으로만 정리됨([get_next_line.c:98-99](get_next_line/get_next_line.c#L98-L99)).
- 그런데 [src/fd_factory_heredoc_impl.c:36-55](src/fd_factory_heredoc_impl.c#L36-L55)의 `write_heredoc`은 리미터(`EOF`) 라인을 만나면 그 자리에서 바로 `break`하고 루프를 끝내버려서, 그 정리를 트리거할 "한 번 더 호출"이 일어나지 않음. 그 결과 마지막 `trim_leftover`가 만든 빈 문자열 버퍼(1바이트)가 static 변수에 담긴 채 해제되지 않고 남음.

---

## 요약 표

| # | 증상 | 실패 테스트 | 원인 파일 |
|---|---|---|---|
| 1 | `make bonus` 룰 없음 | (빌드 단계) | [Makefile](Makefile) |
| 2 | 빈 커맨드 문자열 → 세그폴트 | #24, #25 | [src/cmd.c](src/cmd.c#L56-L59), [util/create_cmd_path.c](util/create_cmd_path.c#L57), [ft_strchr.c](ft_printf/libft/ft_strchr.c) |
| 3 | outfile 권한 없음 → exit code 불일치(0 vs 1) | #14, #15, #16 | [src/fd_factory.c](src/fd_factory.c#L24-L32) |
| 4 | heredoc에서 outfile을 truncate로 염(append여야 함) | #28 (Output 불일치) | [src/fd_factory.c](src/fd_factory.c#L24-L32) |
| 5-1 | execve 실패 시 자식 프로세스가 메모리 정리 없이 `_exit` | #17, #18 (LEAKS) | [src/cmd.c](src/cmd.c#L33-L38) |
| 5-2 | heredoc 종료 시 get_next_line static 버퍼 미해제 | #28 (LEAKS) | [get_next_line/get_next_line.c](get_next_line/get_next_line.c#L90-L103), [src/fd_factory_heredoc_impl.c](src/fd_factory_heredoc_impl.c#L36-L55) |


---
```
taegokim@c2r14s4:~/pipex/42_pipex_tester$ bash run.sh --show-valgrind
make: Entering directory '/home/taegokim/pipex'
make: Nothing to be done for 'all'.
make: Leaving directory '/home/taegokim/pipex'
make: Entering directory '/home/taegokim/pipex'
make: *** No rule to make target 'bonus'.  Stop.
make: Leaving directory '/home/taegokim/pipex'

compiling           [OK]    
norminette          [OK]    


TESTNAME                                                                                  OUT     EXIT    TIME    LEAKS   

BASIC CHECKS
# 1: "infiles/basic.txt" "cat -e" "cat -e" "outfiles/outfile"                             [OK]    [OK]    [OK]    [OK]    
# 2: "infiles/basic.txt" "ls -la" "cat -e" "outfiles/outfile"                             [OK]    [OK]    [OK]    [OK]    
# 3: "infiles/basic.txt" "ls -l -a" "cat -e -n" "outfiles/outfile"                        [OK]    [OK]    [OK]    [OK]    
# 4: "infiles/basic.txt" "ls -l -a -f" "cat -e -n" "outfiles/outfile"                     [OK]    [OK]    [OK]    [OK]    
# 5: "infiles/basic.txt" "ls -laf" "cat -e -n" "outfiles/outfile"                         [OK]    [OK]    [OK]    [OK]    
# 6: "infiles/basic.txt" "grep -A5 is" "cat -e" "outfiles/nonexistingfile"                [OK]    [OK]    [OK]    [OK]    
# 7: "infiles/basic.txt" "cat -e" "grep nonexistingword" "outfiles/nonexistingfile"       [OK]    [OK]    [OK]    [OK]    
# 8: "infiles/empty.txt" "grep nonexistingword" "cat -e" "outfiles/outfile"               [OK]    [OK]    [OK]    [OK]    
# 9: "infiles/basic.txt" "sleep 3" "ls" "outfiles/outfile"                                [OK]    [OK]    [OK]    [OK]    
#10: "infiles/big_text.txt" "cat" "head -2" "outfiles/outfile"                            [OK]    [OK]    [OK]    [OK]    

ERROR CHECKING
#11: "nonexistingfile" "cat -e" "ls" "outfiles/outfile"                                   [OK]    [OK]    [OK]    [OK]    
#12: "nonexistingfile" "cat" "sleep 3" "outfiles/outfile"                                 [OK]    [OK]    [OK]    [OK]    
#13: "infiles/infile_without_permissions" "cat -e" "cat -e" "outfiles/outfile"            [OK]    [OK]    [OK]    [OK]    
#14: "infiles/basic.txt" "cat -e" "cat -e" "outfiles/outfile_without_permissions"         [OK]    [KO]    [OK]    [OK]    
#15: "infiles/basic.txt" "sleep 3" "cat -e" "outfiles/outfile_without_permissions"        [OK]    [KO]    [OK]    [OK]    
#16: "nonexistingfile" "cat -e" "cat -e" "outfiles/outfile_without_permissions"           [OK]    [KO]    [OK]    [OK]    
#17: "infiles/basic.txt" "nonexistingcommand" "cat -e" "outfiles/outfile"                 [OK]    [OK]    [OK]    [KO]    
#18: "infiles/basic.txt" "cat -e" "nonexistingcommand" "outfiles/outfile"                 [OK]    [OK]    [OK]    [KO]    
#19: "infiles/basic.txt" "cat -e" "cat -nonexistingflag" "outfiles/outfile"               [OK]    [OK]    [OK]    [OK]    
#20:                                                                                                              [OK]    
#21: ""                                                                                                           [OK]    
#22: "infiles/basic.txt" "cat -e" "outfiles/outfile"                                                              [OK]    
#23: "" "cat -e" "cat -e" "outfiles/outfile"                                                                      [OK]    
#24: "infiles/basic.txt" "" "cat -e" "outfiles/outfile"                                   ------- FATAL ERROR -------
#25: "infiles/basic.txt" "cat -e" "" "outfiles/outfile"                                   ------- FATAL ERROR -------

BONUS
#26: "infiles/basic.txt" "cat -e" "cat -e" "cat -e" "outfiles/outfile"                    [OK]    [OK]    [OK]    [OK]    
#27: "infiles/basic.txt" "cat -e" "cat -e" "cat -e" "cat -e" "cat -e" "cat -e" "cat -e"   [OK]    [OK]    [OK]    [OK]    
#28: "here_doc" "EOF" "cat -e" "cat -e" "outfiles/outfile"                                [KO]    [OK]    [OK]    [KO]    

ERRORS :(
------------------------------------------------------------------------------------------------------------------------
TEST 14:
./pipex "infiles/basic.txt" "cat -e" "cat -e" "outfiles/outfile_without_permissions" 
< infiles/basic.txt cat -e | cat -e > outfiles/outfile_without_permissions_tester

Exit status:
Your exit status: 0
Orig exit status: 1

------------------------------------------------------------------------------------------------------------------------
TEST 15:
./pipex "infiles/basic.txt" "sleep 3" "cat -e" "outfiles/outfile_without_permissions" 
< infiles/basic.txt sleep 3 | cat -e > outfiles/outfile_without_permissions_tester

Exit status:
Your exit status: 0
Orig exit status: 1

------------------------------------------------------------------------------------------------------------------------
TEST 16:
./pipex "nonexistingfile" "cat -e" "cat -e" "outfiles/outfile_without_permissions" 
< nonexistingfile cat -e | cat -e > outfiles/outfile_without_permissions_tester

Exit status:
Your exit status: 0
Orig exit status: 1

------------------------------------------------------------------------------------------------------------------------
TEST 17:
./pipex "infiles/basic.txt" "nonexistingcommand" "cat -e" "outfiles/outfile" 
< infiles/basic.txt nonexistingcommand | cat -e > outfiles/outfile_tester

Leaks:  
==1531939== Memcheck, a memory error detector
==1531939== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1531939== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==1531939== Command: ./pipex infiles/basic.txt nonexistingcommand cat\ -e outfiles/outfile
==1531939== Parent PID: 1531937
==1531939== 
==1531940== 
==1531940== HEAP SUMMARY:
==1531940==     in use at exit: 266 bytes in 9 blocks
==1531940==   total heap usage: 78 allocs, 69 frees, 1,735 bytes allocated
==1531940== 
==1531940== LEAK SUMMARY:
==1531940==    definitely lost: 0 bytes in 0 blocks
==1531940==    indirectly lost: 0 bytes in 0 blocks
==1531940==      possibly lost: 0 bytes in 0 blocks
==1531940==    still reachable: 266 bytes in 9 blocks
==1531940==         suppressed: 0 bytes in 0 blocks
==1531940== Reachable blocks (those to which a pointer was found) are not shown.
==1531940== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==1531940== 
==1531940== For lists of detected and suppressed errors, rerun with: -s
==1531940== ERROR SUMMARY: 6 errors from 6 contexts (suppressed: 0 from 0)
==1531939== 
==1531939== HEAP SUMMARY:
==1531939==     in use at exit: 0 bytes in 0 blocks
==1531939==   total heap usage: 78 allocs, 78 frees, 1,735 bytes allocated
==1531939== 
==1531939== All heap blocks were freed -- no leaks are possible
==1531939== 
==1531939== For lists of detected and suppressed errors, rerun with: -s
==1531939== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
------------------------------------------------------------------------------------------------------------------------
TEST 18:
./pipex "infiles/basic.txt" "cat -e" "nonexistingcommand" "outfiles/outfile" 
< infiles/basic.txt cat -e | nonexistingcommand > outfiles/outfile_tester

Leaks:  
==1531962== Memcheck, a memory error detector
==1531962== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1531962== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==1531962== Command: ./pipex infiles/basic.txt cat\ -e nonexistingcommand outfiles/outfile
==1531962== Parent PID: 1531960
==1531962== 
==1531964== 
==1531964== HEAP SUMMARY:
==1531964==     in use at exit: 266 bytes in 9 blocks
==1531964==   total heap usage: 78 allocs, 69 frees, 1,735 bytes allocated
==1531964== 
==1531964== LEAK SUMMARY:
==1531964==    definitely lost: 0 bytes in 0 blocks
==1531964==    indirectly lost: 0 bytes in 0 blocks
==1531964==      possibly lost: 0 bytes in 0 blocks
==1531964==    still reachable: 266 bytes in 9 blocks
==1531964==         suppressed: 0 bytes in 0 blocks
==1531964== Reachable blocks (those to which a pointer was found) are not shown.
==1531964== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==1531964== 
==1531964== For lists of detected and suppressed errors, rerun with: -s
==1531964== ERROR SUMMARY: 6 errors from 6 contexts (suppressed: 0 from 0)
==1531962== 
==1531962== HEAP SUMMARY:
==1531962==     in use at exit: 0 bytes in 0 blocks
==1531962==   total heap usage: 78 allocs, 78 frees, 1,735 bytes allocated
==1531962== 
==1531962== All heap blocks were freed -- no leaks are possible
==1531962== 
==1531962== For lists of detected and suppressed errors, rerun with: -s
==1531962== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
------------------------------------------------------------------------------------------------------------------------
TEST 28:
./pipex "here_doc" "EOF" "cat -e" "cat -e" "outfiles/outfile" 
cat -e << EOF | cat -e >> outfiles/outfile_tester

Output: 
outfiles/outfile:                                               outfiles/outfile_tester:
							      >	This is random text echoed into existing outfiles before 
							      >	applying pipex. This allows to verify whether your program
							      >	and the original replace or append existing text
Hello$$								Hello$$
Hello$$								Hello$$
Hello$$								Hello$$

Leaks:  
==1532415== Memcheck, a memory error detector
==1532415== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==1532415== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==1532415== Command: ./pipex here_doc EOF cat\ -e cat\ -e outfiles/outfile
==1532415== Parent PID: 1532413
==1532415== 
==1532415== 
==1532415== HEAP SUMMARY:
==1532415==     in use at exit: 1 bytes in 1 blocks
==1532415==   total heap usage: 83 allocs, 82 frees, 1,685 bytes allocated
==1532415== 
==1532415== LEAK SUMMARY:
==1532415==    definitely lost: 0 bytes in 0 blocks
==1532415==    indirectly lost: 0 bytes in 0 blocks
==1532415==      possibly lost: 0 bytes in 0 blocks
==1532415==    still reachable: 1 bytes in 1 blocks
==1532415==         suppressed: 0 bytes in 0 blocks
==1532415== Reachable blocks (those to which a pointer was found) are not shown.
==1532415== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==1532415== 
==1532415== For lists of detected and suppressed errors, rerun with: -s
==1532415== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

```