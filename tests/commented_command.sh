echo ---comment everything #output comment everything
# ls -a; echo hello && mkdir test || echo world; git status
echo ---comment 1/5th #output comment 1/5
ls -a #; echo hello && mkdir test || echo world; git status
echo ---comment 2/5th #output comment 2/5
ls -a; echo hello # && mkdir test || echo world; git status
echo ---comment 3/5th #output comment 3/5
ls -a; echo hello && mkdir -p test # || echo world; git status
echo ---comment 4/5th #output comment 4/5
ls -a; echo hello && mkdir -p test || echo world #; git status
echo ---comment 5/5th #output comment 5/5
ls -a; echo hello && mkdir -p test || echo world; git status #
rm -rf test #remove the extra test directory
exit