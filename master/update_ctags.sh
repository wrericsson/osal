 #!/bin/sh
 find . -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.xml"> cscope.files
 cscope -bkq -i cscope.files
 ctags -R
