time clang++ -std=c++20  include/*.cpp -O2 -Wno-psabi -o main
echo -e "\nCompilation completed.\n\nGenerating data...\n"
./main > log
echo -e "Completed.\n"
rm main