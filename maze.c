#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MAX_HEIGHT 100

typedef struct __MazeInfo
{
    int width, height, playerPos[2], start[2], end[2];
    char **grid;

} maze;

int checkMaze(maze *mz)
{
    // check each row for S and E and invalid characters

    // if two S or E or invalid characters found throw error

    // set player pos to S

}

void firstPass(const char *mazeFile, maze *mz){
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
        return 2;
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
            return 2;
        }
    }

    // Check number of lines is within maze height bounds
    if (rows < MIN_HEIGHT || rows > MAX_HEIGHT){
        printf("Error: Invalid maze");
            fclose(mazeFile);
            return 2;
    }

    // Fill height and width into maze struct
    mz->width = cols;
    mz->height = rows;
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

void readFileIntoStruct(const char *fileName, maze *mz)
{
    // Open file (error if cant find or open)
    FILE *mazeFile = fopen(fileName, "r");

    if (!mazeFile) {
        printf("Error: Could not open file\n");
        return 2;
    }

    // First Pass - will find height and width and check format of file
    firstPass(mazeFile, mz);

    // Allocate Memory bases on first pass
    allocateMemory(mz);

    // Second Pass -  will read file into maze struct
    secondPass(mazeFile, mz);

    // Close file
    fclose(mazeFile);

}

int freeMazeMemory(maze *mz)
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

int movePlayer(char move)
{
    // switch case for each move
        // calculate new player pos based on the move

    // check move is valid 
        // check coord is on map
        // check pos is not a wall

    // check if player is about to move to end coord
        // if they are then print congrats and exit

    // update player pos


}

int main(int argc, char *argv[])
{   
    // check arguments

    if (argc != 2) {
        printf("Usage: ./maze <filename>\n");
        return 1;
    }

    // create maze instance
    
    maze maze;

    // readFileIntoStruct

    readFileIntoStruct(argv[1],&maze);

    // loop until game exits

        // ask player for move/option

        // check move/option char is valid

        // if M display maze

        // if move char is valid movePlayer

    // free struct when game ends
    freeMazeMemory(&maze);
}