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

    tiles[ntiles] = tiles[0];
    recptr = rectangles;
    for (int r = 0; r < nrectangles; r++) {
        int maxx, maxy, minx, miny;
        if (recptr->a->x > recptr->b->x) { maxx = recptr->a->x; minx = recptr->b->x; }
        else  { maxx = recptr->b->x; minx = recptr->a->x; }
        if (recptr->a->y > recptr->b->y) { maxy = recptr->a->y; miny = recptr->b->y; }
        else  { maxy = recptr->b->y; miny = recptr->a->y; }
        bool valid = true;
        tileptr = tiles;
        for (int t = 0; t < ntiles; t++) {
            // first filter rectangles with no points inside
            bool xwithin = (tileptr->x > minx) && (tileptr->x < maxx);
            bool ywithin = (tileptr->y > miny) && (tileptr->y < maxy);
            if (xwithin && ywithin) { valid = false; break; }
            Tile *tileptr2 = tileptr + 1;
            if (tileptr2->x == tileptr->x) {
                // x within but y's above and below
                if (xwithin)
                    if ((tileptr->y >= maxy && tileptr2->y <= miny) || (tileptr2->y >= maxy && tileptr->y <= miny)) {
                        valid = false;
                        break;
                    }
            }
            else {
                // y within but x's left and right
                if (ywithin)
                    if ((tileptr->x >= maxx && tileptr2->x <= minx) || (tileptr2->x >= maxx && tileptr->x <= minx)) {
                        valid = false;
                        break;
                    }
            }
            tileptr++;
        }
        if (valid) { printf("%li\n", recptr->area); break; }
        recptr++;
    }

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

