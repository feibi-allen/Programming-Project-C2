#!/bin/bash

echo -n "Walking off map - "
((testCounter++))
timeout 0.5s stdbuf -o0 -e0 ./maze mazes/m5by5.txt < inputs/i5by5OffMap.txt > tmp
if grep -q "You can't move there." tmp && grep -q mazes/m5by5Start.txt tmp;
then
    echo -e "\e[32mPASS\e[0m"
    ((passCounter++))
else
    echo -e "\e[31mFAIL\e[0m"
fi

