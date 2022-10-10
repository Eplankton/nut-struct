echo -e "\n\e[31;1m[Debug]\e[0m: Begin..."
echo -e "\e[31;1m[Debug]\e[0m: Generating binary files..."
time clang++ -fsanitize=address -std=c++20 -fsanitize=leak -fno-omit-frame-pointer -Og -g include/*.cpp -Wno-psabi -o debug
echo -e "\n\e[31;1m[Debug]\e[0m: Success with no error...\n\n================================"
./debug
rm debug
echo -e "================================"
echo -e "\n\e[31;1m[Debug]\e[0m: Exit with no memory leaks...\n\n"
