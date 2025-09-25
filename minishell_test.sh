#!/bin/bash
# minishell_test.sh
# 测试 minishell 各项功能，包括基本和刁钻测试
# Usage: ./minishell_test.sh /path/to/minishell

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

VALGRIND="valgrind --leak-check=full --track-origins=yes --log-file=valgrind.log"
MINISHELL="$1"

if [[ ! -x "$MINISHELL" ]]; then
    echo "Usage: $0 /path/to/minishell"
    exit 1
fi

echo "=== STARTING MINISHELL TESTS ==="

run_test() {
    local description="$1"
    local commands="$2"
    echo -e "\n--- Test: $description ---" | tee -a valgrind.log
    echo -e "Commands:\n$commands"
    $VALGRIND "$MINISHELL" <<EOF
$commands
EOF
    echo "--- End Test ---" | tee -a valgrind.log
}

# ===========================
# 1. Simple Commands & Globals
# ===========================
run_test "Absolute path simple command" "/bin/ls"
run_test "Absolute path command with spaces" "/bin/ls    "
run_test "Empty command" ""
run_test "Only spaces" "   "
run_test "Only tabs" "    "
run_test "Invalid command path" "/bin/ls_nonexistent"

# ===========================
# 2. Arguments
# ===========================
run_test "Command with arguments" "/bin/echo hello world"
run_test "Multiple commands" "/bin/echo a b c"
run_test "Command with non-existent argument" "/bin/ls /nonexistentfolder"

# ===========================
# 3. Echo Command
# ===========================
run_test "echo without arguments" "echo"
run_test "echo with arguments" "echo Hello Minishell"
run_test "echo with -n" "echo -n NoNewline"
run_test "echo with spaces" "echo    spaced    text"

# ===========================
# 4. Exit
# ===========================
run_test "exit without args" "exit"
"$MINISHELL"
run_test "exit with arg" "exit 42"
"$MINISHELL"

# ===========================
# 5. Return Values
# ===========================
run_test "Return value of success" "/bin/true
echo \$?"
run_test "Return value of failure" "/bin/false
echo \$?"
run_test "Return value of missing file" "/bin/ls /nonexistent
echo \$?"
run_test "Expr \$? + \$?" "/bin/true
expr \$? + \$?"

# ===========================
# 6. Signals
# ===========================
echo -e "\n*** SIGNAL TESTS ***"
echo "(CTRL-C in empty prompt)"
"$MINISHELL" & sleep 1 && kill -SIGINT $!
echo "(CTRL-\\ in empty prompt)"
"$MINISHELL" & sleep 1 && kill -SIGQUIT $!
echo "(CTRL-D in empty prompt should quit)"
"$MINISHELL" <<< $'\x04'
echo "(CTRL-C after typing text)"
"$MINISHELL" <<< "echo hello" &
sleep 1 && kill -SIGINT $!
echo "(CTRL-D after typing text)"
"$MINISHELL" <<< "echo hello"$'\x04'

# ===========================
# 7. Quotes
# ===========================
run_test "Double quotes with spaces" "echo \"Hello World\""
run_test "Double quotes with pipe" "echo \"cat lol.c | cat > lol.c\""
run_test "Single quotes normal" "echo 'Hello World'"
run_test "Single quotes environment var" "echo '\$USER'"
run_test "Empty single quotes" "echo ''"

# ===========================
# 8. Environment
# ===========================
run_test "env command" "env"
run_test "export new var" "export MYVAR=test
echo \$MYVAR"
run_test "unset var" "export MYVAR=test
unset MYVAR
echo \$MYVAR"

# ===========================
# 9. cd & pwd
# ===========================
run_test "cd to /tmp" "cd /tmp
pwd"
run_test "cd to invalid dir" "cd /nonexistent"
run_test "cd . and .." "cd .
pwd
cd ..
pwd"

# ===========================
# 10. Relative Paths
# ===========================
run_test "relative path command" "./minishell_test.sh"

# ===========================
# 11. PATH Environment
# ===========================
run_test "Unset PATH" "unset PATH
ls"
run_test "Set PATH with multiple dirs" "export PATH=/bin:/usr/bin
ls"

# ===========================
# 12. Redirections
# ===========================
run_test "Simple redirection >" "echo Hello > test.txt"
run_test "Append redirection >>" "echo World >> test.txt"
run_test "Input redirection <" "cat < test.txt"
run_test "Multiple redirections" "echo Multiple > test.txt
cat < test.txt"

# ===========================
# 13. Pipes
# ===========================
run_test "Simple pipe" "echo Hello | grep H"
run_test "Multiple pipes" "echo Hello World | grep Hello | wc -l"
run_test "Pipe with wrong command" "ls nonexist | grep foo"
run_test "Pipe + redirection" "echo Hello | grep H > out.txt"

# ===========================
# 14. Go Crazy & History
# ===========================
run_test "Invalid command" "dsbksdgbksdghsd"
run_test "Multiple pipes" "cat | cat | ls"
run_test "Long command" "echo a b c d e f g h i j k l m n o p q r s t u v w x y z"

# ===========================
# 15. Environment Variables
# ===========================
run_test "Echo env var" "echo \$USER"
run_test "Double quotes env var" "echo \"\$USER\""
run_test "Check USER existence" "echo \$USER || export USER=testuser
echo \$USER"

# ===========================
# 16. 基础命令额外测试
# ===========================
run_test "echo with -n spaced" "echo -n 'No newline here'"
run_test "pipe single command" "echo 'hello world' | cat"
run_test "double pipe" "echo 'hello world' | cat | cat"
run_test "cat file" "cat test_files/test.txt"
run_test "invalid command" "nonexistentcommand"
run_test "pwd sequence" "pwd
cd test_files
pwd
cd ..
pwd"
run_test "grep minishell" "ls | grep minishell"
run_test "wc word count" "echo 'hello world' | wc -w"
run_test "multiple echo commands" "echo 'First command'
echo 'Second command'
echo 'Third command'"
run_test "which command" "which ls"
run_test "env PATH check" "env | grep PATH"

# ===========================
# 17. 重定向 & Here-Doc
# ===========================
run_test "redirect >" "echo 'test1' > test_files/redir.txt"
run_test "append >>" "echo 'test2' >> test_files/redir.txt"
run_test "cat redirected file" "cat test_files/redir.txt"
run_test "input redirect <" "cat < test_files/redir.txt"
run_test "missing file input redirect" "cat < nonexistent_file.txt"
run_test "multiple redirections" "echo 'redirected text' > test_files/output.txt
cat test_files/output.txt"
run_test "append line to file" "echo 'append line' >> test_files/output.txt"
run_test "here document" "cat << EOF
line1
line2
EOF"

# ===========================
# 18. 管道 & 组合测试
# ===========================
run_test "ls count" "ls | wc -l"
run_test "pipe grep wc" "cat /etc/passwd | grep root | wc -l"
run_test "pipe with invalid" "| ls"
run_test "pipe missing command end" "ls |"
run_test "pipe missing command start" "| ls |"
run_test "invalid pipe sequence" "ls | | wc"

# ===========================
# 19. 环境变量 & PATH
# ===========================
run_test "print undefined var" "echo \$TEST"
run_test "set env var" "export TEST=42
echo \$TEST"
run_test "unset env var" "unset TEST
echo \$TEST"
run_test "unset PATH then ls" "unset PATH
ls"
run_test "set PATH multiple dirs" "export PATH=/bin:/usr/bin
ls"

# ===========================
# 20. 引号 & 转义测试
# ===========================
run_test "single quotes" "echo 'hello world'"
run_test "mixed quotes" "echo hello'world'"
run_test "quotes with backslash" "echo hello'\\world\\'"
run_test "nested quotes" "echo 'this is a \"test\"'"
run_test "escaped quotes" "echo \"hello \\\"world\\\"\""
run_test "multiple escapes" "echo double\\'momo"
run_test "quote sequence" "echo 'lol\\world'"
run_test "empty quotes" "echo ''"

# ===========================
# 21. 复杂组合测试
# ===========================
run_test "multiple commands with pipes and redirects" "echo Hello | grep H > out.txt
cat out.txt"
run_test "long chained command" "echo a b c d e f g h i j k l m n o p q r s t u v w x y z"
run_test "quotes with env var" "echo \"\$USER\""
run_test "quotes + escape" "echo \"Mix 'quotes' and \\\"double quotes\\\"\""

echo -e "\n=== MINISHELL TESTS COMPLETE ==="
