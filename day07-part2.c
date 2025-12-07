#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define MAXWIDTH    150
#define MAXHEIGHT   150

typedef unsigned long BigInt;
typedef enum { EMPTY, SPLITTER, BEAM } SquareType;
typedef struct { SquareType type; BigInt count, cache; } Square;

BigInt recurse(int col, int row, BigInt count);

Square manifold[MAXWIDTH][MAXHEIGHT];
int maniheight;



int main()
{ 
    char buffer[MAXWIDTH];
    FILE *fp = fopen("inputs/day07", "r");
    if (!fp) { perror("fopen"); exit(1); }

    char *fgetsret = myfgets(buffer, MAXWIDTH, fp);
    int maniwidth = strlen(buffer);
    int row = 0;
    int startcol = -1;
    int startrow = -1;
    while (fgetsret != NULL) {
        for (int col = 0; col < maniwidth; col++) {
            char c = buffer[col];
            SquareType t = EMPTY;
            switch (c) {
                case 'S':
                    startcol = col;
                    startrow = row;
                    break;
                case '.':
                    break;
                case '^':
                    t = SPLITTER;
                    break;
                default:
                    fprintf(stderr, "Unknown character: %c\n", c);
                    exit(1);
            }
            manifold[col][row].type  = t;
            manifold[col][row].cache = 0;
        }
        row++;
        fgetsret = myfgets(buffer, MAXWIDTH, fp);
    }
    maniheight = row;

    printf("%li\n", recurse(startcol, startrow, 1));

    exit(0);
}



BigInt recurse(int col, int row, BigInt count)
{
    if (row == maniheight) return count;

    Square *sq = &manifold[col][row];
    if (sq->cache == 0) {
        BigInt retcount;
        switch(sq->type) {
            case BEAM:
                sq->count =+ count;
                retcount = recurse(col, row + 1, sq->count);
                break;
            case SPLITTER:
                retcount = recurse(col - 1, row, count) + recurse(col + 1, row, count);
                break;
            case EMPTY:
                sq->type = BEAM;
                sq->count = count;
                retcount = recurse(col, row + 1, count);
                break;
            default:
                fprintf(stderr, "Wrong type: %i\n", sq->type);
                exit(1);
        }
        sq->cache = retcount;
        return retcount;
    }
    else return sq->cache;
}

