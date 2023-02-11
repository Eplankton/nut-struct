time clang++-15 -O3 -std=c++2b -fno-exceptions src/*.cpp -fno-exceptions -Wno-psabi -o main
echo -e "\nCompilation completed.\n\nGenerating data...\n"
./main > log
echo -e "Completed.\n"
rm main