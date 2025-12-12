#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"

#define BUFLEN      200
#define MAXNDEVS    600

typedef struct ListNode ListNode;
struct ListNode { int hash; ListNode *next; };
typedef struct { int hash; ListNode *outputs; long cache[4]; } Device;

int compar_devices(const void *a, const void * b);
long recurse(char dac, char fft, int hash);
int gethash(char *label);

Device devices[MAXNDEVS];
int ndevices = 0;
int dachash, ffthash;



int main()
{
    FILE *fp = fopen("inputs/day11", "r");
    if (!fp) { perror("fopen"); exit(1); }
    char buffer[BUFLEN];

    dachash = gethash("dac");
    ffthash = gethash("fft");

    while (myfgets(buffer, BUFLEN, fp)) {
        Device *newdev = devices + ndevices++;
        newdev->outputs = NULL;
        newdev->hash    = gethash(buffer);
        for (int i = 0; i < 4; i++)
            newdev->cache[i] = -1;
        char *token = strtok(buffer + 5, " ");
        while (token) {
            ListNode *newout = malloc(sizeof(ListNode));
            newout->hash    = gethash(token);
            newout->next    = newdev->outputs;
            newdev->outputs = newout;
            token = strtok(NULL, " ");
        }
    }

    Device *outdev   = devices + ndevices++;
    outdev->hash     = gethash("out");
    outdev->outputs  = NULL;
    outdev->cache[0] = 0;
    outdev->cache[1] = 0;
    outdev->cache[2] = 0;
    outdev->cache[3] = 1;

    qsort(devices, ndevices, sizeof(Device), compar_devices);
    printf("%li\n", recurse(0, 0, gethash("svr")));

    exit(0);
}



long recurse(char dac, char fft, int hash)
{
    if (hash == dachash) dac = 1;
    else if (hash == ffthash) fft = 1;

    int index = (dac << 1) + fft;

    Device sdev;
    sdev.hash = hash;
    Device *dev = binsearch(devices, ndevices, sizeof(Device), compar_devices, &sdev);
    long *value = dev->cache + index;

    if (*value != -1)
        return *value;
    else {
        long sum = 0;
        for (ListNode *out = dev->outputs; out != NULL; out = out->next)
            sum += recurse(dac, fft, out->hash);
        *value = sum;
        return sum;
    }
}



int compar_devices(const void *a, const void * b)
{
    return ((Device *)a)->hash - ((Device *)b)->hash;
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

