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
#define FUNCTION_SUCCESS -1

// FIXME - add descritions at head of all functions

typedef struct __MazeInfo
{
    int width, height, playerPos[2];
    // 2D maze represented by 1D array means less fragmentation and easier to free
    // position [i][j] in a 2D array can be accessed by [(i*width)+j]
    char *grid;

} maze;

/**
 * @brief Find number of characters in a line of the file (not including '\n' or '\r')
 * @param buffer string containing one line in the file
 * @return int - number of characters
 */

int getStringLen(char *buffer)
{
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

/**
 * @brief Print error message, close the file, and return error code
 * @param mazeFile FILE containing the maze
 * @return int - error code
 */

int invalidMaseError(FILE *mazeFile)
{
    printf("Error: Invalid maze\n");
    fclose(mazeFile);
    return EXIT_MAZE_ERROR;
}

/**
 * @brief Find the height(h) and width(w) of the maze file and assign to maze struct,
 * check h and w is within acceptable range, ensure all rows are same length
 * @param mazeFile FILE containing the maze
 * @param mz pointer to maze struct
 * @return int - error code 
 */

int firstPass(FILE *mazeFile, maze *mz)
{
    int rows = 1, cols, bufferSize = 256;
    char buffer[bufferSize];

    fgets(buffer, bufferSize, mazeFile);

    // Find first line length and check it is within bounds
    int len = getStringLen(buffer);
    if (len < MIN_WIDTH || len > MAX_WIDTH)
    {
        return invalidMaseError(mazeFile);
    }
    cols = len;

    // Find height
    while (fgets(buffer, bufferSize, mazeFile))
    {
        rows++;
        // Check all lines are same length
        len = getStringLen(buffer);

        if (len != cols)
        {
            return invalidMaseError(mazeFile);
        }
    }
    // Check number of lines is within maze height bounds
    if (rows < MIN_HEIGHT || rows > MAX_HEIGHT)
    {
        return invalidMaseError(mazeFile);
    }
    // Fill height and width into maze struct
    mz->width = cols;
    mz->height = rows;
    return FUNCTION_SUCCESS;
}

/**
 * @brief dynamically allocate memory based on height and width found in first pass
 * @param mz pointer to maze struct
 * @return int - error code
 */

int allocateMemory(maze *mz)
{
    // Allocate memory for rows and check allocation was successful
    mz->grid = (char *)malloc(mz->height * mz->width * sizeof(char));
    if (mz->grid == NULL)
    {
        printf("Memory allocation failed\n");
        return 100;
    }
    return FUNCTION_SUCCESS;
}

/**
 * @brief assigns characters to the maze grid array, checks each character is valid, checks there is only one S and E
 * @param mazeFile FILE containing the maze
 * @param mz pointer to maze struct
 * @return int - error code
 */

int secondPass(FILE *mazeFile, maze *mz)
{
    // Set pointer to start of file for second pass
    fseek(mazeFile, 0, SEEK_SET);

    
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
            // Set sFound and eFound to true if they are found so we know if there are two or more instances
            if (symbol == 'S' && sFound == 0)
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
            // assign char to position in grid once validated
            mz->grid[(i * mz->width) + j] = symbol;
        }
    }
    // Check if no Start or End is found
    if (sFound == 0 || eFound == 0)
    {
        printf("Error: Invalid maze\n");
        return EXIT_MAZE_ERROR;
    }

    return FUNCTION_SUCCESS;
}

/**
 * @brief reads from file into maze grid
 * @param fileName string containing the name of the file
 * @param mz pointer to maze struct
 * @return int - error code
 */

int readFileIntoStruct(const char *fileName, maze *mz)
{
    // Open file (error if cant find or open)
    FILE *mazeFile = fopen(fileName, "r");

    if (!mazeFile)
    {
        printf("Error: Could not open file\n");
        return EXIT_FILE_ERROR;
    }

    // Reading file with two pass method to find size, alocate memory, and read-in as files are not very big 
    int returnCode = firstPass(mazeFile, mz);
    if (returnCode != -1)
    {
        return returnCode;
    }

    returnCode = allocateMemory(mz);
    if (returnCode != -1)
    {
        return returnCode;
    }

    returnCode = secondPass(mazeFile, mz);
    if (returnCode != -1)
    {
        // free the maze if invalid
        free(mz->grid);
        return returnCode;
    }

    fclose(mazeFile);

    // Successful file load - tell users and return code
    printf("Maze %s loaded successfully\n", fileName);
    return FUNCTION_SUCCESS;
}

/**
 * @brief Prints the maze
 * @param mz pointer to maze struct
 */

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

/**
 * @brief validates move and performs movement in specfied direction 
 * @param xMove horizontal movement direction (-1  = left, 1 = right)
 * @param mz pointer to maze struct
 * @return int - error code (FUNCTION_SUCCESS returned if there is no error but game isn't complete,
 * EXIT_SUCCESS returned if game is complete)
 */

int movePlayer(int xMove, int yMove, maze *mz)
{
    int xPos = mz->playerPos[0] + xMove;
    int yPos = mz->playerPos[1] + yMove;

    // Check move is valid
    // Check if move is off map (x axis)
    if (xPos < 0 || xPos > (mz->width - 1))
    {
        printf("You can't move there.\n");
        return FUNCTION_SUCCESS;
    }
    // Check if move is off map (y axis)
    if (yPos < 0 || yPos > (mz->height - 1))
    {
        printf("You can't move there.\n");
        return FUNCTION_SUCCESS;
    }
    // Check move isnt into a wall
    if (mz->grid[(yPos * mz->width) + xPos] == '#')
    {
        printf("You can't move there.\n");
        return FUNCTION_SUCCESS;
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
    return FUNCTION_SUCCESS;
}

/**
 * @brief calls game option functions based on user input
 * @param input char - users input
 * @param mz pointer to maze struct
 * @return int - error code (FUNCTION_SUCCESS returned if there is no error but game isn't complete,
 * EXIT_SUCCESS returned if game is complete)
 */

int inputSwitch(char input, maze *mz)
{
    switch (input)
    {
    case 'M':
        displayMaze(mz);
        return FUNCTION_SUCCESS;
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

/**
 * @brief checks users input is onlyone character and is a valid character
 * @param inputString - users input
 * @param input char - first character of users input
 * @return int - 1 if not valid, 0 if valid
 */

int checkMoveIsValid(char *inputString, char input){
    if (inputString[1] != '\0')
    {
        return 1;
    }
    if (input != 'W' && input != 'A' && input != 'S' && input != 'D' && input != 'M')
    {
        return 1;
    }
    return 0;
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
    if (returnCode != -1)
    {
        return returnCode;
    }

    // Run game
    while (1)
    {
        // Get input
        char inputString[2];

        printf("Enter input (W,A,S,D,M)\n");
        scanf("%2s", inputString);

        // check move/option char is valid
        char input = toupper(inputString[0]);
        if (checkMoveIsValid(inputString,input)){
            printf("Error: Invalid move option\n");
            continue;
        }

        // switch for each input case
        returnCode = inputSwitch(input, &maze);

        // End game if user has won
        if (returnCode == 0)
        {
            free(maze.grid);
            return EXIT_SUCCESS;
        }
        // Return return code if error has occered
        if (returnCode != -1)
        {
            return returnCode;
        }
    }
}