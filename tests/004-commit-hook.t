Check that a big file can be committed with the .gitattributes file
 $ git init
 $ git big init
 $ echo "*.txt filter=big"
 -> .gitattributes
 $ echo "Test"
 -> test.txt
 $ git add test.txt
 $ git add .gitattributes
 $ git commit -m "Test..."
 = 0

Check that a big file cannot be committed with the .gitattributes file
 $ git init
 $ git big init
 $ echo "*.bmp filter=big"
 -> .gitattributes
 $ git add .gitattributes
 $ echo "*.txt filter=big"
 -> .gitattributes
 $ echo "Test"
 -> test.txt
 $ git add test.txt
 $ git commit -m "Test..."
 = 1

