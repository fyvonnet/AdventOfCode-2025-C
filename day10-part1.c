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
    int lights[10];
} Button;

typedef struct {
    int nlights;
    bool lights[10];
    int nbuttons;
    Button buttons[15];
} Machine;

typedef struct {
    int npresses;
    bool lights[10];
} QueueElm;



int main()
{
    Machine machines[MAXNMACHINES];
    Machine *machptr = machines;
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day10", "r");
    if (!fp) { perror("fopen"); exit(1); }
    char *fgetsret = myfgets(buffer, BUFLEN, fp);

    while (fgetsret) {
        char *token = strtok(buffer, " ");
        int len = strlen(buffer);
        machptr->nlights = len - 2;
        for (int i = 0; i < machptr->nlights; i++) {
            char c = token[i + 1];
            switch(c) {
                case '.':
                    machptr->lights[i] = false;
                    break;
                case '#':
                    machptr->lights[i] = true;
                    break;
                default:
                    fprintf(stderr, "Wrong character: %c\n", c);
                    exit(1);
            }
        }

        token = strtok(NULL, " ");
        machptr->nbuttons = 0;
        while (token[0] != '{') {
            Button *btnptr = machptr->buttons + machptr->nbuttons++;
            btnptr->nlights = 0;
            char *saveptr;
            char *subtok = strtok_r(token + 1, ",", &saveptr);
            while (subtok) {
                btnptr->lights[btnptr->nlights++] = atoi(subtok);
                subtok = strtok_r(NULL, ",", &saveptr);
            }
            btnptr++;
            token = strtok(NULL, " ");
        }

        machptr++;
        fgetsret = myfgets(buffer, BUFLEN, fp);
    }
    int nmachines = machptr - machines;

    int sum = 0;
    for (int m = 0; m < nmachines; m++) {
        deque *queue = deque_create();
        machptr = machines + m;
        QueueElm *elm = malloc(sizeof(QueueElm));
        elm->npresses = 0;
        int nlights = machptr->nlights;
        int nbuttons = machptr->nbuttons;
        bool *lights = machptr->lights;
        for (int l = 0; l < nlights; l++)
            elm->lights[l] = false;
        while (true) {
            bool found = true;
            for (int l = 0; l < nlights; l++)
                if (elm->lights[l] != lights[l]) {
                    found = false;
                    break;
                }
            if (found) { sum += elm->npresses; break; }

            for (int b = 0; b < nbuttons; b++) {
                Button *btnptr = machptr->buttons + b;
                QueueElm *newelm = malloc(sizeof(QueueElm));
                newelm->npresses = elm->npresses + 1;
                for (int l = 0; l < nlights; l++)
                    newelm->lights[l] = elm->lights[l];
                for (int tl = 0; tl < btnptr->nlights; tl++) {
                    int l = btnptr->lights[tl];
                    newelm->lights[l] = !newelm->lights[l];
                }
                deque_push_tail(queue, newelm);
            }
            free(elm);
            elm = deque_pop_head(queue);
        }
    }

    printf("%i\n", sum);

    exit(0);
}

