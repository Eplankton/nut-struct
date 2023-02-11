echo -e "\n\e[31;1m[Debug]\e[0m: Begin..."
echo -e "\e[31;1m[Debug]\e[0m: Generating binary files..."

time clang++-15 \
             -fsanitize=address \
             -std=c++2b \
             -fsanitize=leak \
             -fno-omit-frame-pointer \
             -Og \
             -g src/*.cpp \
             -Wno-psabi \
             -o debug

echo -e "\n\e[31;1m[Debug]\e[0m: Success with no error...\n\n================================\n"
./debug
rm debug
echo -e "\n================================"
echo -e "\n\e[31;1m[Debug]\e[0m: Exit with no memory leaks...\n\n"
