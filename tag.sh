echo -e "\n\e[36;1m[Tag]\e[0m: Generating..."
cd include
ctags -R *
cd ..
echo -e "\e[36;1m[Tag]\e[0m: Done!\n"
