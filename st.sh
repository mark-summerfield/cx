clc -L rb
cppcheck -q --enable=warning --enable=style --enable=performance \
    --enable=portability --enable=unusedFunction .
git st
