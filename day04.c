#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFLEN          150
#define GRIDMAXSIDE     135
#define ROLL            '@'
#define EMPTY           '.'
#define REMOVED         'x'



int main()
{
    char grid[GRIDMAXSIDE][GRIDMAXSIDE];
    char buffer[BUFLEN];

    FILE *fp = fopen("inputs/day04", "r");
    if (!fp) { perror("fopen"); exit(1); }

    myfgets(buffer, BUFLEN, fp);
    int gridside = strlen(buffer);

    for (int row = 0; row < gridside; row++) {
        for (int col = 0; col < gridside; col++)
            grid[col][row] = buffer[col];
        myfgets(buffer, BUFLEN, fp);
    }

    fclose (fp);

    int removed, sumremoved = 0;
    do {
        removed = 0;
        for (int row = 0; row < gridside; row++) {
            for (int col = 0; col < gridside; col++) {
                char s = grid[col][row];
                if ((s == ROLL) && (s != REMOVED)) {
                    int nrolls = 0;
                    for (int nrow = -1; nrow <= 1; nrow++) {
                        for (int ncol = -1; ncol <= 1; ncol++) {
                            if ((nrow == 0) && (ncol == 0)) continue;
                            int r = row + nrow;
                            int c = col + ncol;
                            if (r < 0) continue;
                            if (c < 0) continue;
                            if (r >= gridside) continue;
                            if (c >= gridside) continue;
                            char s = grid[c][r];
                            if ((s == ROLL) || (s == REMOVED)) nrolls++;
                        }
                    }
                    if (nrolls < 4) {
                        removed++;
                        grid[col][row] = REMOVED;
                    }
                }
            }
        }
        sumremoved += removed;

        for (int row = 0; row < gridside; row++)
            for (int col = 0; col < gridside; col++)
                if (grid[col][row] == REMOVED)
                    grid[col][row] = EMPTY;
    } while (removed != 0);

    printf("%i\n", sumremoved);

    exit(0);
}

