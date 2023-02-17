cd src/

g++-12 \
$1 \
-g \
*.cpp \
-pg \
-std=c++23 \
-fno-exceptions \
-fno-omit-frame-pointer \
-o main

# -fno-inline-functions \
# -fno-inline-functions-called-once \

./main

gprof --brief main gmon.out > tmp.txt
gprof2dot -s tmp.txt > tmp.dot

# -n0 -e0
# -s

cat tmp.dot \
 | c++filt -p \
 | sed 's,>,\\>,g; s,-\\>,->,g; s,<,\\<,g; s/nuts:://g; s/std:://g' \
 | gawk '/external node/{id=$1} $1 != id' > ../result.dot

rm main
rm gmon.out
rm tmp.dot
rm tmp.txt

# dot -Tsvg result.dot > result.svg

cd ..