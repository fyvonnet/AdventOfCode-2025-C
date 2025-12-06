#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFLEN      3800
#define MAXWIDTH    1000
#define MAXHEIGHT   5
#define ADD         '+'
#define MULT        '*'



int main()
{
    char operations[MAXWIDTH];
    char buffer[BUFLEN];
    char *lines[MAXHEIGHT];
    FILE *fp = fopen("inputs/day06", "r");
    if (!fp) { perror("fopen"); exit(1); }
    int matrix[MAXWIDTH][MAXHEIGHT];
    int col, row, ncols, nrows;
    unsigned long sum, acc;

    myfgets(buffer, BUFLEN, fp);
    row = 0;
    while ((*buffer != '*') && (*buffer != '+')) {
        lines[row] = malloc(BUFLEN);
        strncpy(lines[row], buffer, BUFLEN);
        col = 0;
        char *token = strtok(buffer, " ");
        while (token) {
            matrix[col++][row] = atoi(token);
            token = strtok(NULL, " ");
        }
        row++;
        myfgets(buffer, BUFLEN, fp);
    }
    nrows = row;

    fclose(fp);

    // measure length of digits by counting space after operators in the last line
    int lens[MAXWIDTH];
    col = 0;
    char *ptr = buffer;
    while (*ptr != '\0') {
        operations[col] = *ptr;
        ptr++;
        int len = 0;
        while (*ptr == ' ') {
            len++;
            ptr++;
        }
        // add one for the last column
        if (*ptr == '\0')
            len++;
        lens[col] = len;
        col++;
    }
    ncols = col;

    // start position of each column in the input file
    int poss[ncols];
    poss[0] = 0;
    for (col = 0; col < ncols - 1; col++)
        poss[col + 1] = poss[col] + lens[col] + 1;

    // part 1
    sum = 0;
    for (col = 0; col < ncols; col++) {
        char op = operations[col];
        acc = matrix[col][0];
        for (row = 1; row < nrows; row++) {
            int n = matrix[col][row];
            acc = (op == MULT ? (acc * n) : (acc + n));
        }
        sum += acc;
    }
    printf("%li\n", sum);

    // part 2
    sum = 0;
    for (int col = 0; col < ncols; col++) {
        char op = operations[col];
        acc = (op == MULT ? 1 : 0);
        for (int icol = poss[col]; icol < poss[col] + lens[col]; icol++) {
            int n = 0;
            for (int row = 0; row < nrows; row++) {
                char *line = lines[row];
                char c = line[icol];
                if (c != ' ')
                    n += (c - '0');
                else if (n != 0)
                    break;
                n *= 10;
            }
            n /= 10;
            acc = (op == MULT ? (acc * n) : (acc + n));
        }
        sum += acc;
    }
    printf("%li\n", sum);

    exit(0);
}

