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

#define EXIT_SUCCESS 0
#define EXIT_ARG_ERROR 1
#define EXIT_FILE_ERROR 2

typedef struct __Node
{
    char symbol;
    int visited;
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

/**
 * @brief Pushes a node to the stack by changing address in pointer
 * @param head pointer to the pointer to head
 * @param maze struct containing maze information
 */
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

/**
 * @brief Pops a node off the stack and frees pointers
 * @param head pointer to the pointer to head
 */
int pop(item **head){
    if ((*head)->previous == NULL){
        //printf("last node reached\n");
        return 1;
    }
    //printf("Free item %d\n", (*head)->node);
    item *tmp = *head;
    *head = (*head)->previous;
    free(tmp);
    //printf("Head item %d\n", (*head)->node);
    return 0;
}

/**
 * @brief Allocated momory and initializes nodes for every position in the maze
 * @param maze struct containing maze information
 * @param height user inputed height of maze
 * @param width  user inputed width of the maze
 */
void mazeStructSetup(maze *maze, int height, int width){
    maze->grid = (node *)malloc(height*width*sizeof(node));
    for (int i = 0; i<height*width; i++){
        node cell;
        cell.visited = 0;
        cell.exploredEdges=0;
        cell.pathEdges= 0;
        cell.position = i;
        maze->grid[i] = cell;
    }
    maze->height = height;
    maze->width = width;
}

/**
 * @brief Randomly picks a node in the maze to start generating from
 * @param maze struct containing maze information
 */
void pickStartnode(maze *maze){
    srand(time(NULL));
    int col = rand()%((maze->width+1)/2)*2;
    int row = rand()%((maze->height+1)/2)*2;
    //printf("row:%d, col:%d\n", row, col);
    maze->currentPos = (row*maze->width)+col;
    node startNode;
    startNode.visited = 1;
    startNode.symbol = '-';
    startNode.position = maze->currentPos;
    maze->grid[maze->currentPos] = startNode;
}

/**
 * @brief Checks if path can continue in the chosen direction
 * @param maze struct containing maze information
 * @param row row the path would continue to
 * @param col  column the path would continue to
 * @return 1 if direction is valid 0 if not
 */
int validDirection(maze *maze, int row, int col){
    //printf("row:%d, maxrow;%d, col:%d, maxcol:%d\n", row,maze->height-1, col,maze->width-1);
    if (row<0||row>=maze->height||col<0||col>=maze->width){
        return 0;
    }
    int position = (row*maze->width)+col;
    // check if already visited
    if (maze->grid[position].visited){
        return 0;
    }
    return 1;
}

/**
 * @brief Creates a path to the next node
 * @param maze struct containing maze information
 * @param nextPos position that the path will branch to
 * @param bridge  the position of the node inbetween the current node and the next node
 */
void moveToNextnode(maze *maze,int nextPos, int bridge){
    //printf("moving to %d:\n", nextnode);
    maze->grid[nextPos].symbol = '-';
    maze->grid[nextPos].visited = 1;
    maze->grid[bridge].symbol = '-';
    maze->grid[bridge].visited = 1;
    maze->currentPos = nextPos;
}

/**
 * @brief Randomly chooses a direction the path shoudl branch
 * @param maze struct containing maze information
 * @return 1 if all directions have been checked, else 0
 */
int pickDirection(maze *maze){
    if (maze->grid[maze->currentPos].exploredEdges == 4){
        return 1;
    }
    // directions[] lists the directions which were visited to ensure all are checked
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
            if (validDirection(maze,row-2,col)){
                moveToNextnode(maze,maze->currentPos-(2*maze->width),maze->currentPos-maze->width);
                return 0;
            }
            break;
        case 1:
            //east
            //printf("east\n");
            if (validDirection(maze,row,col+2)){
                moveToNextnode(maze,maze->currentPos+2,maze->currentPos+1);
                return 0;
            }
            break;
        case 2:
            //south
            //printf("south\n");
            if (validDirection(maze,row+2,col)){
                moveToNextnode(maze,maze->currentPos+(2*maze->width),maze->currentPos+maze->width);
                return 0;
            }
            break;
        case 3:
            //west
            //printf("west\n");
            if (validDirection(maze,row,col-2)){;
                moveToNextnode(maze,maze->currentPos-2,maze->currentPos-1);
                return 0;
            }
            break;
        }
        int allDirectionsChecked = 1;
        for (int i = 0; i < 4; i++) {
            //printf("direction[%d]",i);
            if (directions[i] !=1){
                allDirectionsChecked = 0;
            }
        }
        //printf("{%d,%d,%d,%d}\n", directions[0], directions[1], directions[2], directions[3]);
        if (allDirectionsChecked) {
            maze->grid[maze->currentPos].exploredEdges = 4;
           return 1;  
        }
          
    }
}

/**
 * @brief checks if there is a path above the given node
 * @param maze struct containing maze information
 * @param pos position being checked
 * @return 1 if there is a path, else 0
 */
int pathAbove(maze *maze, int pos){
    if (pos<maze->width){
        return 0;
    }
    if (maze->grid[pos-maze->width].symbol == '-'){
        return 1;
    }
    return 0;
}

int pathLeft(maze *maze, int pos){
    if (pos%maze->width==0){
        return 0;
    }
    if (maze->grid[pos-1].symbol == '-'){
        return 1;
    }
    return 0;
}

int pathRight(maze *maze, int pos){
    if (pos%maze->width==maze->width-1){
        return 0;
    }
    if (maze->grid[pos+1].symbol == '-'){
        return 1;
    }
    return 0;
}

int pathDown(maze *maze, int pos){
    if (pos != 0 && pos/maze->width==maze->height-1){
        return 0;
    }
    if (maze->grid[pos+maze->width].symbol == '-'){
        return 1;
    }
    return 0;
}

/**
 * @brief places paths in the last row of a maze with an even number of rows
 * @param maze struct containing maze information
 */
void fillLastRow(maze *maze){
    int count = 0;
    while (count<maze->width){
        //printf("count:%d\n",count);
        int pos = ((maze->height - 1)*maze->width)+rand() % maze->width;
        //printf("maze->grid[%d] = %c\n",node, maze->grid[node]);
        if (!maze->grid[pos].visited){
            // decide if wall or path
            char options[2] = {'#','-'};
            char symbol = options[rand()%2];
            //printf("mazegrid[%d] != '-''#'\n",node);
            // check if it can connect to a path
            if (pathAbove(maze,pos)){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].visited = 1;
            }else if (pathLeft(maze,pos)){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].visited = 1;
            }else if (pathRight(maze,pos)){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].visited = 1;
            }
            count++;
        }
    }
}

/**
 * @brief places paths in the last column of a maze with an even number of colums
 * @param maze struct containing maze information
 */
void fillLastCol(maze *maze){
    int count = 0, height = maze->height;
    if (maze->height%2 ==0){
        height--;
    }
    while (count < height){
        int row = rand() % maze->height;
        int pos = (row*maze->width)+maze->width-1;
        if (!maze->grid[pos].visited){
            // decide if wall or path
            char options[2] = {'#','-'};
            char symbol = options[rand()%2];
            //printf("mazegrid[%d] != '-''#'\n",node);
            // check if it can connect to a path
            if (pathLeft(maze,pos)){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].visited = 1;
            }else if (pathAbove(maze,pos)){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].visited = 1;
            }else if (pathDown(maze,pos)){
                maze->grid[pos].symbol = symbol;
                maze->grid[pos].visited = 1;
            }
            count++;
        }
    }
}

/**
 * @brief for each path node, counts the number of connected path nodes and updates that node instance
 * @param maze struct containing maze information
 */
void countEdges(maze *maze){
    for (int i = 0 ; i<maze->height*maze->width; i++){
        //printf("position:%d visitied:%d\n",i,maze->grid[i].visited);
        if (maze->grid[i].symbol != '-'){
            continue;
        }
        if (pathAbove(maze,i)){
            maze->grid[i].pathEdges++;
        }
        if (pathDown(maze,i)){
            maze->grid[i].pathEdges++;
        }
        if (pathLeft(maze,i)){
            maze->grid[i].pathEdges++;
        }
        if (pathRight(maze,i)){
            maze->grid[i].pathEdges++;
        }
    }
}

/**
 * @brief places the start of the maze in the first position it finds with the fewest connected paths
 * (starting from top left and workign towards bottom right)
 * @param maze struct containing maze information
 * @return exits the function once a start has been placed so there is only one instance
 */
void addStart(maze *maze){
    for (int i = 0;i<maze->height*maze->width;i++){
        //printf("position %d has path edges:%d\n",i,maze->grid[i].pathEdges);
        if (maze->grid[i].pathEdges == 1){
            //printf("adding start to %d\n",i);
            maze->grid[i].symbol = 'S';
            return;
        }
    }
    for (int i = 0;i<maze->height*maze->width;i++){
        if (maze->grid[i].pathEdges == 2){
            maze->grid[i].symbol = 'S';
            return;
        }
    }
    for (int i = 0;i<maze->height*maze->width;i++){
        if (maze->grid[i].pathEdges == 3){
            maze->grid[i].symbol = 'S';
            return;
        }
    }
}

/**
 * @brief places the end of the maze in the first position it finds with the fewest connected paths
 * (starting from the bottom right and workign towards top left)
 * @param maze struct containing maze information
 * @return exits the function once a end has been placed so there is only one instance
 */
void addEnd(maze *maze){
    for (int i = maze->height*maze->width-1;i>=0;i--){
        //printf("position %d has path edges:%d, visited:%d\n",i,maze->grid[i].pathEdges,maze->grid[i].visited);
        if (maze->grid[i].symbol == 'S'){
            continue;
        }
        if (maze->grid[i].pathEdges == 1){
            maze->grid[i].symbol = 'E';
            return;
        }
    }
    for (int i = 0;i<maze->height*maze->width;i++){
        if (maze->grid[i].symbol == 'S'){
            continue;
        }
        if (maze->grid[i].pathEdges == 2){
            maze->grid[i].symbol = 'E';
            return;
        }
    }
    for (int i = 0;i<maze->height*maze->width;i++){
        if (maze->grid[i].symbol == 'S'){
            continue;
        }
        if (maze->grid[i].pathEdges == 3){
            maze->grid[i].symbol = 'E';
            return;
        }
    }
}

/**
 * @brief places a wall in any positions which are not a path or start or end
 * @param maze struct containing maze information
 */
void fillWalls(maze *mz) {
    for (int i = 0; i < mz->height; i++)
    {
        for (int j = 0; j < mz->width; j++)
        {
            //printf("pos:%d visited:%d\n",(i*mz->width)+j,mz->grid[(i*mz->width)+j].visited);
            if (!mz->grid[(i*mz->width)+j].visited){
                mz->grid[(i*mz->width)+j].symbol = '#';
            }
            
        }
    }
}

/**
 * @brief displays the maze, used for development and debugging
 * @param maze struct containing maze information
 */
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

/**
 * @brief writes the maze into the given file
 * @param filename name of the file to write to given by user
 * @param maze struct containing maze information
 * @return 0 if file could not be opened, else 1
 */
int writeToFile(char *fileName, maze *maze){
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        return 0;
    }
    for (int i = 0; i<maze->height*maze->width;i++){
        fprintf(file,"%c",maze->grid[i].symbol);
        if (i%maze->width == maze->width-1){
            fprintf(file,"\n");
        }
    }
    fclose(file);
    return 1;
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
            
            if (pop(&head)){
                if (maze.height%2 == 0){
                    fillLastRow(&maze);
                }
                if (maze.width%2 == 0){
                    fillLastCol(&maze);
                }
                countEdges(&maze);
                addStart(&maze);
                addEnd(&maze);
                fillWalls(&maze);
                //displayMaze(&maze);
                if (!writeToFile(argv[1],&maze)){
                    return EXIT_FILE_ERROR;
                }
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