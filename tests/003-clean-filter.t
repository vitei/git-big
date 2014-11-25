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
 $ [ $(cat .git/big/1c68ea370b40c06fcaf7f26c8b1dba9d9caf5dea) = "Test" ]
 = 0
 $ git show :test.txt
 - "GIT-BIG00011c68ea370b40c06fcaf7f26c8b1dba9d9caf5dea"

