typedef struct __MazeInfo
{
    int width, height, playerPos[2], start[2], end[2];
    char **grid;

} maze;

int checkMaze(maze **mz)
{
    // check each row for S and E and invalid characters

    // if two S or E or invalid characters found throw error

    // set player pos to S

}


int readFileIntoStruct(const char *fileName, maze **mz)
{
    // open file (error if cant find or open)

    // for each line in file
        // fgets()
        // check length of line (throw error if inconsistent)
        // allocate memory for row (not including \n)
        // initialize each [row][col] to char in file
    // realloc for every row

    // initialize width and height of maze 

    // check the maze for invalid data and player pos

    // close file

}

int freeMaze(maze **mz)
{
    // check not null

    // free array in each row

    // free the rows

}

int displayMaze(maze **mz)
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

    // create maze instance
    
    // readFileIntoStruct(argv[1],maze)

    // loop until game exits

        // ask player for move/option

        // check move/option char is valid

        // if M display maze

        // if move char is valid movePlayer

    // free struct when game ends
}