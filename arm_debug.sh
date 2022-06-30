echo -e "\n\e[33;1m[Debug]\e[0m: Begin..."
echo -e "\e[33;1m[Debug]\e[0m: Generating binary files..."
time clang++ -O0 -g cpp_type/*.cpp -Wno-psabi -o debug
echo -e "\n\e[33;1m[Debug]\e[0m: Success with no error...\n"
valgrind --leak-check=full ./debug
rm debug
echo -e "\n\e[33;1m[Debug]\e[0m: Exit with no memory leaks...\n"
