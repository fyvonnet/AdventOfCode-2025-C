#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#define BUFLEN      32
#define MAXNTILES   500

typedef unsigned long BigInt;
typedef struct { int x, y; } Tile;
typedef struct { Tile *a, *b; BigInt area; } Rectangle;

int compar_area_inv(const void *a, const void *b);



int main()
{
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day09", "r");
    if (!fp) { perror("fopen"); exit(1); }

    Tile tiles[MAXNTILES];
    Tile *tileptr = tiles;
    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    while (fgetsret != NULL) {
        char *token;
        token = strtok(buffer, ",");
        tileptr->x = atoi(token);
        token = strtok(NULL, ",");
        tileptr->y = atoi(token);
        tileptr++;
        fgetsret = myfgets(buffer, BUFLEN, fp);
    }
    int ntiles = tileptr - tiles;

    int nrectangles = (ntiles * (ntiles - 1)) / 2;
    Rectangle rectangles[nrectangles];
    Rectangle *recptr = rectangles;

    for (int a = 0; a < ntiles - 1; a++)
        for (int b = a + 1; b < ntiles; b++) {
            Tile *ptra = tiles + a;
            Tile *ptrb = tiles + b;
            recptr->a = ptra;
            recptr->b = ptrb;
            recptr->area = (labs(ptra->x - ptrb->x) + 1) * (labs(ptra->y - ptrb->y) + 1);
            recptr++;
        }

    qsort(rectangles, nrectangles, sizeof(Rectangle), compar_area_inv);

    printf("%li\n", rectangles[0].area);

    exit(0);
}



int compar_area_inv(const void *a, const void *b)
{
    Rectangle *aa = (Rectangle *)a;
    Rectangle *bb = (Rectangle *)b;

    if (aa->area > bb->area) return -1;
    if (aa->area < bb->area) return  1;
    return 0;
}

