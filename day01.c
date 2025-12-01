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
    int pos = STARTPOS;
    int count1 = 0;
    int count2 = 0;
    while (fgetsret != NULL) {
        char c = *buffer;
        int rot = atoi(buffer + 1);
        int dist = 0;
        switch(c) {
            case 'L':
                dist = pos;
                pos -= rot;
                break;
            case 'R':
                dist = DIALSIZE - pos;
                pos += rot;
                break;
            default:
                fprintf(stderr, "Unknow character: %c\n", c);
                exit(1);
        }

        if (rot >= dist) {
            int over = rot - dist;
            div_t res = div(over, DIALSIZE);
            count2 += res.quot;
            if (dist != 0) count2++;
            if (res.rem == 0) count1++;
        }

        pos = modulo(pos, DIALSIZE);
        fgetsret = myfgets(buffer, BUFLEN, fp);
    }

    printf("%i\n%i\n", count1, count2);

    exit(0);
}

