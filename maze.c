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
#define EXIT_FILE_ERROR 2
#define EXIT_MAZE_ERROR 3

// FIXME - add descritions at head of all functions

typedef struct __MazeInfo
{
    int width, height, playerPos[2];
    // 1D array means less fragmentation and easier to free
    char *grid;

} maze;

int getStringLen(char *buffer){
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n')
    {
        len--;
    }
    if (buffer[len - 1] == '\r')
    {
        len--;
    }
    return len;
}

void invalidMaseError(FILE *mazeFile){
    printf("Error: Invalid maze\n");
    fclose(mazeFile);
    return EXIT_MAZE_ERROR;
}

int firstPass(FILE *mazeFile, maze *mz)
{
    int rows = 1, cols, bufferSize = 256;
    char buffer[bufferSize];

    fgets(buffer, bufferSize, mazeFile);
    // Find first line length
    int len = getStringLen(buffer);

    // Check first line is within maze width bounds
    if (len < MIN_WIDTH || len > MAX_WIDTH)
    {
        invalidMaseError(mazeFile);
    }
    cols = len;

    // Find height
    while (fgets(buffer, bufferSize, mazeFile))
    {
        rows++;
        // check all lines are same length
        len = getStringLen(buffer);

        if (len != cols)
        {
            invalidMaseError(mazeFile);
        }
    }
    // Check number of lines is within maze height bounds
    if (rows < MIN_HEIGHT || rows > MAX_HEIGHT)
    {
        invalidMaseError(mazeFile);
    }
    // Fill height and width into maze struct
    mz->width = cols;
    mz->height = rows;
    return EXIT_SUCCESS;
}

int allocateMemory(maze *mz)
{
    // Allocate memory for rows and check allocation was successful
    mz->grid = (char *)malloc(mz->height * mz->width * sizeof(char));
    if (mz->grid == NULL)
    {
        printf("Memory allocation failed\n");
        return 100;
    }
    return EXIT_SUCCESS;
}

int secondPass(FILE *mazeFile, maze *mz)
{
    // Set pointer to start of file
    fseek(mazeFile, 0, SEEK_SET);

    // Read in lines
    int bufferSize = 256;
    char buffer[bufferSize];
    int sFound = 0, eFound = 0;

    for (int i = 0; i < mz->height; i++)
    {
        fgets(buffer, bufferSize, mazeFile);
        for (int j = 0; j < mz->width; j++)
        {
            char symbol = toupper(buffer[j]);
            // Check if char is valid and return error code if not
            if (symbol != 'S' && symbol != 'E' && symbol != '#' && symbol != ' ')
            {
                printf("Error: Invalid maze\n");
                return EXIT_MAZE_ERROR;
            }
            // Set sFound and eFound to true if found
            else if (symbol == 'S' && sFound == 0)
            {
                sFound = 1;
                mz->playerPos[0] = j;
                mz->playerPos[1] = i;
            }
            else if (symbol == 'E' && eFound == 0)
            {
                eFound = 1;
            }
            // Check if S or E is already found
            else if ((symbol == 'S' && sFound == 1) || (symbol == 'E' && eFound == 1))
            {
                printf("Error: Invalid maze\n");
                return EXIT_MAZE_ERROR;
            }
            mz->grid[(i * mz->width) + j] = symbol;
        }
    }
    if (sFound == 0 || eFound == 0)
    {
        printf("no S or E\n");
        printf("Error: Invalid maze\n");
        return EXIT_MAZE_ERROR;
    }
    return EXIT_SUCCESS;
}

int readFileIntoStruct(const char *fileName, maze *mz)
{
    // Open file (error if cant find or open)
    FILE *mazeFile = fopen(fileName, "r");

    if (!mazeFile)
    {
        printf("Error: Could not open file\n");
        return EXIT_FILE_ERROR;
    }

    // First Pass - will find height and width and check they are winith bounds
    int returnCode = firstPass(mazeFile, mz);
    if (returnCode != 0)
    {
        return returnCode;
    }

    // Allocate Memory bases on first pass
    returnCode = allocateMemory(mz);
    if (returnCode != 0)
    {
        return returnCode;
    }

    // Second Pass -  will read file into maze struct and check contense of file
    returnCode = secondPass(mazeFile, mz);
    if (returnCode != 0)
    {
        free(mz->grid);
        return returnCode;
    }

    // Close file
    fclose(mazeFile);

    // Tell user file successfully loaded
    printf("Maze %s loaded successfully\n", fileName);
    return EXIT_SUCCESS;
}

void displayMaze(maze *mz)
{
    printf("\n");
    for (int i = 0; i < mz->height; i++)
    {
        for (int j = 0; j < mz->width; j++)
        {
            if (mz->playerPos[0] == j && mz->playerPos[1] == i)
            {
                printf("X");
            }
            else
            {
                printf("%c", mz->grid[(i * mz->width) + j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int movePlayer(int xMove, int yMove, maze *mz)
{
    int xPos = mz->playerPos[0] + xMove;
    int yPos = mz->playerPos[1] + yMove;

    // Check move is valid
    // Check if move is off map (x axis)
    if (xPos < 0 || xPos > (mz->width - 1))
    {
        printf("You can't move there.\n");
        return -1;
    }
    // Check if move is off map (y axis)
    if (yPos < 0 || yPos > (mz->height - 1))
    {
        printf("You can't move there.\n");
        return -1;
    }
    // Check move isnt into a wall
    if (mz->grid[(yPos * mz->width) + xPos] == '#')
    {
        printf("You can't move there.\n");
        return -1;
    }

    // Check if move ends the game
    if (mz->grid[(yPos * mz->width) + xPos] == 'E')
    {
        printf("Congratulations! You finished the maze.\n");
        return EXIT_SUCCESS;
    }

    // Move the player
    mz->playerPos[0] += xMove;
    mz->playerPos[1] += yMove;
    return -1;
}

int inputSwitch(char input, maze *mz)
{
    switch (input)
    {
    case 'M':
        displayMaze(mz);
        return -1;
        break;
    case 'W':
        return movePlayer(0, -1, mz);
        break;
    case 'A':
        return movePlayer(-1, 0, mz);
        break;
    case 'S':
        return movePlayer(0, 01, mz);
        break;
    case 'D':
        return movePlayer(1, 0, mz);
        break;
    default:
        return 100;
        break;
    }
}

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2)
    {
        printf("Usage: ./maze <filename>\n");
        return EXIT_ARG_ERROR;
    }

    // Create maze instance
    maze maze;

    // ReadFileIntoStruct - returns appropreate code if error has occered
    int returnCode = readFileIntoStruct(argv[1], &maze);
    if (returnCode != 0)
    {
        return returnCode;
    }

    // Run game
    while (1)
    {
        char inputString[2];
        // ask player for move/option
        printf("Enter input (W,A,S,D,M)\n");
        scanf("%2s", inputString);
        char input = toupper(inputString[0]);

        // check move/option char is valid
        if (inputString[1] != '\0')
        {
            printf("Error: Invalid move option\n");
        }
        else if (input != 'W' && input != 'A' && input != 'S' && input != 'D' && input != 'M')
        {
            printf("Error: Invalid move option\n");
        }
        // switch for each input case
        else
        {
            returnCode = inputSwitch(input, &maze);
            // End game with win
            if (returnCode == 0)
            {
                free(maze.grid);
                return EXIT_SUCCESS;
            }
            // Return return code if error has occered
            else if (returnCode != -1)
            {
                return returnCode;
            }
            // Else if return code was -1, continue game
        }
    }
}