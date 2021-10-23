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
exec_test 'mkdir test_dir ; cd test_dir ; rm -rf ../test_dir ; cd . ; pwd ; cd . ; pwd ; cd .. ; pwd ; env | grep PWD | sort'
exec_test 'cd fdfddffdfdfdff'
# exec_test 'cd ./; unset OLDPWD ;cd ./; env | grep PWD' fix later,,,,,
if [ $(uname) == "Linux" ]; then
	exec_test 'export CDPATH=/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort' 
	exec_test 'export CDPATH=/home:/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort' 
fi
#	exec_test 'export CDPATH=/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort' 
#	exec_test 'export CDPATH=/home:/home/fyuta/minishell/srcs; cd builtin; pwd; env | grep PWD | sort'
# replace with your minishell path!!



# export TESTS
exec_test 'export AAA=BBB; env | grep AAA'
exec_test 'export AAA=BBB; export AAA+=BBB; env | grep AAA'
exec_test 'export AAA-=BBB; env | grep AAA'
exec_test 'export AAA=; env | grep AAA'
exec_test 'export 1AAA=; env | grep 1AAA'
exec_test 'export A1AA=; env | grep A1AA'
exec_test 'export A1AA; env | grep A1AA'
exec_test 'export LLL'
# echo "export > a | exit" | bash ; echo "export | grep -v "minishell >" > a | exit" | ./minishell > b | diff a b >> test_env.log | rm a b;

# unset TESTS
exec_test 'export AAA=B; unset AAA ; env | grep AAA'
exec_test 'export FFF=B; unset A B C D E FFF ; env | grep FFF'
exec_test 'unset A B C D E F'

# env TESTS
# echo "env > a | exit" | bash ; echo "env | grep -v "minishell > a" | exit" | ./minishell > b | diff a b >> test_env.log | rm a b;

# PIPE TESTS
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
exec_test 'echo test > ls ; cat ls'
exec_test 'echo test > ls >> ls >> ls ; echo test >> ls; cat ls'
exec_test 'echo test > ls >> lol > f1 ; cat ls lol f1'
exec_test '> lol echo test lol; cat lol test'
exec_test '>lol echo > test>lol>test>>lol>test mdr >lol test >test; cat test lol'
exec_test 'cat < ls'
exec_test 'cat < ls > ls'
exec_test '< ls cat'
exec_test '< ls > ls cat'
exec_test 'echo test > ls < result.log >> lol > f1 ; cat ls lol f1'

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
exec_test 'ls -la | grep 2 | sort | head -1'
exec_test 'ls "-la" | grep 2 | "sort" | head "-1"'
exec_test 'ls | grep abcdefg'
exec_test 'echo $PATH | wc -l'
exec_test 'echo $PATH | wc -l | cat'
exec_test 'echo "$PATH" | wc -l'
exec_test 'echo "$PATH" | wc -l | cat'
exec_test 'echo ''$PATH\'' | wc -l'
exec_test 'echo ''$PATH\'' | wc -l | cat'
exec_test 'cat result.log | wc -l'
exec_test 'cat result.log | wc -l | cat'
exec_test 'echo result.log | grep 2 | sort | head -1'
exec_test '"echo" result.log | grep 2 | "sort" | head "-1"'
exec_test 'echo result.log | grep abcdefg'
exec_test 'ls | ls | ls'
exec_test 'ls | cat'
exec_test 'cat | cat | cat'
exec_test 'cat | ls'

# # MULTI TESTS
# exec_test 'echo testing multi ; echo "test 1 ; | and 2" ; cat tests/lorem.txt | grep Lorem'

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
exec_test "exit wrong"
exec_test "exit wrong_command"
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

rm lol ls a f1 test perm_a perm_b perm_c test_stdout i1
