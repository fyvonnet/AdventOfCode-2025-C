#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFLEN  110

unsigned long findmaxjolt(char *batteries, int banklen, int nbatts);



int main()
{
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day03", "r");
    if (!fp) { perror("fopen"); exit(1); }

    unsigned long sumjolt1 = 0;
    unsigned long sumjolt2 = 0;
    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    
    while (fgetsret != NULL) {
        int banklen = strlen(buffer);
        char batteries[banklen];

        for (int b = 0; b < banklen; b++)
            batteries[b] = buffer[b] - '0';

        sumjolt1 += findmaxjolt(batteries, banklen,  2);
        sumjolt2 += findmaxjolt(batteries, banklen, 12);

        fgetsret = myfgets(buffer, BUFLEN, fp);
    }

    fclose(fp);

    printf("%li\n", sumjolt1);
    printf("%li\n", sumjolt2);

    exit(0);
}



unsigned long findmaxjolt(char *batteries, int banklen, int nbatts)
{
        unsigned long joltage = 0;
        int maxjolt, maxpos = -1;

        for (int i = 0; i < nbatts; i++) {
            maxjolt = 0;
            for (int b = maxpos + 1; b < banklen - nbatts + i + 1; b++)
                if (batteries[b] > maxjolt) {
                    maxjolt = batteries[b];
                    maxpos = b;
                }
            joltage = (joltage * 10) + maxjolt;
        }

        return joltage;
}

