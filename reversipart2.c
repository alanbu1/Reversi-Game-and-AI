// Alan Bu, April 2 2022
// Reversi Part 2:
// Move priority. Corner = priority 1,
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
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
        done = true;
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
void copyingBoard(char board[][26], char copyBoard[][26], int dim)
{
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            copyBoard[row][col] = board[row][col];
        }
    }
}
bool checkAroundCorners(char board[][26], int dim, int row, int col)
{
    bool around = false;
    if ((row == 0 && (col == 1 || col == dim - 2)) || (row == 1 && (col == 0 || col == 1 || col == dim - 2 || col == dim - 1)) || (row == dim - 2 && (col == 0 || col == 1 || col == dim - 2 | col == dim - 1)) || (row == dim - 1 && (col == 1 || col == dim - 2)))
        around = true;

    return around;
}
bool checkMove(char board[][26], int dim, char colour)
{
    bool available = false;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            // Check legalDirection in all directions. Print if true
            // South done
            if (checkAllDirections(board, dim, row, col, colour, false))
            { // Add a counter for legal moves?
                available = true;
                return available;
            }
        }
    }
    return available;
}
int numberOfTiles(char board[][26], int dim, char colour)
{
    int count = 0;
    for (int row = 0; row < dim; row++)
    {
        for (int col = 0; col < dim; col++)
        {
            if (board[row][col] == colour)
                count++;
        }
    }
    return count;
}
bool edgePiece(char board[][26], int dim, int row, int col)
{
    if ((row == 0 && col > 1 && col < dim - 2) || (col == 0 && row > 1 && row < dim - 2) || (row == dim - 1 && col > 1 && col < dim - 2) || (col == dim - 1 && row > 1 && row < dim - 2))
    {
        return true;
    }
    return false;
}

void newComputerMove(char board[][26], char copyBoard[][26], int dim, char computerColour, int *row, int *col)
{
    int startingNumber = numberOfTiles(board, dim, computerColour);
    int highestScore = startingNumber; // highest score is the highest amount of tiles flipped in a move
    int lowestScore = dim * dim;
    char highRowInd, highColInd, lowRowInd, lowColInd;
    for (int row = 0; row < dim; row++)
    {
        // copy the board and use it for each position to determine number of black tiles flipped
        for (int col = 0; col < dim; col++)
        {
            copyingBoard(board, copyBoard, dim);
            (checkAllDirections(copyBoard, dim, row, col, computerColour, true));
            if ((numberOfTiles(copyBoard, dim, computerColour) > highestScore) && !(checkAroundCorners(copyBoard, dim, row, col)))
            {
                highestScore = (numberOfTiles(copyBoard, dim, computerColour));
                highRowInd = row + 'a';
                highColInd = col + 'a';
            }
            if ((numberOfTiles(copyBoard, dim, computerColour) < lowestScore) && !(checkAroundCorners(copyBoard, dim, row, col)))
            {
                lowestScore = (numberOfTiles(copyBoard, dim, computerColour));
                lowRowInd = row + 'a';
                lowColInd = col + 'a';
            }
        }
    }
    int half = (dim * dim) * 1 / 3; // Actually a third of the board filled. For some reason this wins. 
    int count = 0;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] == 'U')
                count++;
        }
    }
    if (count < half)
    {
        *row = lowRowInd - 'a';
        *col = lowColInd - 'a';
    }
    else
    {
        *row = highRowInd - 'a';
        *col = highColInd - 'a';
    }
}
int makeMove(const char board[][26], int dim, char colour, int *row, int *col)
{
    // There will be a list of priority moves. Corners will be priority 1, board edges, except the ones next to corners will be priority 2
    // Most flipped priority 3, priority 4 will be the pieces around the corners.
    bool foundMove = false;
    // Check corners. First priority.
    if ((checkAllDirections(board, dim, 0, 0, colour, false)) || (checkAllDirections(board, dim, dim - 1, dim - 1, colour, false)) || (checkAllDirections(board, dim, 0, dim - 1, colour, false)) || (checkAllDirections(board, dim, dim - 1, 0, colour, false)))
    {
        if (checkAllDirections(board, dim, 0, 0, colour, false))
        {
            *row = 0;
            *col = 0;
        }
        else if (checkAllDirections(board, dim, dim - 1, dim - 1, colour, false))
        {
            *row = dim - 1;
            *col = dim - 1;
        }
        else if (checkAllDirections(board, dim, 0, dim - 1, colour, false))
        {
            *row = 0;
            *col = dim - 1;
        }
        else
        {
            *row = dim - 1;
            *col = 0;
        }
        foundMove = true;
    }
    if (!foundMove)
    {                                               // checking second priority
        for (int i = 0; i < dim && !foundMove; i++) // row
        {
            for (int j = 0; j < 0 && !foundMove; j++) // col
            {
                if (edgePiece(board, dim, i, j) && (checkAllDirections(board, dim, i, j, colour, false)))
                {
                    *row = i;
                    *col = j;
                    foundMove = true;
                }
            }
        }
    }

    // Third priority, pieces in the centre.
    if (!foundMove)
    {
        for (int i = 0; i < dim && !foundMove; i++)
        {
            for (int j = 0; j < dim && !foundMove; j++)
            {
                for (int k = 1; k < dim / 2 && !foundMove; k++)
                {
                    if ((checkAllDirections(board, dim, i, j, colour, false)) && ((row == dim / 2 + k) || (row == dim / 2 - k + 1)) && ((col == dim / 2 + k) || (col == dim / 2 - k + 1)))
                    {
                        *row = i;
                        *col = j;
                        foundMove = true;
                    }
                }
            }
        }
    }
    // fourth
    if (!foundMove) // fourth priority
    {
        char copyBoard[26][26];
        int fliprow, flipcol;
        newComputerMove(board, copyBoard, dim, colour, &fliprow, &flipcol);
        if ((checkAllDirections(board, dim, fliprow, flipcol, colour, false)))
        {
            *row = fliprow;
            *col = flipcol;
            foundMove = true;
        }
    }

    if (!foundMove)
    { // at this point put a random piece...
        for (int i = 0; i < dim && !foundMove; i++)
        {
            for (int j = 0; j < dim && !foundMove; j++)
            {
                if (checkAllDirections(board, dim, i, j, colour, false))
                {
                    *row = i;
                    *col = j;
                    foundMove = true;
                }
            }
        }
    }

    return 0;
}