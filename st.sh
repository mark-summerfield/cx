rm -f core
clc -sL rb
FILES=(*.c)
cppcheck -q --enable=warning --enable=style --enable=performance \
    --enable=portability --enable=unusedFunction "${FILES[@]}"
malloc_check.py "${FILES[@]}"
git st
