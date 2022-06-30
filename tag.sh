echo -e "\n\e[36;1m[Tag]\e[0m: Generating..."
cd cpp_type
ctags -R *
cd ..
echo -e "\e[36;1m[Tag]\e[0m: Done!\n"
