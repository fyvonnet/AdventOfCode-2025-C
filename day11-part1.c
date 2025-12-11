#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"
#include "deque.h"

#define BUFLEN      200
#define MAXNDEVS    600

typedef struct ListElm ListElm;
struct ListElm { char label[5]; ListElm *next; };
typedef struct { char label[5]; ListElm *outputs; } Device;

int compar_device(const void *a, const void * b);



int main()
{
    FILE *fp = fopen("inputs/day11", "r");
    if (!fp) { perror("fopen"); exit(1); }
    char buffer[BUFLEN];
    Device devices[MAXNDEVS];

    int ndevices = 0;
    while (myfgets(buffer, BUFLEN, fp)) {
        Device *newdev = devices + ndevices++;
        newdev->outputs = NULL;
        strncpy(newdev->label, buffer, 3);
        char *token = strtok(buffer + 5, " ");
        while (token) {
            ListElm *newout = malloc(sizeof(ListElm));
            strncpy(newout->label, token, 3);
            newout->next = newdev->outputs;
            newdev->outputs = newout;
            token = strtok(NULL, " ");
        }
    }

    qsort(devices, ndevices, sizeof(Device), compar_device);

    int count = 0;
    char *label;
    deque *queue = deque_create();
    deque_push_tail(queue, "you");
    while ((label = deque_pop_head(queue)) != NULL)
        if (!strncmp(label, "out", 3))
            count++;
        else {
            Device *dev = binsearch(devices, ndevices, sizeof(Device), compar_device, label);
            for (ListElm *out = dev->outputs; out != NULL; out = out->next)
                deque_push_tail(queue, out->label);
        }

    printf("%i\n", count);

    exit(0);
}



int compar_device(const void *a, const void * b)
{
    return strncmp((const char *)a, (const char *)b, 3);
}

