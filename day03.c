#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFLEN  110



int main()
{
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day03", "r");
    if (!fp) { perror("fopen"); exit(1); }

    int sumjolt = 0;
    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    while (fgetsret) {
        int banklen = strlen(buffer);
        int batteries[banklen];
        int joltage = 0;

        for (int b = 0; b < banklen; b++)
            batteries[b] = buffer[b] - '0';

        int maxjolt = -1;
        int maxpos = -1;
        for (int b = 0; b < banklen - 1; b++)
            if (batteries[b] > maxjolt) {
                maxjolt = batteries[b];
                maxpos = b;
            }

        joltage = maxjolt * 10;

        maxjolt = -1;
        for (int b = maxpos + 1; b < banklen; b++)
            if (batteries[b] > maxjolt)
                maxjolt = batteries[b];

        joltage += maxjolt;
        sumjolt += joltage;

        fgetsret = myfgets(buffer, BUFLEN, fp);
    }

    printf("%i\n", sumjolt);

    exit(0);
}

