/**
 * @file maze.c
 * @author Fei-bi Allen
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void mazeStructSetup(maze *maze, int height, int width){
    maze->grid = (char *)malloc(height*width*sizeof(char));
    maze->height = height;
    maze->width = width;
}

int pickStartCell(maze *maze){
    srand(time(NULL));
    int row = rand()%((maze->width+1)/2)*2;
    int col = rand()%((maze->height+1)/2)*2;
    maze->currentCell = (row*maze->width)+col;
}

int validDirection(maze *maze, int row, int col){
    if (row<0||row>=maze->width||col<0||col>=maze->height){
        return FALSE;
    }
    int cell = (row*maze->width)+col;
    if (maze->grid[cell]==' '){
        return FALSE;
    }
    return TRUE;
}

void moveToNextCell(maze *maze,int nextCell, int bridge){
    printf("moving to %d:\n", nextCell);
    maze->grid[bridge] = ' ';
    maze->grid[nextCell] = ' ';
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
            if (validDirection(maze,row-2,col) == TRUE){
                moveToNextCell(maze,maze->currentCell-(2*maze->width),maze->currentCell-maze->width);
                return TRUE;
            }
            break;
        case 1:
            //east
            if (validDirection(maze,row,col+2)== TRUE){
                moveToNextCell(maze,maze->currentCell+2,maze->currentCell+1);
                return TRUE;
            }
            break;
        case 2:
            //south
            if (validDirection(maze,row+2,col)== TRUE){
                moveToNextCell(maze,maze->currentCell+(2*maze->width),maze->currentCell+maze->width);
                return TRUE;
            }
            break;
        case 3:
            //west
            if (validDirection(maze,row,col-2)== TRUE){
                moveToNextCell(maze,maze->currentCell-2,maze->currentCell-1);
                return TRUE;
            }
            break;
        }
        for (int i = 0; i < 4; i++) {
            if (directions[i] !=1){
                continue;
            }
        }
        return FALSE;   
    }
    


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
    maze maze;
    mazeStructSetup(&maze,height,width);

    //initialize stack
    item *head = NULL;

    //pick start cell
    pickStartCell(&maze);
    push(&head,maze.currentCell);

    //while (1){
        // pick direction
        // check if visited or out of rance (pick other direction)
        // check if all directions are checked (pop)
        // move to new cell and create path
        // repeat until stack is empty


    //}

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