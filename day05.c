#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#define BUFLEN 40
#define MAXRANGES 200
#define MAXNIDS     1000

typedef struct { unsigned long s, e; } Range;

int compar(const void *a, const void *b);



int main()
{
    Range ranges[MAXRANGES];
    int nranges = 0;
    char buffer[BUFLEN];

    FILE *fp = fopen("inputs/day05", "r");
    if (!fp) { perror("fopen"); exit(1); }

    myfgets(buffer, BUFLEN, fp);
    for (int r = 0; strlen(buffer) != 0; r++) {
        nranges++;
        char *token = strtok(buffer, "-");
        ranges[r].s = atol(token);
        token = strtok(NULL, "-");
        ranges[r].e = atol(token) + 1;
        myfgets(buffer, BUFLEN, fp);
    }

    qsort(ranges, nranges, sizeof(Range), compar);

    Range mranges[MAXRANGES];
    int nmranges = 0;
    int ri = 0;
    unsigned long ids = 0;
    int mr = 0;
    while (ri < nranges) {
        nmranges++;
        Range range;
        range = ranges[ri];
        int rn;
        for (rn = ri + 1; (rn < nranges) && (range.e > ranges[rn].s); rn++)
            if (ranges[rn].e > range.e)
                range.e = ranges[rn].e;
        mranges[mr] = range;
        mr++;
        ids += range.e - range.s;
        ri = rn;
    }

    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    int nfresh = 0;
    while (fgetsret != NULL) {
        unsigned long n = atol(buffer);
        bool spoiled = true;
        for (int r = 0; r < nmranges; r++)
            if ((n >= mranges[r].s) && (n < mranges[r].e)) {
                spoiled = false;
                break;
            }
        if (!spoiled) nfresh++;
        fgetsret = myfgets(buffer, BUFLEN, fp);
    }

    fclose (fp);

    printf("%i\n", nfresh);
    printf("%li\n", ids);

    exit(0);
}



int compar(const void *a, const void *b)
{
    Range *aa = (Range *)a;
    Range *bb = (Range *)b;

    if (aa->s > bb->s)
        return 1;

    if (aa->s < bb->s)
        return -1;

    return 0;

}

