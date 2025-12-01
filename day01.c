#include <stdlib.h>
#include <stdio.h>
#include "misc.h"

#define BUFLEN      10
#define STARTPOS    50
#define DIALSIZE    100


int main()
{
    FILE *fp = fopen("inputs/day01", "r");
    if (!fp) { perror("fopen"); exit(1); }
    char buffer[BUFLEN];

    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    int position = STARTPOS;
    int count1 = 0;
    int count2 = 0;
    while (fgetsret != NULL) {
        char c = *buffer;
        int rotations = atoi(buffer + 1);
        int clicks_to_zero = 0;
        switch(c) {
            case 'L':
                clicks_to_zero = position;
                position -= rotations;
                break;
            case 'R':
                clicks_to_zero = DIALSIZE - position;
                position += rotations;
                break;
            default:
                fprintf(stderr, "Unknow character: %c\n", c);
                exit(1);
        }

        if (rotations >= clicks_to_zero) {
            int clicks_over_zero = rotations - clicks_to_zero;
            div_t res = div(clicks_over_zero, DIALSIZE);
            count2 += res.quot; // number of complete rotation of the dial starting at 0
            if (clicks_to_zero != 0) count2++; // we only reach 0 if we're not already on it
            if (res.rem == 0) count1++;
        }

        position = modulo(position, DIALSIZE);
        fgetsret = myfgets(buffer, BUFLEN, fp);
    }

    printf("%i\n%i\n", count1, count2);

    exit(0);
}

