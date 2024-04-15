/**
 * @file maze.c
 * @author Fei-bi Allen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MAX_HEIGHT 100

#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1

typedef struct __Maze
{
    char *grid;
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
    tmp->cell=cell;
    tmp->previous= *head;
    (*head) = tmp;
}

int pop(item **head){
    if (*head == NULL){
        return -1;
    }
    int cell = (*head)->cell;
    item *tmp = *head;
    *head = (*head)->previous;
    free(tmp);
    return cell;
}

int main(int argc, char *argv[]) {
    // error check args
    if (argc !=4){
        printf("Usage: ./maze <filename>\n");
        return 1;
    }

    int height = atoi(argv[3]);
    int width = atoi(argv[2]);

    if (height<MIN_HEIGHT||height>MAX_HEIGHT||width<MIN_WIDTH||width>MAX_WIDTH){
        printf("Usage: ./maze <filename>\n");
        return 1;
    }

    // asign maze struct grid memory
    maze mz;
    mz.grid = (char *)malloc(atoi(argv[2])*atoi(argv[3])*sizeof(char));

    //initialize stack
    item *head = NULL;
    for (int i=0;i<10;i++){
        push(&head,i);
        printf("%d",pop(&head));
    }

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