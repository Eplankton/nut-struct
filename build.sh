echo -e "\n\e[32;1m[Build]\e[0m: Begin..."
echo -e "\e[32;1m[Build]\e[0m: Generating binary files..."
rm build -rf
mkdir build
time g++ cpp_type/*.cpp -Wno-psabi -o build/build_example
echo -e "\n\e[32;1m[Build]\e[0m: Success build with no error...\n"
