# MINISHELL-TESTER

RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"

BOLDBLACK="\033[1m\033[30m"
BOLDRED="\033[1m\033[31m"
BOLDGREEN="\033[1m\033[32m"
BOLDYELLOW="\033[1m\033[33m"
BOLDBLUE="\033[1m\033[34m"
BOLDMAGENTA="\033[1m\033[35m"
BOLDCYAN="\033[1m\033[36m"
BOLDWHITE="\033[1m\033[37m"

# Compile and set executable rights
make debug >/dev/null 2>/dev/null
rm test_error.log

function exec_test()
{
	echo "command : "$@ >> test_error.log
	echo ">>> MINISHELL" >>test_error.log
	echo $@ "; exit" | ./minishell 2>>test_error.log > test_stdout
	ES_1=$?
	TEST1=$(cat test_stdout | grep -v "minishell >")
	echo ">>> BASH" >>test_error.log
	TEST2=$(echo $@ "; exit" | bash 2>>test_error.log)
	ES_2=$?
	echo >>test_error.log
	if [ "$TEST1" == "$TEST2" ] && [ "$ES_1" == "$ES_2" ]; then
		printf " $BOLDGREEN%s$RESET" "✓ "
	else
		printf " $BOLDRED%s$RESET" "✗ "
	fi
	printf "$CYAN \"$@\" $RESET"
	if [ "$TEST1" != "$TEST2" ]; then
		echo
		echo
		printf $BOLDRED"Your output : \n%.20s\n$BOLDRED$TEST1\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
		printf $BOLDGREEN"Expected output : \n%.20s\n$BOLDGREEN$TEST2\n%.20s$RESET\n" "-----------------------------------------" "-----------------------------------------"
	fi
	if [ "$ES_1" != "$ES_2" ]; then
		echo
		echo
		printf $BOLDRED"Your exit status : $BOLDRED$ES_1$RESET\n"
		printf $BOLDGREEN"Expected exit status : $BOLDGREEN$ES_2$RESET\n"
	fi
	echo
	sleep 0.1
}

printf "$BOLDMAGENTA __  __ _____ _   _ _____  _____ _    _ ______ _      _      \n"
printf "|  \/  |_   _| \ | |_   _|/ ____| |  | |  ____| |    | |     \n"
printf "| \  / | | | |  \| | | | | (___ | |__| | |__  | |    | |     \n"
printf "| |\/| | | | | . \` | | |  \___ \|  __  |  __| | |    | |     \n"
printf "| |  | |_| |_| |\  |_| |_ ____) | |  | | |____| |____| |____ \n"
printf "|_|  |_|_____|_| \_|_____|_____/|_|  |_|______|______|______|\n$RESET"
echo

# EXECUTION TESTS
exec_test '/bin/no'
exec_test 'no'
exec_test '/bin/ls'
exec_test 'ls'

# ECHO TESTS
exec_test 'echo test tout'
exec_test 'echo test      tout'
exec_test 'echo -n test tout'
exec_test 'echo -n -n -n test tout'
exec_test 'echo -nnnnnnnnnnnnnnnnnnnn test tout'
exec_test 'echo -nnnnnnnnnnnnnfnnnnnnn test tout'

# CD PWD TESTS
exec_test 'cd .. ; pwd; env | grep PWD | sort'
if [ $(uname) == "Linux" ]; then 
	exec_test 'cd /home ; pwd; env | grep PWD | sort'
else
	exec_test 'cd /Users ; pwd; env | grep PWD | sort'
fi
exec_test 'cd ; pwd; env | grep PWD | sort'
exec_test 'cd . ;cd -; pwd; env | grep PWD | sort'
exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; pwd ; cd . ; pwd ; cd .. ; pwd ; env | grep PWD | sort'
exec_test 'cd fdfddffdfdfdff'
# exec_test 'cd ./; unset OLDPWD ;cd ./; env | grep PWD' fix later,,,,,
if [ $(uname) == "Linux" ]; then
	exec_test 'export CDPATH=/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort' 
	exec_test 'export CDPATH=/home/fyuta/minishell/./srcs; cd builtin; pwd; env | grep PWD | sort' 
	exec_test 'export CDPATH=/home:/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort' 
fi
#	exec_test 'export CDPATH=/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort' 
#	exec_test 'export CDPATH=/home:/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort'
# replace with your minishell path!!
exec_test 'mkdir dir; ln -s dir sdir; cd sdir; pwd; cd ..; rmdir dir; env | grep PWD | sort; rm sdir'
exec_test 'cd nodir/..'
exec_test 'mkdir dir; chmod 055 dir; cd dir; pwd; env | grep PWD | sort; rmdir dir'
exec_test 'cd ./././././../minishell/srcs;  pwd; env | grep PWD | sort'
exec_test 'cd ././././././././././.;  pwd; env | grep PWD | sort'
exec_test 'export HOME=; cd ; pwd'
exec_test 'export CDPATH="./"; mkdir dir; cd dir; pwd; cd ..; rmdir dir'
exec_test 'export HOME=NO; cd'
exec_test 'export HOME=;cd ; pwd'

# export TESTS
exec_test 'export AAA=BBB; env | grep AAA'
exec_test 'export AAA=BBB; export AAA+=BBB; env | grep AAA'
exec_test 'export AAA-=BBB; env | grep AAA'
exec_test 'export AAA=; env | grep AAA'
exec_test 'export 1AAA=; env | grep 1AAA'
exec_test 'export A1AA=; env | grep A1AA'
exec_test 'export A1AA; env | grep A1AA'
exec_test 'export LLL; env | grep LLL ; export | grep LLL'
exec_test 'export LLL;  export LLL=YOYO;env | grep LLL ; export | grep LLL'
exec_test 'export LLL; unset LLL;env | grep LLL ; export | grep LLL'
exec_test 'export LLL=a; export LLL; env | grep LLL ; export | grep LLL'

# echo "export > a | exit" | bash ; echo "export | grep -v "minishell >" > a | exit" | ./minishell > b | diff a b >> test_env.log | rm a b;

# unset TESTS
exec_test 'export AAA=B; unset AAA ; env | grep AAA'
exec_test 'export FFF=B; unset A B C D E FFF ; env | grep FFF'
exec_test 'unset A B C D E F'

# env TESTS
# echo "env > a | exit" | bash ; echo "env | grep -v "minishell > a" | exit" | ./minishell > b | diff a b >> test_env.log | rm a b;

# PIPE TESTS
exec_test '|'
exec_test 'echo result.log | grep abcdefg'
exec_test 'ls | ls | ls'
exec_test 'ls | ls | l'
exec_test 'ls | l | ls'
exec_test 'l | ls | ls'
exec_test 'ls | cat'
exec_test 'ls | ca'
exec_test 'l | cat'
exec_test 'cat | cat | cat'
exec_test 'cat | ls'
exec_test 'ls -la | grep 2 | sort | head -1'
exec_test 'ls | grep abcdefg'
exec_test 'cat result.log | wc -l'
exec_test 'cat result.log | wc -l | cat'
exec_test 'cat result.log | wc | cat'
exec_test 'cat result.lo | wc -l | cat'
exec_test 'echo result.log | grep 2 | sort | head -1'
exec_test 'echo test | cat -e | cat -e '
# exec_test 'head -n 10 /dev/urandom  | wc ; wc -c'
exec_test 'ls|ls|ls|ls|ls|ls|ls'

# ENV EXPANSIONS + ESCAPE
exec_test 'echo test     \    test'
exec_test 'echo \"test'
exec_test 'echo $TEST'
exec_test 'echo "$TEST"'
exec_test "echo '$TEST'"
exec_test 'echo "$TEST$TEST$TEST"'
exec_test 'echo "$TEST$TEST=lol$TEST"'
exec_test 'echo "   $TEST lol $TEST"'
exec_test 'echo $TEST$TEST$TEST'
exec_test 'echo $TEST$TEST=lol$TEST""lol'
exec_test 'echo    $TEST lol $TEST'
exec_test 'echo test "" test "" test'
exec_test 'echo "\$TEST"'
exec_test 'echo "$=TEST"'
exec_test 'echo "$"'
exec_test 'echo "$?TEST"'
exec_test 'echo $TEST $TEST'
exec_test 'echo "$1TEST"'
exec_test 'echo "$T1TEST"'
exec_test 'echo "''''"'

# # ENV EXPANSIONS
# ENV_SHOW="env | sort | grep -v SHLVL | grep -v _="
# EXPORT_SHOW="export | sort | grep -v SHLVL | grep -v _= | grep -v OLDPWD"
# exec_test 'export ='
# exec_test 'export 1TEST= ;' $ENV_SHOW
# exec_test 'export TEST ;' $EXPORT_SHOW
# exec_test 'export ""="" ; ' $ENV_SHOW
# exec_test 'export TES=T="" ;' $ENV_SHOW
# exec_test 'export TE+S=T="" ;' $ENV_SHOW
# exec_test 'export TEST=LOL ; echo $TEST ;' $ENV_SHOW
exec_test '$NO'
exec_test 'export TEST=LOL ; echo $TEST$TEST$TEST=lol$TEST'
# exec_test 'export TEST=LOL; export TEST+=LOL ; echo $TEST ;' $ENV_SHOW
# exec_test $ENV_SHOW
# exec_test $EXPORT_SHOW
exec_test 'export TEST="ls       -l     - a" ; echo $TEST '

# # REDIRECTIONS
exec_test '>'
exec_test '< a < ls < lol < f1; ls'
exec_test '> a > ls > lol > f1; ls'
exec_test '>> a >> ls >> lol >> f1; ls'
exec_test 'echo test > ls ; cat ls; ls'
exec_test 'ech test > ls ; cat ls; ls'
exec_test 'cat memo.txt > ls ; cat ls; ls'
exec_test 'cat memo.txt > ls ; cat l; ls'
exec_test 'cat memo.txt > ls ; ca ls; ls'
exec_test 'cat memo.tx > ls ; cat ls; ls'
exec_test 'ca memo.txt > ls ; cat ls; ls'
exec_test 'echo test > ls >> ls >> ls ; echo test >> ls; cat ls; ls'
exec_test 'echo test > ls >> ls >> ls ; cat ls; ls'
exec_test 'echo test > ls >> lol > f1 ; cat ls lol f1; ls'
exec_test '> lol echo test lol; cat lol test; ls'
exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test; cat test lol; ls'
exec_test 'cat < ls; ls'
exec_test 'cat < ls > ls; ls'
exec_test '< ls cat; ls'
exec_test '< ls > ls cat; ls'
exec_test 'echo test > ls < result.log >> lol > f1 ; cat ls lol f1; ls'

# REDIRECTION + PIPING
exec_test 'ls > i1 | grep i | echo hey; cat i1'
exec_test 'ls -la | grep i > i1 | echo hey; cat i1'
exec_test 'echo aaa > a; < a cat | cat'
exec_test 'echo aaa > a; cat < a | cat'
exec_test 'echo aaa > a; cat a | cat'
exec_test 'echo aaa > a; < a cat | grep a'
exec_test 'echo aaa > a; cat < a | grep a'
exec_test 'echo aaa > a; cat a | grep a'
exec_test 'ls > i1 | echo hello > lol; cat i1 lol'

# REDIRECTIONS + EXPANSIONS
exec_test 'echo $HOME > test; cat test'
exec_test 'echo a b c d e f g > test; < test cat'
exec_test 'echo test > $NO'
exec_test 'cat < $NO'
exec_test '> test echo $HOME'
exec_test '> $NO echo test'

# PIPING + EXPANSION
exec_test 'ls "-la" | grep 2 | "sort" | head "-1"'
exec_test 'echo $PATH | wc -l'
exec_test 'echo $PATH | wc -l | cat'
exec_test 'echo "$PATH" | wc -l'
exec_test 'echo "$PATH" | wc -l | cat'
exec_test 'echo ''$PATH\'' | wc -l'
exec_test 'echo ''$PATH\'' | wc -l | cat'
exec_test '"echo" result.log | grep 2 | "sort" | head "-1"'

# # MULTI TESTS
# exec_test 'echo testing multi ; echo "test 1 ; | and 2" ; cat tests/lorem.txt | grep Lorem'
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 755 hello.sh; hello.sh;cp hello.sh ./incs; hello.sh; ./hello.sh; chmod 000 hello.sh; ./hello.sh; export PATH+=:$PWD; hello.sh ; export PATH+=:./incs; hello.sh; rm -rf hello.sh"
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 755 hello.sh; hello.sh; ./hello.sh ; unset PWD; hello.sh; export PWD="::::::"; hello.sh; rm -rf hello.sh"
exec_test 'export aaa; echo $aaa'

# # SYNTAX ERROR
exec_test ';; test'
exec_test '| test'
exec_test 'echo > <'
exec_test 'echo | |'
exec_test '<'

# EXIT STATUS
exec_test "exit 42"
# exec_test "exit 42 53 68"
exec_test "exit 259"
exec_test "exit 9223372036854775807"
exec_test "exit -9223372036854775808"
exec_test "exit 9223372036854775808"
exec_test "exit -9223372036854775810"
exec_test "exit -4"
exec_test "exit -"
exec_test "exit ''"
exec_test "exit wrong"
exec_test "exit wrong_command"
exec_test "exit 1 2 | ls"
exec_test "exit -- no"
exec_test "exit -- no NO"
exec_test "exit -- 1"
exec_test "gdagadgag"
exec_test "ls -Z"
exec_test "cd gdhahahad"
exec_test "touch perm_a; chmod 355 perm_a; cat perm_a"
exec_test "touch perm_b; chmod 355 perm_b; < perm_b cat"
exec_test "touch perm_c; chmod 355 perm_c; perm_c"
exec_test "< nofile cat"
exec_test "pwd | wtf"
exec_test "wtf | pwd"
exec_test "export AAA"
exec_test "export AAA=A"
exec_test "export 1AA"
exec_test "export 1AA=A"
exec_test "export AAA-=A"
exec_test "export AAA+=A"
exec_test "cd"
exec_test "cd ./a b"
exec_test "cd a b"
exec_test '/bin/no'
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 300 hello.sh; ./hello.sh; rm -f ./hello.sh"
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 400 hello.sh; ./hello.sh; rm -f ./hello.sh"
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 500 hello.sh; ./hello.sh; rm -f ./hello.sh"
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 600 hello.sh; ./hello.sh; rm -f ./hello.sh"
exec_test "printf '#!/bin/bash\n echo hello' > hello.sh; chmod 700 hello.sh; ./hello.sh; rm -f ./hello.sh"

# PERMISSIONS TESTS
#exec_test 'touch a; chmod 000 a; ./a; chmod 777 a'
#exec_test 'touch b; chmod 100 b; ./b; chmod 777 b'
#exec_test 'touch c; chmod 200 c; ./c; chmod 777 c'
#exec_test 'touch d; chmod 400 d; ./d; chmod 777 d'
#exec_test 'touch e; chmod 500 e; ./e; chmod 777 e'
#exec_test 'touch f; chmod 600 f; ./f; chmod 777 f'
#exec_test 'touch g; chmod 700 g; ./g; chmod 777 g'
#exec_test 'touch b; chmod 010 b; ./b; chmod 777 b'
#exec_test 'touch c; chmod 020 c; ./c; chmod 777 c'
#exec_test 'touch d; chmod 040 d; ./d; chmod 777 d'
#exec_test 'touch e; chmod 050 e; ./e; chmod 777 e'
#exec_test 'touch f; chmod 060 f; ./f; chmod 777 f'
#exec_test 'touch g; chmod 070 g; ./g; chmod 777 g'
#exec_test 'touch b; chmod 001 b; ./b; chmod 777 b'
#exec_test 'touch c; chmod 002 c; ./c; chmod 777 c'
#exec_test 'touch d; chmod 004 d; ./d; chmod 777 d'
#exec_test 'touch e; chmod 005 e; ./e; chmod 777 e'
#exec_test 'touch f; chmod 006 f; ./f; chmod 777 f'
#exec_test 'touch g; chmod 007 g; ./g; chmod 777 g'
#exec_test 'touch a; chmod 000 a; cat a; chmod 777 a'
#exec_test 'touch b; chmod 100 b; cat b; chmod 777 b'
#exec_test 'touch c; chmod 200 c; cat c; chmod 777 c'
#exec_test 'touch d; chmod 400 d; cat d; chmod 777 d'
#exec_test 'touch e; chmod 500 e; cat e; chmod 777 e'
#exec_test 'touch f; chmod 600 f; cat f; chmod 777 f'
#exec_test 'touch g; chmod 700 g; cat g; chmod 777 g'
#exec_test 'touch b; chmod 010 b; cat b; chmod 777 b'
#exec_test 'touch c; chmod 020 c; cat c; chmod 777 c'
#exec_test 'touch d; chmod 040 d; cat d; chmod 777 d'
#exec_test 'touch e; chmod 050 e; cat e; chmod 777 e'
#exec_test 'touch f; chmod 060 f; cat f; chmod 777 f'
#exec_test 'touch g; chmod 070 g; cat g; chmod 777 g'
#exec_test 'touch b; chmod 001 b; cat b; chmod 777 b'
#exec_test 'touch c; chmod 002 c; cat c; chmod 777 c'
#exec_test 'touch d; chmod 004 d; cat d; chmod 777 d'
#exec_test 'touch e; chmod 005 e; cat e; chmod 777 e'
#exec_test 'touch f; chmod 006 f; cat f; chmod 777 f'
#exec_test 'touch g; chmod 007 g; cat g; chmod 777 g'
#exec_test 'touch a; chmod 000 a; cd a; chmod 777 a'
#exec_test 'touch b; chmod 100 b; cd b; chmod 777 b'
#exec_test 'touch c; chmod 200 c; cd c; chmod 777 c'
#exec_test 'touch d; chmod 400 d; cd d; chmod 777 d'
#exec_test 'touch e; chmod 500 e; cd e; chmod 777 e'
#exec_test 'touch f; chmod 600 f; cd f; chmod 777 f'
#exec_test 'touch g; chmod 700 g; cd g; chmod 777 g'
#exec_test 'touch a; chmod 010 a; cd a; chmod 777 b'
#exec_test 'touch c; chmod 020 c; cd c; chmod 777 c'
#exec_test 'touch d; chmod 040 d; cd d; chmod 777 d'
#exec_test 'touch e; chmod 050 e; cd e; chmod 777 e'
#exec_test 'touch f; chmod 060 f; cd f; chmod 777 f'
#exec_test 'touch g; chmod 070 g; cd g; chmod 777 g'
#exec_test 'touch b; chmod 001 b; cd b; chmod 777 b'
#exec_test 'touch c; chmod 002 c; cd c; chmod 777 c'
#exec_test 'touch d; chmod 004 d; cd d; chmod 777 d'
#exec_test 'touch e; chmod 005 e; cd e; chmod 777 e'
#exec_test 'touch f; chmod 006 f; cd f; chmod 777 f'
#exec_test 'touch g; chmod 007 g; cd g; chmod 777 g'
#exec_test 'mkdir aa; chmod 000 aa; cd aa; cd ../'
#exec_test 'mkdir bb; chmod 100 bb; cd bb; cd ../'
#exec_test 'mkdir cc; chmod 200 cc; cd cc; cd ../'
#exec_test 'mkdir dd; chmod 400 dd; cd dd; cd ../'
#exec_test 'mkdir ee; chmod 500 ee; cd ee; cd ../'
#exec_test 'mkdir ff; chmod 600 ff; cd ff; cd ../'
#exec_test 'mkdir gg; chmod 700 gg; cd gg; cd ../'

rm  lol ls a b c d e f g f1 test perm_a perm_b perm_c test_stdout i1 ./incs/hello.sh
rmdir aa bb cc dd ee ff gg

# export SHLVL=999; ./minishell
# env | grep SHLVL; exit
# export SHLVL=2; ./minishell
# env | grep SHLVL; exit
# export PWD=NO; ./minishell
# env | grep PWD; exit
