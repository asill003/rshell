#!bin/rshell
echo testing: multiple commands

echo working=ls -a ::: failing=rm tests

echo ---working or working
ls -a || ls -a
echo ---working or failing
ls -a || rm tests

echo ---failing or working
rm tests || ls -a
echo ---failing or failing
rm tests || rm tests

echo ---working and working
ls -a && ls -a
echo ---working and failing
ls -a && rm tests

echo ---failing and working
rm tests && ls -a
echo ---failing and failing
rm tests && rm tests

echo ---working semicolon working
ls -a; ls -a
echo ---working semicolon failing
ls -a; rm tests

echo ---failing semicolon working
rm tests; ls -a
echo ---failing semicolon failing
rm tests; rm tests

echo ---chain commands: semicolon -> and -> or -> semiclon
ls -a; echo hello && mkdir test || echo world; git status

exit
