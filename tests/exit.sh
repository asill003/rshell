#!/bin/sh
echo testing: exit with other commands

mkdir exits

echo ---exit byitself
echo "exit" > exits/oneexit.sh
bin/rshell exits/onexit.sh

echo ---exit with and
echo "ls -a && exit" > exits/andexit.sh
bin/rshell exits/andexit.sh

echo ---exit with or
echo "rm tests || exit" > exits/orexit.sh
bin/rshell exits/orexit.sh

echo ---exit with semicolon
echo "ls -a; exit" > exits/scexit.sh
bin/rshell exits/scexit.sh

echo ---exit with multiple
echo "ls -a || exit && ls -a; exit" > exits/chainexit.sh
bin/rshell exits/chainexit.sh

rm -rf exits

exit
