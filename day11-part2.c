#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"
#include "redblacktree.h"

#define BUFLEN      200
#define MAXNDEVS    600

typedef struct ListElm ListElm;
struct ListElm { int label; ListElm *next; };
typedef struct { int label; ListElm *outputs;} Device;
typedef struct { int label; bool dac, fft; unsigned long value; } CacheElm;

int compar_devices(const void *a, const void * b);
int compar_cacheelms(const void *a, const void * b);
unsigned long recurse(bool dac, bool fft, int label);
int gethash(char *label);

Device devices[MAXNDEVS];
int ndevices = 0;
redblacktree *cache;
int outhash, dachash, ffthash;



int main()
{
    FILE *fp = fopen("inputs/day11", "r");
    if (!fp) { perror("fopen"); exit(1); }
    char buffer[BUFLEN];

    outhash = gethash("out");
    dachash = gethash("dac");
    ffthash = gethash("fft");

    while (myfgets(buffer, BUFLEN, fp)) {
        Device *newdev = devices + ndevices++;
        newdev->outputs = NULL;
        newdev->label   = gethash(buffer);
        char *token = strtok(buffer + 5, " ");
        while (token) {
            ListElm *newout = malloc(sizeof(ListElm));
            newout->label   = gethash(token);
            newout->next    = newdev->outputs;
            newdev->outputs = newout;
            token = strtok(NULL, " ");
        }
    }

    cache = redblacktree_init(compar_cacheelms);
    qsort(devices, ndevices, sizeof(Device), compar_devices);
    printf("%li\n", recurse(false, false, gethash("svr")));

    exit(0);
}



unsigned long recurse(bool dac, bool fft, int label)
{
    if (label == outhash) {
        if (dac && fft) return 1;
        else return 0;
    }

    if (label == dachash) dac = true;
    else if (label == ffthash) fft = true;

    CacheElm *elm = malloc(sizeof(CacheElm));
    elm->label = label;
    elm->dac   = dac;
    elm->fft   = fft;

    CacheElm *ret = redblacktree_insert(cache, elm);
    if (ret == NULL) {
        Device sdev;
        sdev.label = label;
        Device *dev = binsearch(devices, ndevices, sizeof(Device), compar_devices, &sdev);
        unsigned long sum = 0;
        for (ListElm *out = dev->outputs; out != NULL; out = out->next)
            sum += recurse(dac, fft, out->label);
        elm->value = sum;
        return sum;
    }
    else {
        free(elm);
        return ret->value;
    }
}



int compar_devices(const void *a, const void * b)
{
    Device *aa = (Device *)a;
    Device *bb = (Device *)b;
    return aa->label - bb->label;
}



int compar_cacheelms(const void *a, const void * b)
{
    CacheElm *aa = (CacheElm *)a;
    CacheElm *bb = (CacheElm *)b;
    int ret = aa->label - bb->label;
    if (ret != 0) return ret;
    ret = aa->dac - bb->dac;
    if (ret != 0) return ret;
    return aa->fft - bb->fft;
}



int gethash(char *label)
{
    int hash = 0;
    for (int i = 0; i < 3; i++) {
        hash <<= 7;
        hash |= *label++;
    }
    return hash;
}

