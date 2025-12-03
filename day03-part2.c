#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFLEN  110
#define NBATTS  12



int main()
{
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day03", "r");
    if (!fp) { perror("fopen"); exit(1); }

    unsigned long sumjolt = 0;
    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    while (fgetsret) {
        int banklen = strlen(buffer);
        int batteries[banklen];

        for (int b = 0; b < banklen; b++)
            batteries[b] = buffer[b] - '0';

        unsigned long joltage = 0;
        int maxpos = -1;
        int maxjolt;
        for (int i = 0; i < NBATTS; i++) {
            maxjolt = 0;
            for (int b = maxpos + 1; b < banklen - NBATTS + i + 1; b++)
                if (batteries[b] > maxjolt) {
                    maxjolt = batteries[b];
                    maxpos = b;
                }
            joltage = (joltage * 10) + maxjolt;
        }
        sumjolt += joltage;

        fgetsret = myfgets(buffer, BUFLEN, fp);
    }

    printf("%li\n", sumjolt);

    exit(0);
}

