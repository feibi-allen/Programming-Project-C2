#include <stdio.h>
#include <stdlib.h>

#define MIN_WIDTH 5
#define MAX_WIDTH 100
#define MIN_HEIGHT 5
#define MIN_HEIGHT 100

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

void firstPass(const char *fileName, maze *mz){
    int rows = 1, cols, bufferSize = 256;
    char buffer[bufferSize];

    fgets(buffer, bufferSize, fileName);

    // Check maze width is within bounds
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
            len--;
        }
    // Check first line is within maze width bounds
    if (len < MIN_WIDTH || len > MAX_WIDTH){
        printf("Error: Invalid maze");
        fclose(fileName);
        return 2;
    }
    cols = len;

}

void readFileIntoStruct(const char *fileName, maze *mz)
{
    // open file (error if cant find or open)

     FILE *mazeFile = fopen(fileName, "r");

    if (!mazeFile) {
        printf("Error: Could not open file\n");
        return 2;
    }

    // Initialize variables

    int rows = 0;
    int matrixSize = 6;
    int bufferSize = 6;
    char buffer[bufferSize];

    // First Pass

    char **matrix = (char **)malloc(sizeof(char *) * matrixSize);
    if (matrix == NULL){
        perror("memory allocation failed\n");
        fclose(mazeFile);
        return 100;
    }

    while (fgets(buffer, bufferSize, mazeFile)){

        }

}

int freeMaze(maze *mz)
{
    // check not null

    // free array in each row

    // free the rows

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

    readFileIntoStruct(argv[1],&maze)

    // loop until game exits

        // ask player for move/option

        // check move/option char is valid

        // if M display maze

        // if move char is valid movePlayer

    // free struct when game ends
}