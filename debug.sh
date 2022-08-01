echo -e "\n\e[31;1m[Debug]\e[0m: Begin..."
echo -e "\e[31;1m[Debug]\e[0m: Generating binary files..."
time clang++ -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer -O0 -g include/*.cpp -Wno-psabi -o debug
echo -e "\n\e[31;1m[Debug]\e[0m: Success with no error...\n================================"
./debug
rm debug
echo -e "\n================================"
echo -e "\n\e[31;1m[Debug]\e[0m: Exit with no memory leaks...\n"
