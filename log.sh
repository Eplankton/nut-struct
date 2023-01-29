time clang++ -std=c++20 -fno-exceptions include/*.cpp -O3 -Wno-psabi -o main
echo -e "\nCompilation completed.\n\nGenerating data...\n"
./main > log
echo -e "Completed.\n"
rm main