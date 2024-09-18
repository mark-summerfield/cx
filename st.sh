rm -f core
./doc.sh
clc -sL rb
cppcheck -q --enable=warning --enable=style --enable=performance \
    --enable=portability .
    # --enable=unusedFunction .
malloc_check.py *.[ch]
git st
