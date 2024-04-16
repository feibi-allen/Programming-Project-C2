/**
 * @file maze.c
 * @author Fei-bi Allen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MAX_HEIGHT 100

#define TRUE 0
#define FALSE 1
#define EXIT_ARG_ERROR 1

typedef struct __Maze
{
    int width, height, currentCell;
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
    //printf("Creating item %d\n", cell);
    tmp->cell=cell;
    tmp->previous= *head;
    if(*head != NULL) {
       //printf("Previous cell item %d\n", tmp->previous->cell); 
    }
    
    (*head) = tmp;
}

int pop(item **head){
    if ((*head)->previous == NULL){
        //printf("last cell reached\n");
        return FALSE;
    }
    //printf("Free item %d\n", (*head)->cell);
    item *tmp = *head;
    *head = (*head)->previous;
    free(tmp);
    //printf("Head item %d\n", (*head)->cell);
    return TRUE;
}

void mazeStructSetup(maze *maze, int height, int width){
    maze->grid = (char *)malloc(height*width*sizeof(char));
    maze->height = height;
    maze->width = width;
}

int pickStartCell(maze *maze){
    srand(time(NULL));
    int col = rand()%((maze->width+1)/2)*2;
    int row = rand()%((maze->height+1)/2)*2;
    //printf("row:%d, col:%d\n", row, col);
    maze->currentCell = (row*maze->width)+col;
    maze->grid[maze->currentCell] = '-';
}

int validDirection(maze *maze, int row, int col){
    //printf("row:%d, maxrow;%d, col:%d, maxcol:%d\n", row,maze->height-1, col,maze->width-1);
    if (row<0||row>=maze->height||col<0||col>=maze->width){
        return FALSE;
    }
    int cell = (row*maze->width)+col;
    if (maze->grid[cell]=='-'){
        return FALSE;
    }
    return TRUE;
}

void moveToNextCell(maze *maze,int nextCell, int bridge){
    //printf("moving to %d:\n", nextCell);
    maze->grid[bridge] = '-';
    maze->grid[nextCell] = '-';
    maze->currentCell = nextCell;
}

int pickDirection(maze *maze){
    int directions[] = {0,0,0,0};
    int row = maze->currentCell/maze->width;
    int col = maze->currentCell%maze->width;
    int nextCell;
    while(1){
        int direction = rand()%4;
        if (directions[direction] == 1) {
            continue;
        }
        directions[direction] = 1;
        switch (direction)
        {
        case 0:
            //north
            //printf("north\n");
            if (validDirection(maze,row-2,col) == TRUE){
                moveToNextCell(maze,maze->currentCell-(2*maze->width),maze->currentCell-maze->width);
                return TRUE;
            }
            break;
        case 1:
            //east
            //printf("east\n");
            if (validDirection(maze,row,col+2)== TRUE){
                moveToNextCell(maze,maze->currentCell+2,maze->currentCell+1);
                return TRUE;
            }
            break;
        case 2:
            //south
            //printf("south\n");
            if (validDirection(maze,row+2,col)== TRUE){
                moveToNextCell(maze,maze->currentCell+(2*maze->width),maze->currentCell+maze->width);
                return TRUE;
            }
            break;
        case 3:
            //west
            //printf("west\n");
            if (validDirection(maze,row,col-2)== TRUE){
                moveToNextCell(maze,maze->currentCell-2,maze->currentCell-1);
                return TRUE;
            }
            break;
        }
        int allDirectionsChecked = TRUE;
        for (int i = 0; i < 4; i++) {
            //printf("direction[%d]",i);
            if (directions[i] !=1){
                allDirectionsChecked = FALSE;
            }
        }
        //printf("{%d,%d,%d,%d}\n", directions[0], directions[1], directions[2], directions[3]);
        if (allDirectionsChecked == TRUE) {
           return FALSE;  
        }
          
    }
}

void displayMaze(maze *mz)
{
    printf("\n");
    for (int i = 0; i < mz->height; i++)
    {
        for (int j = 0; j < mz->width; j++)
        {
            printf("%c", mz->grid[(i * mz->width) + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void fillWalls(maze *mz) {
    for (int i = 0; i < mz->height; i++)
    {
        for (int j = 0; j < mz->width; j++)
        {
            mz->grid[(i*mz->width)+j] = '#';
        }
    }
}

int main(int argc, char *argv[]) {

    // error check args
    if (argc !=4){
        //printf("Usage: ./maze <filename>\n");
        return 1;
    }

    int height = atoi(argv[3]);
    int width = atoi(argv[2]);

    if (height<MIN_HEIGHT||height>MAX_HEIGHT||width<MIN_WIDTH||width>MAX_WIDTH){
        printf("Usage: ./maze <filename>\n");
        return 1;
    }

    // asign maze struct grid memory
    maze maze;
    mazeStructSetup(&maze,height,width);
    fillWalls(&maze);

    //initialize stack
    item *head = NULL;

    //pick start cell
    pickStartCell(&maze);
    
    push(&head,maze.currentCell);
    //printf("start cell: %d\n",maze.currentCell);
    //int stack_cntr = 1;

    while (1) {
        // int input;
        // printf("input 1 for step:");
        // scanf("%d", &input);
        if (pickDirection(&maze)) {
            //printf("popping %d stack size %d\n",head->cell, stack_cntr);
            //stack_cntr--;
            
            if (pop(&head) == FALSE){
                displayMaze(&maze);
                return 0;
            }
            maze.currentCell = head->cell;
        }else{
            //printf("pushing %d stack size %d\n",maze.currentCell, stack_cntr);
            push(&head,maze.currentCell);
            //stack_cntr++;
        }
        //displayMaze(&maze);
    }
}