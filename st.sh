rm -f core
clc -L rb
FILES=( 
    set_int.c
    cx_test.c
    cx_util_test.c
    sx.c
    sx_test.c
    va_test.c
    vec.c
    vec_common_test.c
    vec_int.c
    vec_int_test.c
    vec_str.c
    vec_str_test.c
    vec_test.c
)
cppcheck -q --enable=warning --enable=style --enable=performance \
    --enable=portability --enable=unusedFunction "${FILES[@]}"
git st
