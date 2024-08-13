# ligadoc -x macros.adoc
asciidoctor -v -r ./doc.rb -a stylesheet=./doc.css -o cx.html doc.adoc
