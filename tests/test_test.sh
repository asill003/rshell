mkdir -p testfiles1 #make directory
touch testfiles1/file1 #make file

test -e testfiles1 #return true because directory exists
test -e testfiles2 #return false because directory/file does not exists
test -e testfiles1/file1 #return true because file exists
test -e testfiles1/file2 #return false because directory/file does not exist

test -f testfiles1 #return false because it is a directory
test -f testfiles2 #return false because directory/file does not exists
test -f testfiles1/file1 #return true because file exists
test -f testfiles1/file2 #return false because file does not exist

test -d testfiles1 #return true because directory exists
test -d testfiles2 #return false because directory does not exists
test -d testfiles1/file1 #return false because it is a file
test -d testfiles1/file2 #return false because directory does not exist

rm -rf testfiles1 #remove test directory
exit #no infinite loop