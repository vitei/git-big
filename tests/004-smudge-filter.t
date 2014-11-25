Check a file can be correctly checked out
 $ git init
 $ git big init
 $ echo "*.txt filter=big"
 -> .gitattributes
 $ echo "Test1"
 -> test.txt
 $ git add .gitattributes test.txt
 $ git commit -m "Version1"
 $ echo "Test2"
 -> test.txt
 $ git add test.txt
 $ git commit -m "Version2"
 $ git checkout HEAD^
 = 0
 $ [ $(cat test.txt) = "Test1" ]
 = 0
 $ cat test.txt
 $ git checkout master
 = 0
 $ [ $(cat test.txt) = "Test2" ]
 = 0

