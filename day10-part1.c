#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"
#include "deque.h"

#define MAXNMACHINES    200
#define BUFLEN          250

typedef struct {
    int nlights;
    int lights;
} Button;

typedef struct {
    int npresses;
    int lights;
    int lastpress;
} QueueElm;



int main()
{
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day10", "r");
    if (!fp) { perror("fopen"); exit(1); }
    int sum = 0;

    while ( myfgets(buffer, BUFLEN, fp)) {
        char *token = strtok(buffer, " ");
        int len = strlen(buffer);
        int nlights = len - 2;
        int lights = 0;
        for (int i = 0; i < nlights; i++) {
            if (token[i + 1] == '#') lights |= 1;
            lights <<= 1;
        }

        token = strtok(NULL, " ");
        int nbuttons = 0;
        Button buttons[15];
        while (token[0] != '{') {
            Button *btnptr = buttons + nbuttons++;
            btnptr->nlights = 0;
            btnptr->lights = 0;
            char *saveptr;
            char *subtok = strtok_r(token + 1, ",", &saveptr);
            while (subtok) {
                btnptr->lights |= (1 << (nlights - atoi(subtok)));
                subtok = strtok_r(NULL, ",", &saveptr);
            }
            btnptr++;
            token = strtok(NULL, " ");
        }

        deque *queue = deque_create();
        QueueElm *elm = malloc(sizeof(QueueElm));
        elm->npresses = 0;
        elm->lights = 0;
        elm->lastpress = -1;
        while (true) {
            if (elm->lights == lights) { sum += elm->npresses; break; }
            for (int b = 0; b < nbuttons; b++) {
                if (b == elm->lastpress) continue;
                Button *btnptr = buttons + b;
                QueueElm *newelm = malloc(sizeof(QueueElm));
                newelm->npresses = elm->npresses + 1;
                newelm->lights = elm->lights ^ btnptr->lights;
                newelm->lastpress = b;
                deque_push_tail(queue, newelm);
            }
            free(elm);
            elm = deque_pop_head(queue);
        }
    }

    printf("%i\n", sum);

    exit(0);
}

