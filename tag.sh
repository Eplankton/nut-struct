echo -e "\n\e[33;1m[Tag]\e[0m: Generating...\n"
cd cpp_type
ctags -R *
cd ..
