./log.sh < test_data
echo -e "Drawing graph...\n"
python test.py
echo -e "Completed.\n"
rm log