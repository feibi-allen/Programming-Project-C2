/**
 * @file mazeGen.c
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

typedef struct __Node
{
    char symbol;
    int position;
    int exploredEdges;
    int pathEdges;
} node;

typedef struct __Maze
{
    int width, height, currentPos;
    node *grid;
} maze;

typedef struct __item
{
    node nodeItem;
    struct __item *previous;
} item;

// modify pointer to head 
void push(item **head, maze *maze)
{
    item *tmp = (item*)malloc(sizeof(item));
    //printf("Current Pos: %d\nCreating item %d\n",maze->currentPos, maze->grid[maze->currentPos].position);
    tmp->nodeItem=maze->grid[maze->currentPos];
    tmp->previous= *head;
    if(*head != NULL) {
       //printf("Previous node item %d\n", tmp->previous->node); 
    }
    
    (*head) = tmp;
}

int pop(item **head){
    if ((*head)->previous == NULL){
        //printf("last node reached\n");
        return FALSE;
    }
    //printf("Free item %d\n", (*head)->node);
    item *tmp = *head;
    *head = (*head)->previous;
    free(tmp);
    //printf("Head item %d\n", (*head)->node);
    return TRUE;
}

void mazeStructSetup(maze *maze, int height, int width){
    maze->grid = (node *)malloc(height*width*sizeof(node));
    for (int i = 0; i<height*width; i++){
        node cell;
        cell.exploredEdges=0;
        cell.pathEdges=0;
        cell.position = i;
        maze->grid[i] = cell;
    }
    maze->height = height;
    maze->width = width;
}

int pickStartnode(maze *maze){
    srand(time(NULL));
    int col = rand()%((maze->width+1)/2)*2;
    int row = rand()%((maze->height+1)/2)*2;
    //printf("row:%d, col:%d\n", row, col);
    maze->currentPos = (row*maze->width)+col;
    node startNode;
    startNode.exploredEdges = 0;
    startNode.pathEdges = 0;
    startNode.symbol = '-';
    startNode.position = maze->currentPos;
    maze->grid[maze->currentPos] = startNode;
}

int validDirection(maze *maze, int row, int col){
    //printf("row:%d, maxrow;%d, col:%d, maxcol:%d\n", row,maze->height-1, col,maze->width-1);
    if (row<0||row>=maze->height||col<0||col>=maze->width){
        return FALSE;
    }
    int position = (row*maze->width)+col;
    // check if already visited
    if (maze->grid[position].symbol =='-'){
        return FALSE;
    }
    return TRUE;
}

void moveToNextnode(maze *maze,int nextPos, int bridge){
    //printf("moving to %d:\n", nextnode);
    maze->grid[nextPos].symbol = '-';
    maze->grid[bridge].symbol = '-';
    maze->currentPos = nextPos;
}

int pickDirection(maze *maze){
    if (maze->grid[maze->currentPos].exploredEdges == 4){
        return FALSE;
    }
    int directions[] = {0,0,0,0};
    int row = maze->currentPos/maze->width;
    int col = maze->currentPos%maze->width;
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
                maze->grid[maze->currentPos].pathEdges++;
                moveToNextnode(maze,maze->currentPos-(2*maze->width),maze->currentPos-maze->width);
                return TRUE;
            }
            break;
        case 1:
            //east
            //printf("east\n");
            if (validDirection(maze,row,col+2)== TRUE){
                maze->grid[maze->currentPos].pathEdges++;
                moveToNextnode(maze,maze->currentPos+2,maze->currentPos+1);
                return TRUE;
            }
            break;
        case 2:
            //south
            //printf("south\n");
            if (validDirection(maze,row+2,col)== TRUE){
                maze->grid[maze->currentPos].pathEdges++;
                moveToNextnode(maze,maze->currentPos+(2*maze->width),maze->currentPos+maze->width);
                return TRUE;
            }
            break;
        case 3:
            //west
            //printf("west\n");
            if (validDirection(maze,row,col-2)== TRUE){
                maze->grid[maze->currentPos].pathEdges++;
                moveToNextnode(maze,maze->currentPos-2,maze->currentPos-1);
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
            maze->grid[maze->currentPos].exploredEdges = 4;
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
            printf("%c", mz->grid[(i * mz->width) + j].symbol);
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
            if (mz->grid[(i*mz->width)+j].symbol != '-'||mz->grid[(i*mz->width)+j].symbol != 'S'){
                mz->grid[(i*mz->width)+j].symbol = '#';
            }
            
        }
    }
}

void fillLastRow(maze *maze){
    int count = 0;
    while (count<maze->width){
        //printf("count:%d\n",count);
        int col = rand() % maze->width;
        int pos = ((maze->height - 1)*maze->width)+col;
        //printf("maze->grid[%d] = %c\n",node, maze->grid[node]);
        if (maze->grid[pos].symbol != '-' && maze->grid[pos].symbol != '#'){
            // decide if wall or path
            char options[2] = {'#','-'};
            char symbol = options[rand()%2];
            //printf("mazegrid[%d] != '-''#'\n",node);
            // check if it can connect to a path
            if (maze->grid[pos-maze->width].symbol == '-'){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].pathEdges++;
            }else if (col>0 && maze->grid[pos-1].symbol == '-'){
                maze->grid[pos].symbol = symbol;
            }else if (col<maze->width-1 && maze->grid[pos-1].symbol == '-'){
                maze->grid[pos].symbol = symbol;
            }else{
                maze->grid[pos].symbol = '#';
            }
            count++;
        }
    }
}

void fillLastCol(maze *maze){
    int count = 0, height = maze->height;
    if (maze->height%2 ==0){
        height--;
    }
    while (count < height){
        int row = rand() % maze->height;
        int node = (row*maze->width)+maze->width-1;
        if (maze->grid[node].symbol != '-' && maze->grid[node].symbol != '#'){
            // decide if wall or path
            char options[2] = {'#','-'};
            char symbol = options[rand()%2];
            //printf("mazegrid[%d] != '-''#'\n",node);
            // check if it can connect to a path
            if (maze->grid[node-1].symbol == '-'){
                maze->grid[node].symbol = symbol;
            }else if (row>0 && maze->grid[node-(maze->width)].symbol == '-'){
                maze->grid[node].symbol = symbol;
            }else if (row<height-1 && maze->grid[node+(maze->width)].symbol == '-'){
                maze->grid[node].symbol = symbol;
            }else{
                maze->grid[node].symbol = '#';
            }
            count++;
        }
    }
}

void addStart(maze *maze){
    for (int i = 0;i<maze->height*maze->width;i++){
        printf("path edges:%d\n",maze->grid[i].pathEdges);
        if (maze->grid[i].pathEdges == 1){
            printf("adding start to %d\n",i);
            maze->grid[i].symbol == 'S';
            printf("maze->grid[i].symbol = %c\n",maze->grid[i].symbol);
        }
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

    //pick start node
    pickStartnode(&maze);
    
    push(&head,&maze);
    //printf("start node: %d\n",maze.currentPos);
    //int stack_cntr = 1;

    while (1) {
        // int input;
        // printf("input 1 for step:");
        // scanf("%d", &input);
        if (pickDirection(&maze)) {
            //printf("popping %d stack size %d\n",head->node, stack_cntr);
            //stack_cntr--;
            
            if (pop(&head) == FALSE){
                printf("end of stacl\n");
                if (maze.height%2 == 0){
                    fillLastRow(&maze);
                }
                if (maze.width%2 == 0){
                    fillLastCol(&maze);
                }
                fillWalls(&maze);
                displayMaze(&maze);
                return 0;
            }
            //printf("current position before pop: %d\n", maze.currentPos);
            maze.currentPos = head->nodeItem.position;
            //printf("current position after pop: %d\n", maze.currentPos);
        }else{
            //printf("pushing %d stack size %d\n",maze.currentPos, stack_cntr);
            push(&head,&maze);
            //stack_cntr++;
        }
    }
}