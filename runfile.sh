#!/bin/bash
stdbuf -o0 -e0 ./maze mazes/m6by9.txt | tee tmp
