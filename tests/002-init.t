Check that git big init works correctly on a git repo
 $ git init
 $ git big init
 = 0
 $ [ -d .git/big ]
 = 0

Check git big init does not work on a non-git folder
 $ git big init
 = 1
 ! "Invalid git repository"
 $ [ -d .git/big ]
 = 1

