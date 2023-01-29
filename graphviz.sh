cd include/

clang++ \
$1 \
-S \
-std=c++20 \
-fno-exceptions \
-emit-llvm *.cpp \
-o - | \
opt -enable-new-pm=0  -analyze -dot-callgraph --callgraph-dot-filename-prefix=test > log
rm log

cat test.callgraph.dot \
 | c++filt -p \
 | sed 's,>,\\>,g; s,-\\>,->,g; s,<,\\<,g; s/nuts:://g; s/std:://g' \
 | gawk '/external node/{id=$1} $1 != id' > test.dot

rm test.callgraph.dot
mv test.dot ..
cd ..
# dot -Tsvg test.dot > test.svg