echo -e "\n\n\e[36;1m[Demo]\e[0m: Begin..."
echo -e "\e[36;1m[Demo]\e[0m: Generating binary files..."

time clang++ \
    -std=c++20 \
    include/*.cpp \
    -O2 \
    -Wno-psabi \
    -o demo

echo -e "\n\e[36;1m[Demo]\e[0m: Success with no error...\n\n================================\n"
./demo
rm demo
echo -e "\n================================"
echo -e "\n\e[36;1m[Demo]\e[0m: Exit...\n\n"
