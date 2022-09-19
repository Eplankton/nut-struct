time clang++ -std=c++20  include/*.cpp -Og -Wno-psabi -o main
./main > log
rm main