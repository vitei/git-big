Check that a big file is correctly added
 $ git init
 $ git big init
 $ echo "*.txt filter=big"
 -> .gitattributes
 $ echo "Test"
 -> test.txt
 $ git add test.txt
 = 0
 $ [ -f .git/big/1c68ea370b40c06fcaf7f26c8b1dba9d9caf5dea ]
 = 0

