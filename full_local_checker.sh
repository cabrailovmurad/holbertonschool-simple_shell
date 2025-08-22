#!/bin/sh
# Full local checker for simple_shell 0.3 (problematic cases)
# Compatible with minimal sh / container

HSH="./hsh"

echo "=== Test 3: Execute relative command (./hbtn_ls) ==="
cp /bin/ls ./hbtn_ls
echo "./hbtn_ls /var" | $HSH

echo
echo "=== Test 5: Execute ls with leading/trailing spaces ==="
echo "   ls   " | $HSH
echo " ls -l " | $HSH

echo
echo "=== Test 11: Execute /bin/ls surrounded by spaces ==="
echo "   /bin/ls   " | $HSH

echo
echo "=== Test 15: Execute command that does not exist ==="
echo "qwerty" | $HSH

echo
echo "=== Test 16: PATH empty, execute ls ==="
echo "ls" | PATH="" $HSH

echo
echo "=== Test 17: PATH empty, execute non-existent command ==="
echo "foobar" | PATH="" $HSH

echo
echo "=== Test 21: Remove all environment variables, execute /bin/ls ==="
env -i $HSH <<< "/bin/ls"

echo
echo "=== Test 22: PATH does not contain /bin, execute ls ==="
PATH="/usr/local/bin" echo "ls" | $HSH

echo
echo "=== Test 26: Execute /bin/ls multiple times ==="
echo "/bin/ls" | $HSH
echo "/bin/ls" | $HSH
echo "/bin/ls" | $HSH

echo
echo "=== Test 27: Execute ls multiple times with spaces ==="
echo "   ls   " | $HSH
echo "ls" | $HSH
echo "  ls -l /tmp  " | $HSH

echo
echo "=== Test 28: fork should not be called when command does not exist ==="
echo "nonexistentcmd" | $HSH

echo
echo "=== Test 29: Execute command with relative path in current dir ==="
cp /bin/ls ./.hbtnls
echo "./.hbtnls /var" | $HSH

echo
echo "=== Test 30: Remove PATH, set PATH1, execute ls ==="
PATH="" PATH1="/usr/bin" echo "ls" | $HSH

echo
echo "=== Test 32: Memory check with Valgrind ==="
echo "/bin/ls" | valgrind --leak-check=full $HSH
