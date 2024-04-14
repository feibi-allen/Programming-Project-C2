/**
 * @file maze.c
 * @author Fei-bi Allen
 */

#include <stdio.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MAX_HEIGHT 100

typedef struct __Maze
{
    char *completeMaze;
} maze;

typedef struct __itemOnStack
{   
    int cell;
    stackItem *previousItem;
} stackItem;

int pop(stackItem *head){
    int cell = head->cell;
    stackItem *tmp = head;
    head = head->previousItem;
    free(tmp);
    return cell;
}

void push(stackItem *head, int *cell){
    stackItem *tmp = head;
    head->previousItem=tmp;
    head->cell = cell;
}

int main(int argc, char *argv[]) {
    // error check args
    // asign maze struct grid memory
    // fill grid with walls
    // using working maze:
        // pick random start
        // repeat:  
            // random branch
            // check if already visited
            // if not move to that cell, connect the two cells
            // if is go back along the stack
}