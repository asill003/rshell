#!bin/rshell
echo testing: single command

echo working=ls -al ::: failing=rm tests

echo ---working command
ls -al

echo ---failing command
rm tests

exit
