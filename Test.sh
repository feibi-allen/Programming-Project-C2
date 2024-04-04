#!/bin/bash

testCounter=0
passCounter=0

## Argument tests

echo -e "\e[34m\n~~Command Line Args~~\e[0m"

# No arguments
# Expected output - "Usage: ./maze <filename>"

echo -n "No arguments - "
((testCounter++))
./maze > tmp
if grep -q "Usage: ./maze <filename>" tmp;
then
    echo -e "\e[32mPASS\e[0m"
((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Too many arguments
# Expected output - "Usage: ./maze <filename>"

echo -n "Two arguments - "
((testCounter++))
./maze mazes/m5by5.txt mazes/m100by100.txt > tmp
if grep -q "Usage: ./maze <filename>" tmp;
then 
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

## File tests

echo -e "\e[34m\n~~File Handling~~\e[0m"

# File doesnt exist
# Expected output - "Error: Could not open file"

echo -n "File does not exist - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze fake.txt > tmp
if grep -q "Error: Could not open file" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Bad permissions (no permission to access the file)
# Expected output - "Error: Could not open file"

echo -n "No permission to access file - "
((testCounter++))
chmod -r mazes/perms.txt
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/perms.txt > tmp
if grep -q "Error: Could not open file" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

chmod +r mazes/perms.txt

rm -f tmp

# Invalid data (contains invalid chars)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (invalid data) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/invalid7by7.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# No maze (empty file)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (empty File) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/empty.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze column is too big (column size is 101)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (maze column is too big) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m100by101.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze column is too big (column size is 4)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (maze column is too small) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m5by4.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze row is too big (row size is 101)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (maze row is too big) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m101by100.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze row is too small (row size is 4)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (maze row is too small) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m4by5.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze is not rectangle (one row is different size)
# Expected output - "Error: Invalid maze"

echo -n "Bad file (non rectangle) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/nonRectangle.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze has no start
# Expected output - "Error: Invalid maze"

echo -n "Bad file (no start) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/noStart.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze has more then one start (two "S")
# Expected output - "Error: Invalid maze"

echo -n "Bad file (two start) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/twoStart.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze has no end
# Expected output - "Error: Invalid maze"

echo -n "Bad file (no end) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/noEnd.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze has more then one end (two "E")
# Expected output - "Error: Invalid maze"

echo -n "Bad file (two ends) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/twoEnds.txt > tmp
if grep -q "Error: Invalid maze" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze ends in new line
# Expected output - maze is loaded successfully and message with file name is shown

echo -n "Maze ends with new line - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/mNewLineChar.txt > tmp
if grep -q "Maze mazes/mNewLineChar.txt loaded successfully" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp


# Maze is maximum size (100 by 100)
# Expected output - maze is loaded successfully and message with file name is shown

echo -n "Max size - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m100by100.txt > tmp
if grep -q "Maze mazes/m100by100.txt loaded successfully" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze is minimum size (5 by 5)
# Expected output - maze is loaded successfully and message with file name is shown

echo -n "Min size - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m5by5.txt > tmp
if grep -q "Maze mazes/m5by5.txt loaded successfully" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Maze is rectangle (columns and rows are not the same value)
# Expected output - maze is loaded successfully and message with file name is shown

echo -n "Rectangle with different columns and rows- "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m6by9.txt > tmp
if grep -q "Maze mazes/m6by9.txt loaded successfully" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

## User input

echo -e "\e[34m\n~~Bad Inputs~~\e[0m"

# No characters entered
# Expected output - error message "Error: Invalid move option" is shown

echo -n "No input - "
((testCounter++))
echo "" | timeout 0.2s ./maze mazes/m5by5.txt > tmp
if grep -q "Error: Invalid move option" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Too many characters entered (two characters)
# Expected output - error message "Error: Invalid move option" is shown

echo -n "Too many inputs - "
((testCounter++))
echo "WW" | timeout 0.2s ./maze mazes/m5by5.txt > tmp
if grep -q "Error: Invalid move option" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Invalid characters
# Expected output - when invalid char "Q" is input error message "Error: Invalid move option" is shown

echo -n "Invalid input - "
((testCounter++))
echo "Q" | timeout 0.2s ./maze mazes/m5by5.txt > tmp
if grep -q "Error: Invalid move option" tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

## Bad moves

echo -e "\e[34m\n~~Invalid moves~~\e[0m"

# Walking into a wall
# Expected output - "You can't move there." message and player has not moved on map

echo -n "Walking into wall - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m7by7.txt < inputs/i7by7Wall.txt > tmp
if grep -q "You can't move there." tmp && grep -qf mazes/m7by7W.txt tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Walking off maze
# Expected output - "You can't move there." message and player has not moved on map

echo -n "Walking off map - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m5by5.txt < inputs/i5by5OffMap.txt > tmp
if grep -q "You can't move there." tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

## Output

echo -e "\e[34m\n~~Successful Cases~~\e[0m"

rm -f tmp

# Display after a move (upper case M)
# Expected output - maze with "X" in position above "S" starting position

echo -n "Display map (forward move) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m6by6.txt< inputs/i6by6ForwardMove.txt > tmp
if grep -qf mazes/m6by6W.txt tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Display when moving back onto start
# Expected output - "X" is displayed instead of S when player moves onto start pos

echo -n "Display maze (move to start) - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m6by6.txt < inputs/i6by6Start.txt > tmp
if grep -qf mazes/m6by6Start.txt tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp

# Finish maze message whe user moves to "E" position in maze
# Expected output - "Congratulations! You finished the maze."

echo -n "Finish maze - "
((testCounter++))
echo "S" | timeout 0.2s ./maze mazes/m6by6.txt > tmp
if grep -q "Congratulations! You finished the maze." tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

rm -f tmp


## print test results

echo -e "\e[35m\n~~Test Results~~\e[0m"
echo "Total tests: $testCounter"
echo "Passed tests: $passCounter"