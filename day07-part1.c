#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define MAXWIDTH    150
#define MAXHEIGHT   150

#define START       'S'
#define EMPTY       '.'
#define BEAM        '|'
#define SPLITTER    '^'

typedef unsigned long BigInt;

BigInt recurse(int col, int row);

char manifold[MAXWIDTH][MAXHEIGHT];
int maniheight;



int main()
{ 
    char buffer[MAXWIDTH];
    //FILE *fp = fopen("inputs/day07-test", "r");
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
            if (c == START) {
                c = EMPTY;
                startcol = col;
                startrow = row;
            }
            manifold[col][row] = c;
        }
        row++;
        fgetsret = myfgets(buffer, MAXWIDTH, fp);
    }
    maniheight = row;

    printf("%li\n", recurse(startcol, startrow));

    exit(0);
}



BigInt recurse(int col, int row)
{
    if (row == maniheight) return 0;

    char c = manifold[col][row];
    switch(c) {
        case BEAM:
            return 0;
        case SPLITTER:
            return 1 + recurse(col - 1, row) + recurse(col + 1, row);
        case EMPTY:
            manifold[col][row] = BEAM;
            return recurse(col, row + 1);
        default:
            fprintf(stderr, "Wrong character: %c\n", c);
            exit(1);
    }

    return 0; // should not reach here;
}

