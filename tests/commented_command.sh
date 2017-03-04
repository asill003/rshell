#!bin/rshell
echo testing: commented commands

echo ---comment everything
# ls -a; echo hello && mkdir test || echo world; git status

echo ---comment 1/5th
ls -a; # echo hello && mkdir test || echo world; git status

echo ---comment 2/5th
ls -a; echo hello # && mkdir test || echo world; git status

echo ---comment 3/5th
ls -a; echo hello && mkdir test # || echo world; git status

echo ---comment 4/5th
ls -a; echo hello && mkdir test || echo world #; git status

echo ---comment 5/5th
ls -a; echo hello && mkdir test || echo world; git status #

exit
