#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFLEN          150
#define GRIDMAXSIDE     135
#define ROLL            '@'
#define EMPTY           '.'

typedef struct ListElm ListElm;

struct ListElm {
    int col, row;
    ListElm *next;
};



int main()
{
    char grid[GRIDMAXSIDE][GRIDMAXSIDE];
    char buffer[BUFLEN];

    ListElm *rollslst = NULL;
    ListElm *removedlst = NULL;

    FILE *fp = fopen("inputs/day04", "r");
    if (!fp) { perror("fopen"); exit(1); }

    myfgets(buffer, BUFLEN, fp);
    int gridside = strlen(buffer);

    for (int row = 0; row < gridside; row++) {
        for (int col = 0; col < gridside; col++) {
            char c = buffer[col];
            grid[col][row] = c;
            if (c == ROLL) {
                ListElm *elm = malloc(sizeof(ListElm));
                elm->row = row;
                elm->col = col;
                elm->next = rollslst;
                rollslst = elm;
            }
        }
        myfgets(buffer, BUFLEN, fp);
    }

    fclose (fp);

    int removed = 0;
    int totalremoved = 0;
    do {
        removed = 0;
        ListElm *tmprollslst = NULL;
        ListElm *elm = rollslst;
        while (elm != NULL) {
            ListElm *next = elm->next;
            int nrolls = 0;
            for (int nrow = -1; nrow <= 1; nrow++)
                for (int ncol = -1; ncol <= 1; ncol++) {
                    if ((nrow == 0) && (ncol == 0)) continue;
                    int r = elm->row + nrow;
                    int c = elm->col + ncol;
                    if (r < 0) continue;
                    if (c < 0) continue;
                    if (r >= gridside) continue;
                    if (c >= gridside) continue;
                    if (grid[c][r] == ROLL) nrolls++;
                }
            if (nrolls < 4) {
                removed++;
                elm->next = removedlst;
                removedlst = elm;
            }
            else {
                elm->next = tmprollslst;
                tmprollslst = elm;
            }
            elm = next;
        }

        totalremoved += removed;

        elm = removedlst;
        while (elm != NULL) {
            ListElm *next = elm->next;
            grid[elm->col][elm->row] = EMPTY;
            free(elm);
            elm = next;
        }
        removedlst = NULL;
        rollslst = tmprollslst;
    } while (removed != 0);

    printf("%i\n", totalremoved);

    exit(0);
}

