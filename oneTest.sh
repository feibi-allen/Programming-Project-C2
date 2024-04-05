#!/bin/bash

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