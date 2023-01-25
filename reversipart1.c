// Alan Bu, March 19 2022
// Reversi Part 1:
// Final copy
// Notes for the lab:
// Row and column must be greater than zero and less than OR equal to "size"
// (0, 0) is not used. a-z starts from 1 - 26
// First white piece located at (n / 2), black piece to the right and under of that one.
// Another white piece south-east of the first one.
// Maybe print board (abdefgh) after initializing the board (UUUUWBUUU)
// #include "reversi.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
void initializeBoard(char board[][26], int dim)
{
    // Initialize the board
    // Make everything U, then add B and W
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            board[row][col] = 'U';
        }
    }
    int firstPiece = (dim / 2 - 1);
    int second = firstPiece + 1;
    // Initialize the centre pieces
    board[firstPiece][firstPiece] = 'W';
    board[firstPiece][second] = 'B';
    board[second][firstPiece] = 'B';
    board[second][second] = 'W';
}
void printBoard(char board[][26], int dim)
{
    printf("  "); // Spaces before the first row
    // Print first row starting from 'a'
    for (int row = 0; row < dim; row++)
    {
        printf("%c", row + 'a');
    }
    for (int row = 0; row < dim; row++)
    {
        printf("\n");
        printf("%c ", row + 'a'); // Prints the letter and then space
        for (int col = 0; col < dim; col++)
        {
            printf("%c", board[row][col]); // Prints the board
        }
    }
    printf("\n");
}
void configureBoard(char board[][26])
{
    char input[4];
    char done[4] = "!!!";
    char colour;
    int rowInd = 0, colInd = 0;
    printf("Enter board configuration:\n");
    do
    {
        scanf("%s", input);
        if (strncmp(done, input, 3) != 0)
        {
            colour = input[0];
            // Indexes
            rowInd = input[1] - 'a';
            colInd = input[2] - 'a';
            board[rowInd][colInd] = colour;
        }

    } while (strncmp(done, input, 3) != 0);
}
bool positionInBounds(int dim, int row, int col)
{
    bool in = true;
    if (row < 0 || col < 0 || row >= dim || col >= dim)
        in = false;
    return in;
}
bool checkLegalAndFlip(char board[][26], int dim, int row, int col,
                       char colour, int deltaRow, int deltaCol, bool flip)
{
    int givenRow = row, givenCol = col;
    bool legal = true;
    bool done = false;
    int count = 0; // counts how many opposite colour tiles are passed.
                   // If board is not empty, it is false.
    if (board[row][col] != 'U')
    {
        legal = false;
    }
    // Loop until find the same colour, get out of bounds
    while (!done && positionInBounds(dim, row, col))
    {
        row += deltaRow;
        col += deltaCol;
        if (board[row][col] == 'U')
        {
            legal = false;
            done = true;
        }
        // Opposite colours between pieces must be greater than 0
        else if (board[row][col] == colour)
        {
            if (count == 0)
            {
                legal = false;
            }
            done = true;
        }
        if (board[row][col] != colour)
            count++;
    }
    if (!positionInBounds(dim, row, col))
        legal = false;

    // Flip component use given Row and given Col
    if (legal && flip)
    {
        done = false;
        // Leave the starting cell empty. Add it after with checkAllDirections functions.
        for (int i = 0; i < count; i++)
        {
            givenRow += deltaRow;
            givenCol += deltaCol;
            if (board[givenRow][givenCol] != colour)
            {
                board[givenRow][givenCol] = colour;
            }
        }
    }
    return legal;
}
bool checkAllDirections(char board[][26], int dim, int row, int col, char colour, bool flip)
{
    bool tF = false;
    // South done
    if (checkLegalAndFlip(board, dim, row, col, colour, 1, 0, flip))
        tF = true;
    // South west done
    if (checkLegalAndFlip(board, dim, row, col, colour, 1, -1, flip))
        tF = true;
    // West done
    if (checkLegalAndFlip(board, dim, row, col, colour, 0, -1, flip))
        tF = true;
    // North west done
    if (checkLegalAndFlip(board, dim, row, col, colour, -1, -1, flip))
        tF = true;
    // North done
    if (checkLegalAndFlip(board, dim, row, col, colour, -1, 0, flip))
        tF = true;
    // North east done
    if (checkLegalAndFlip(board, dim, row, col, colour, -1, 1, flip))
        tF = true;
    // East done
    if (checkLegalAndFlip(board, dim, row, col, colour, 0, 1, flip))
        tF = true;
    // South east done
    if (checkLegalAndFlip(board, dim, row, col, colour, 1, 1, flip))
        tF = true;

    if (tF && flip)
        board[row][col] = colour;
    return tF;
}
void checkMove(char board[][26], int dim, char colour)
{
    // This function checks move and prints available moves.
    printf("Available moves for %c:\n", colour);
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            // Check legalDirection in all directions. Print if true
            // South done
            if (checkAllDirections(board, dim, row, col, colour, false))
                printf("%c%c\n", row + 'a', col + 'a');
        }
    }
}

int main(void)
{
    char board[26][26];
    int input = 0;
    printf("Enter the board dimension: ");
    scanf("%d", &input);
    const int dim = input;
    // Initialize
    initializeBoard(board, dim);
    printBoard(board, dim);
    // Configure
    configureBoard(board);
    printBoard(board, dim);
    // Moves for white
    char colour = 'W';
    checkMove(board, dim, colour);
    // Moves for black
    colour = 'B';
    checkMove(board, dim, 'B');
    // Get move
    char move[4];
    printf("Enter a move:\n");
    scanf("%s", move);
    colour = move[0];
    int row = move[1] - 'a';
    int col = move[2] - 'a';
    if (checkAllDirections(board, dim, row, col, colour, false))
    {
        printf("Valid move.\n");
        (checkAllDirections(board, dim, row, col, colour, true));
    }
    else
        printf("Invalid move.\n");
    printBoard(board, dim);

    return 0;
}