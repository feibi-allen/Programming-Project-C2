/**
 * @file maze.c
 * @author Fei-bi Allen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MAX_HEIGHT 100

typedef struct __Maze
{
    char *completeMaze;
} maze;

typedef struct __item
{
    int cell;
    struct __item *previous;
} item;

// modify pointer to head 
void push(item **head, int cell)
{
    item *tmp = (item*)malloc(sizeof(item));
    tmp->previous= *head;
    (*head) = tmp;
    tmp->cell=cell;
}

int pop(item **head){
    if (*head == NULL){
        return -1;
    }
    printf("%d\n",(*head)->cell);
    item *tmp = *head;
    *head = (*head)->previous;
    free(tmp);
}

int main() {
    item *head = (item*)malloc(sizeof(item));
    head->cell=0;
    head->previous=NULL;
    push(&head,1);
    printf("%d",head->cell);

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