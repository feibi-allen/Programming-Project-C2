#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MAX_HEIGHT 100

typedef struct __MazeInfo
{
    int width, height, playerPos[2], start[2], end[2];
    char **grid;

} maze;

int firstPass(const char *mazeFile, maze *mz){
    int rows = 1, cols, bufferSize = 256;
    char buffer[bufferSize];

    fgets(buffer, bufferSize, mazeFile);

    // Find first line length
// FIXME - check for invalid characters
// FIXME - following code is repeated and could be made into a function
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
        len--;
    }
    // Check first line is within maze width bounds
    if (len < MIN_WIDTH || len > MAX_WIDTH){
        printf("Error: Invalid maze");
        fclose(mazeFile);
        return 3;
    }
    cols = len;

    // Find height
    while (fgets(buffer, bufferSize, mazeFile)){
        rows++;
        // check all lines are same length
        int len = strlen(buffer);
        if (buffer[len - 1] == '\n') {
            len--;
        }
// FIXME - could make a function called "invalid maze error" to avoid repeated code
        if (len != cols){
            printf("Error: Invalid maze");
            fclose(mazeFile);
            return 3;
        }
    }

    // Check number of lines is within maze height bounds
    if (rows < MIN_HEIGHT || rows > MAX_HEIGHT){
        printf("Error: Invalid maze");
            fclose(mazeFile);
            return 3;
    }

    // Fill height and width into maze struct
    mz->width = cols;
    mz->height = rows;
    return 0;
}

void allocateMemory(maze *mz){
    // Allocate memory for rows and check allocation was successful
    mz->grid = (char **)malloc(mz->height * sizeof(char *));
    if (mz->grid == NULL){
        printf("Memory allocation failed\n");
        return 100;
    }
    // Allocate memory for columns and check allocation was successful
    for (int i = 0; i < mz->height; i ++){
        mz->grid[i] = (char *)malloc(mz->width * sizeof(char));
        if (mz->grid[i] == NULL){
        printf("Memory allocation failed\n");
        return 100;
        }
    }
}

void secondPass(const char *mazeFile, maze *mz){
    // Set pointer to start of file
    fseek(mazeFile, 0, SEEK_SET);

    // Read in lines
    int bufferSize = 256;
    char buffer[bufferSize];
    for (int i = 0; i < mz->height; i++){
        fgets(buffer, bufferSize, mazeFile);
        strncpy(mz->grid[i],buffer,mz->width);
    }
}

int readFileIntoStruct(const char *fileName, maze *mz)
{
    // Open file (error if cant find or open)
    FILE *mazeFile = fopen(fileName, "r");

    if (!mazeFile) {
        printf("Error: Could not open file\n");
        return 2;
    }

    // First Pass - will find height and width and check format of file
    int returnCode = firstPass(mazeFile, mz);
    if (returnCode != 0){
        return returnCode;
    }

    // Allocate Memory bases on first pass
    allocateMemory(mz);

    // Second Pass -  will read file into maze struct
    secondPass(mazeFile, mz);

    // Close file
    fclose(mazeFile);

    // Tell user file successfully loaded
    printf("Maze %s loaded successfully\n");
    return 0;
}

void freeMazeMemory(maze *mz)
{
    // Free array in each row
    for (int i; i<mz->height; i++){
        free(mz->grid[i]);
    }
    // free the rows
    free(mz->grid);
}

int displayMaze(maze *mz)
{
    // for each row
        // for each column
            // if its playerPos then print X else print char from struct
        // newline
}

int movePlayer(int xMove, int yMove, maze *mz)
{
    int xPos = mz->playerPos[0]+xMove;
    int yPos = mz->playerPos[1]+yMove;

    // Check move is valid
    // Check if move is off map (x axis)
    if (xPos < 0 || xPos > (mz->width - 1)){
        printf("You can't move there.\n");
        return -1;
    }
    // Check if move is off map (y axis)
    if (yPos < 0 || yPos > (mz->height - 1)){
        printf("You can't move there.\n");
        return -1;
    }
    // Check move isnt into a wall
    if (mz->grid[xPos][yPos] == '#'){
        printf("You can't move there.\n");
        return -1;
    }
    
    // Check if move ends the game
    if (mz->grid[xPos][yPos] == 'E'){
        printf("Congratulations! You finished the maze.\n");
        return 0;
    }

    // Move the player
    mz->playerPos[0] += xMove;
    mz->playerPos[1] += yMove;
    return -1;
}

int inputSwitch(char input, maze *mz){
    switch (input)
    {
    case 'M':
        displayMaze(mz);
        return -1;
        break;
    case 'W':
        return movePlayer(1,0,mz);
        break;
    case 'A':
        return movePlayer(0,-1,mz);
        break;
    case 'S':
        return movePlayer(-1,0,mz);
        break;
    case 'D':
        return movePlayer(0,1,mz);
        break;
    default:
        return 100;
        break;
    }
}

int main(int argc, char *argv[])
{   
    // Check arguments
    if (argc != 2) {
        printf("Usage: ./maze <filename>\n");
        return 1;
    }

    // Create maze instance
    maze maze;

    // ReadFileIntoStruct - returns appropreate code if error has occered
    int returnCode = readFileIntoStruct(argv[1],&maze);
    if (returnCode != 0){
        return returnCode;
    }

    // Run game
    while(1){
        char inputString[3];
        // ask player for move/option
        printf("Enter input (W,A,S,D,M)\n");
        scanf("%2s",inputString);
        char input = toupper(inputString[0]);

        // check move/option char is valid
        if (inputString[1] != "\n"){
            printf("Error: Invalid move option\n");
        }
        else if (input != "W" && input != "A" && input != "S" && input != "D" && input != "M"){
                printf("Error: Invalid move option\n");
        }
        // switch for each input case
        else {
            returnCode = inputSwitch(input, &maze);
            // End game with win
            if (returnCode == 0){
                freeMazeMemory(&maze);
                return 0;
            }
            // Return return code if error has occered
            else if (returnCode != -1){
                return returnCode;
            }
            // Else if return code was -1, continue game
        }
    }
}