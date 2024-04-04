echo -n "Maze ends with new line - "
((testCounter++))
./maze mazes/mNewLineChar.txt > tmp
if grep -q "Maze mazes/mNewLineChar.txt loaded successfully" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi