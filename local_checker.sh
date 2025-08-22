#!/bin/sh
# Local checker for simple_shell (basic tests)

HSH="./hsh"

echo "=== Test 1: Execute /bin/ls ==="
echo "/bin/ls" | $HSH

echo
echo "=== Test 2: Execute ls with arguments ==="
echo "ls -l /tmp" | $HSH

echo
echo "=== Test 3: Execute relative command (copy /bin/ls to ./hbtn_ls) ==="
cp /bin/ls ./hbtn_ls
echo "./hbtn_ls /var" | $HSH

echo
echo "=== Test 4: Execute command with extra spaces ==="
echo "   /bin/ls    -l    " | $HSH

echo
echo "=== Test 5: Handle empty input and EOF ==="
echo "\n\n" | $HSH

echo
echo "=== Test 6: Command not found ==="
echo "qwerty" | $HSH

echo
echo "=== Test 7: Remove PATH and execute command ==="
PATH="" sh -c "$HSH <<<'/bin/ls'"

echo
echo "=== Memory check with Valgrind ==="
valgrind --leak-check=full $HSH <<<'/bin/ls'

