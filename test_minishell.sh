#!/bin/bash

# Test script for minishell
# Make sure to compile minishell first: make

echo "=== MINISHELL TESTING SCRIPT ==="
echo "Make sure you have compiled minishell first!"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Create test directory
mkdir -p test_files
cd test_files

# Create some test files
echo "Hello World" > test.txt
echo "Line 1" > file1.txt
echo "Line 2" >> file1.txt
echo "This is a test file" > file2.txt

cd ..

echo -e "${YELLOW}Testing basic commands...${NC}"
echo ""

# Test 1: Basic commands
echo "Test 1: Basic echo command"
echo "echo hello world" | ./minishell
echo ""

# Test 2: pwd command
echo "Test 2: pwd command"
echo "pwd" | ./minishell
echo ""

# Test 3: ls command
echo "Test 3: ls command"
echo "ls" | ./minishell
echo ""

# Test 4: Built-in commands
echo "Test 4: Built-in commands"
echo -e "echo 'Testing echo builtin'\npwd\nenv | head -5" | ./minishell
echo ""

# Test 5: Command with arguments
echo "Test 5: Command with arguments"
echo "echo -n 'No newline here'" | ./minishell
echo ""
echo ""

# Test 6: Simple pipe
echo "Test 6: Simple pipe"
echo "echo 'hello world' | cat" | ./minishell
echo ""

# Test 7: Multiple pipes
echo "Test 7: Multiple pipes"
echo "echo 'hello world' | cat | cat" | ./minishell
echo ""

# Test 8: File operations
echo "Test 8: File operations"
echo "cat test_files/test.txt" | ./minishell
echo ""

# Test 9: Error handling
echo "Test 9: Error handling (command not found)"
echo "nonexistentcommand" | ./minishell
echo ""

# Test 10: cd command
echo "Test 10: cd command"
echo -e "pwd\ncd test_files\npwd\ncd ..\npwd" | ./minishell
echo ""

# Test 11: Complex pipe with grep
echo "Test 11: Complex pipe with grep"
echo "ls | grep minishell" | ./minishell
echo ""

# Test 12: wc command
echo "Test 12: wc command"
echo "echo 'hello world' | wc -w" | ./minishell
echo ""

# Test 13: Multiple commands
echo "Test 13: Multiple simple commands"
echo -e "echo 'First command'\necho 'Second command'\necho 'Third command'" | ./minishell
echo ""

# Test 14: PATH resolution
echo "Test 14: PATH resolution"
echo "which ls" | ./minishell
echo ""

# Test 15: Environment variables
echo "Test 15: Environment variables"
echo "env | grep PATH" | ./minishell
echo ""

# Test 16: Complex pipe
echo "Test 16: Complex pipe"
echo "echo 'hello' | cat -e" | ./minishell
echo ""

# Test 17: Complex pipe with variable
echo "Test 16: Complex pipe with variable"
echo "cat /etc/passwd | grep $USER | cut -d: -f1" | ./minishell
echo ""

# Test 18: Quotes
echo "Test 18: Quotes"
echo "echo \"this is a 'test'\"" | ./minishell
echo ""

# Test 18: Quotes
echo "Test 18: Quotes"
echo " echo "\'mix\'"" | ./minishell
echo ""

# Test 19: Quotes
echo "Test 19: Quotes"
echo "echo 'this is a \"test\"'" | ./minishell
echo ""

# Test 20: slashs
echo "Test 20: slashs"
echo "echo hello\ world" | ./minishell
echo ""

# Test 20: slashs
echo "Test 20: slashs"
echo "echo 'hello\ world'" | ./minishell
echo ""

# Test 21: Mix quotes and slashes
echo "Test 21: Mix quotes and slashes"
echo "echo \"Mix 'quotes' and "double quotes"\"" | ./minishell
echo ""

# Test 22: Export environment variables
# echo "Test 22: Export environment variables"
# echo "export TEST_VAR=minishell\necho $TEST_VAR\nunset TEST_VAR\necho $TEST_VAR" | ./minishell
# echo ""
# 
# # Test 23: Unset variable and expansion
# echo "Test 23: Unset variable and expansion"
# echo "export VAR52=test; unset VAR52; echo \$VAR52" | ./minishell  # expect empty
# echo ""

# Test 24: Redirection edge cases
echo "Test 24: Redirection edge cases"
echo "echo 'test1' > test_files/redir.txt" | ./minishell
echo "echo 'test2' >> test_files/redir.txt" | ./minishell
cat test_files/redir.txt
echo "cat < test_files/redir.txt" | ./minishell
echo "cat < nonexistent_file.txt" | ./minishell         # expect error handling
echo ""

# Test 25: Error test-1
echo "Test 25: Error test-1"
echo "cat nonexistentfile" | ./minishell
echo ""

# Test 26: Error test-2
echo "Test 26: Error test-2"
echo "ls /root 2> error.txt\necho error.txt" | ./minishell
echo ""

echo -e "${GREEN}=== INTERACTIVE TESTING ===${NC}"
echo "Now you can test interactively. Try these commands:"
echo "1. Basic commands: ls, pwd, echo hello"
echo "2. Pipes: ls | grep mini"
echo "3. Built-ins: cd, pwd, env"
echo "4. Complex: cat test_files/test.txt | wc -l"
echo "5. Type 'exit' to quit"
echo ""

# Test 27: 重定向输出到文件
echo "Test 27: Output redirection"
echo "echo 'redirected text' > test_files/output.txt" | ./minishell
cat test_files/output.txt
echo ""

# Test 28: 输入重定向
echo "Test 28: Input redirection"
echo "cat < test_files/test.txt" | ./minishell
echo ""

# Test 29: 输出追加 >>
echo "Test 29: Output append redirection"
echo "echo 'append line' >> test_files/output.txt" | ./minishell
cat test_files/output.txt
echo ""

# Test 30: Here document
echo "Test 30: Here document"
echo "cat << EOF
line1
line2
EOF" | ./minishell
echo ""

# Test 31: 空命令
echo "Test 31: Empty command"
echo "" | ./minishell
echo "echo ''" | ./minishell
echo ""

# Test 32: 空格处理
echo "Test 32: Whitespace handling"
echo "        echo spaced" | ./minishell
echo ""

# Test 33: 超长输入 (4096+ chars)
echo "Test 33: Very long input"
LONG_CMD=$(yes "a" | head -n 5000 | tr -d '\n')
echo "echo $LONG_CMD" | ./minishell
echo ""

# Test 34: 组合逻辑 &&
#echo "Test 34: Logical AND"
#echo "echo first && echo second" | ./minishell
#echo ""
#
## Test 35: 组合逻辑 ||
#echo "Test 35: Logical OR"
#echo "false || echo 'fallback executed'" | ./minishell
#echo ""

# Test 36: Env variable with spaces
echo "Test 36: Env variable with spaces"
echo "export VAR57='hello world'; echo \"\$VAR57\"" | ./minishell   # expect hello world
echo ""

# Test 37: 重定向错误文件
echo "Test 37: Invalid redirection"
echo "cat > /root/forbidden.txt" | ./minishell
echo ""

# Test 38: 文件描述符错误
echo "Test 38: Bad file descriptor"
echo "echo hello 1>&9" | ./minishell
echo ""

# Test 39: unset 特殊变量
echo "Test 39: Unset PATH and try ls"
echo -e "unset PATH\nls" | ./minishell
echo ""

# Test 40: 环境变量展开边界情况
echo "Test 40: Environment variable edge cases"
echo -e "export EMPTY=\necho \$EMPTY\necho \$NOT_DEFINED" | ./minishell
echo ""

# Test 41: 多重引号嵌套
# echo "Test 41: Nested quotes"
# echo "echo \"double 'inside'\"; echo 'single \"inside\"'" | ./minishell
# echo ""

# Test 42: 混合管道和重定向
echo "Test 42: Pipe with redirection"
echo "cat test_files/test.txt | grep Hello > test_files/grep_out.txt" | ./minishell
cat test_files/grep_out.txt
echo ""

# Test 43: Nested quotes and escaping
echo "Test 43: Nested quotes and escaping"
echo "echo \"outer 'inner' \\\"double\\\" end\"" | ./minishell
echo ""

# Test 44: exit 状态码
echo "Test 44: Exit status"
echo "false; echo \$?" | ./minishell
echo "true; echo \$?" | ./minishell
echo ""

# Test 45: Trap Ctrl-D (EOF)
echo "Test 45: Ctrl-D simulation (expect exit)"
printf "exit\n" | ./minishell
echo ""

# Test 46: Redirections with pipes and variables
echo "Test 46: Redirections with pipes and variables"
echo "cat < test_files/test.txt | grep \"\$JU\" | wc -l" | ./minishell
echo "cat < test_files/test.txt | grep \$JU | wc -l" | ./minishell
echo "echo \"hello world\" > test_files/out.txt" | ./minishell
cat test_files/out.txt
echo "echo \"hello world\" >> test_files/out.txt" | ./minishell
cat test_files/out.txt
echo "echo \"hello world\" | grep o | wc -l > test_files/out.txt" | ./minishell
cat test_files/out.txt
echo "echo \"\$JU\"" | ./minishell
echo "echo \$JU" | ./minishell
echo ""

# Test 47: Invalid commands
echo "Test 47: Invalid commands"
echo "echoxxx" | ./minishell
echo "ls /nonexistent_dir" | ./minishell
echo "cat /nonexistent_file" | ./minishell

# Test 48: Spaces and tabs
echo "Test 48: Spaces and tabs"
echo "        echo spaced" | ./minishell
echo -e "echo\thello\tworld" | ./minishell
echo ""

# Test 49: Arguments for commands
echo "Test 49: Command arguments"
echo "ls" | ./minishell
echo "pwd" | ./minishell
echo "echo hello" | ./minishell
echo "whoami" | ./minishell
echo "date" | ./minishell
echo "ls -l" | ./minishell
echo "ls -a -l" | ./minishell
echo "echo hello world" | ./minishell
echo "cat /etc/passwd" | ./minishell
echo "cd .." | ./minishell
echo "cd /" | ./minishell
echo "cd juju\\ mm" | ./minishell
echo "export TEST=123" | ./minishell
echo "env" | ./minishell
echo "unset TEST" | ./minishell
echo "exit" | ./minishell
echo "exit oo" | ./minishell
echo ""

# Test 50: Pipes
echo "Test 50: Pipes"
echo "ls | wc -l" | ./minishell
echo "cat /etc/passwd | grep root | wc -l" | ./minishell
echo "echo hello | cat" | ./minishell
echo "| ls" | ./minishell
echo "ls |" | ./minishell
echo "| ls |" | ./minishell
echo "ls | | wc" | ./minishell
echo ""

# Test 51: Redirections
echo "Test 51: Redirections"
echo "echo hello > out.txt" | ./minishell
echo "cat < out.txt" | ./minishell
echo "ls >> out.txt" | ./minishell
echo "ls >" | ./minishell
echo "ls <" | ./minishell
echo "ls >>" | ./minishell
echo "ls > file1 > file2" | ./minishell
echo ""

# Test 52: Environment variables
echo "Test 52: Environment variables"
echo "echo \$TEST" | ./minishell
echo "export TEST=42" | ./minishell
echo "echo \$TEST" | ./minishell
echo "unset TEST" | ./minishell
echo "echo \$TEST" | ./minishell
echo ""

# Test 53: PATH environment
echo "Test 53: PATH environment"
echo "ls" | ./minishell
echo "export PATH=" | ./minishell
echo "echo \$PATH" | ./minishell
echo "ls" | ./minishell
echo ""

# Test 54: Here document
echo "Test 54: Here document"
echo -e "cat << EOF < out.txt\nhehe\nhaha\nEOF" | ./minishell
echo "cat out.txt" | ./minishell
echo -e "cat << EOF > out.txt\nline1\nline2\nEOF" | ./minishell
echo -e "cat << EOF >> out.txt\nline3\nEOF" | ./minishell
echo ""

# Test 55: Pipes with multiple commands
echo "Test 55: Multiple pipes"
echo "cat | cat | ls" | ./minishell
echo ""

# Test 56: Executable paths
echo "Test 56: Executable paths"
echo "/bin/ls" | ./minishell
echo "../bin/ls" | ./minishell
echo "../../../../../../bin/ls" | ./minishell
echo ""

# Test 57: Exit status variable
echo "Test 57: Exit status variable"
true | ./minishell
false | ./minishell
echo "echo \$?" | ./minishell  # should print exit status of previous command
echo ""

# ========================
# Single quotes
# ========================
echo "Test 47: Single quotes handling"
# ok cases
echo "echo 'hello world'" | ./minishell
echo "echo hello'world'" | ./minishell
echo "echo hello'\\world\\'" | ./minishell   # expect ****hello\world\
echo "echo hello'world\\'" | ./minishell     # ****helloworld\
echo "echo 'lol'world''" | ./minishell       # ****lolworld
echo "echo 'lol\\'world''" | ./minishell     # ****lol\world
echo "echo 'lol\\world'" | ./minishell       # ****lol\world
echo "echo world''" | ./minishell            # ****world
echo "echo 'mix quote\\'" | ./minishell      # ****mix quote\
echo "echo '\\'wolrd\\''" | ./minishell      # expect error (unclosed quote)
echo "echo 'mix \\'quote' and \\\"double\\\"\"" | ./minishell # expect error (unclosed quote)
echo "echo world\\''" | ./minishell          # expect error (unclosed quote)
echo ""

# ========================
# Backslash handling
# ========================
echo "Test 48: Backslash handling"
echo "echo \\\\lolo" | ./minishell          # ****\lolo
echo "echo hello\\lolo" | ./minishell       # ****hellololo
echo "echo double\\ momo" | ./minishell     # ****double momo
echo "echo \\double\\momo" | ./minishell    # ****doublemomo
echo "echo double\\'momo" | ./minishell     # ****double'momo
echo "echo double\\\"momo" | ./minishell    # ****double\"momo
echo "echo quote\\'" | ./minishell          # ****quote'
echo "echo mix\\hello\\" | ./minishell      # expect error (incorrect syntax)
echo ""

# ========================
# Double quotes
# ========================
echo "Test 49: Double quotes handling"
echo "echo \"hello world\"" | ./minishell
echo "echo \"hello\"world\"\"" | ./minishell
echo "echo hello\"world\"" | ./minishell
echo "echo \"double\\ momo\"" | ./minishell      # ****double\ momo
echo "echo \"hello\\\"world\\\"\"" | ./minishell # ****hello"world"
echo "echo \"hello \\'world\\'\"" | ./minishell  # ****hello \'world\'
echo "echo hello\"\\'world\\'\"" | ./minishell   # ****hello\'world\'
echo "echo mix\"hello \\'world\\'\"" | ./minishell # ****mixhello \'world\'
echo "echo helo\"\"" | ./minishell               # ****helo
echo "echo helo\"lolo \fufu\"" | ./minishell     # ****helololo \fufu
echo "echo \"mix \\'quote' and \\\"double\\\"\"" | ./minishell  # ****mix \'quote' and "double"
echo "echo \"mix \\'quote\\' and \\\"double\\\"\"" | ./minishell # ****mix \'quote\' and "double"
echo "echo \" kiki just us\\ lolo\"" | ./minishell # **** kiki just us\ lolo
echo "echo helo\"lolo \\\"lolo" | ./minishell     # expect error (unclosed)
echo "echo helo\"lolo \\\"" | ./minishell         # expect error (unclosed)
echo "echo \"ff\\\"" | ./minishell                # expect error (unclosed)
echo ""

# ========================
# Double quotes with variables
# ========================
echo "Test 50: Double quotes with variables"
echo "export JU=ji MOMO=kk" | ./minishell
echo "echo \"mix \$JU\"" | ./minishell          # mix ji
echo "echo \"mix \"\$JU\"\"" | ./minishell      # mix ji
echo "echo mix\"\$JU\"" | ./minishell           # mixji
echo "echo \"\$JU\\ \$MOMO\"" | ./minishell     # ji\ kk
echo "echo \"\$JU\\\"\$MOMO\\\"\"" | ./minishell # ji"kk"
echo "echo \"\$JU \\'\$MOMO\\'\"" | ./minishell # ji \'kk\'
echo "echo hello\"\\'world\\'\"" | ./minishell
echo "echo mix\"hello \\'world\\'\"" | ./minishell
echo "echo helo\"\"" | ./minishell
echo "echo helo\"lolo \fufu\"" | ./minishell
echo "echo \"mix \\'quote' and \\\"double\\\"\"" | ./minishell
echo "echo \"mix \\'quote\\' and \\\"double\\\"\"" | ./minishell
echo "echo \" kiki just us\\ lolo\"" | ./minishell
echo "echo helo\"lolo \\\"lolo" | ./minishell   # expect error (unclosed)
echo "echo helo\"lolo \\\"" | ./minishell       # expect error (unclosed)
echo "echo \"ff\\\"" | ./minishell              # expect error (unclosed)
echo ""

# Cleanup
# rm -rf test_files

echo -e "${YELLOW}Starting interactive minishell...${NC}"
./minishell