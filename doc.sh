codedoc --author 'Mark Summerfield' --title 'C eXtensions library (cx)' \
    --copyright "" --css doc.css \
    `efind .c|grep -v test` > cx.html
./fixhtml.py cx.html
