#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "misc.h"
#include "redblacktree.h"

#define BUFLEN          250


int ncounters;
int nbuttons;
int *buttons[16];
redblacktree *cache;
typedef struct { int array[16]; int value; } cache_elm;



int parse_csv(char *string, int dest[16]);
int compar_arrays(int length, int *array1, int *array2);
int compar_cache_elm(const void *a, const void *b);
int recurse(int counters[16]);



int main()
{
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day10", "r");
    if (!fp) { perror("fopen"); exit(1); }
    int sum1 = 0;
    int sum2 = 0;

    int buttons_targets[16 * 16];
    for (int i = 0; i < 16; i++)
        buttons[i] = buttons_targets + (i * 16);

    while ( myfgets(buffer, BUFLEN, fp)) {
        char *saveptr;
        char *token = strtok_r(buffer, " ", &saveptr);
        cache = redblacktree_init(compar_cache_elm);

        cache_elm *zero = malloc(sizeof(cache_elm));
        for (int i = 0; i < ncounters; i++)
            zero->array[i] = 0;
        zero->value = 0;

        int nlights = 0;
        int lights[16];
        token++;
        while (*token != ']')
            lights[nlights++] = (*(token++) == '#');

        nbuttons = 0;
        token = strtok_r(NULL, " ", &saveptr);
        while (*token != '{') {
            int *button = buttons[nbuttons];
            int n = parse_csv(token, button);
            button[n] = -1;
            nbuttons++;
            token = strtok_r(NULL, " ", &saveptr);
        }

        int counters[16];
        ncounters = parse_csv(token, counters);

        int nseqs = 1 << nbuttons;
        int min = nbuttons;
        for (int seq = 1; seq < nseqs; seq++) {
            int mylights[16];
            for (int i = 0; i < nlights; i++)
                mylights[i] = 0;
            int s = seq;
            int npress = 0;
            for (int b = 0; b < nbuttons; b++) { 
                if (s & 1) {
                    int *button = buttons[b];
                    npress++;
                    while (*button != -1) {
                        mylights[*button] = !mylights[*button];
                        button++;
                    }
                }
                s >>= 1;
            }

            if (compar_arrays(nlights, lights, mylights) == 0)
                if (npress < min)
                    min = npress;

        }
        sum1 += min;
        sum2 += recurse(counters);
    }

    printf("Answer to part 1: %i\n", sum1);
    printf("Answer to part 2: %i\n", sum2);

    exit(0);
}



int parse_csv(char *string, int dest[16])
{
    string++;
    int n = 0;
    char *saveptr;
    char *token = strtok_r(string, ",", &saveptr);

    while (token) {
        dest[n++] = atoi(token);
        token = strtok_r(NULL, ",", &saveptr);
    }

    return n;
}



int compar_arrays(int length, int *array1, int *array2)
{
    for (int i = 0; i < length; i++) {
        int n = array1[i] - array2[i];
        if (n) return n;
    }
    return 0;
}



int compar_cache_elm(const void *a, const void *b)
{
    cache_elm *aa = (cache_elm *)a;
    cache_elm *bb = (cache_elm *)b;

    return compar_arrays(ncounters, aa->array, bb->array);
}



int recurse(int counters[16])
{
    cache_elm *ce = malloc(sizeof(cache_elm));
    for (int i = 0; i < ncounters; i++)
        ce->array[i] = counters[i];
    cache_elm *ret = (cache_elm *)redblacktree_insert(cache, ce);

    if (ret) {
        free(ce);
        return ret->value;
    }
    else {
        int min = 1000000;
        int nseqs = 1 << nbuttons;
        for (int seq = 0; seq < nseqs; seq++) {
            int mycounters[16];
            for (int i = 0; i < ncounters; i++)
                mycounters[i] = counters[i];
            int s = seq;
            int npress = 0;
            for (int i = 0; i < nbuttons; i++) {
                if (s & 1) {
                    npress++;
                    int *button = buttons[i];
                    while (*button != -1) {
                        mycounters[*button]--;
                        button++;
                    }
                }
                s >>= 1;
            }

            for (int i = 0; i < ncounters; i++)
                if ((mycounters[i] % 2) != 0)
                    goto skip;

            for (int i = 0; i < ncounters; i++) {
                if (mycounters[i] < 0) goto skip;
                mycounters[i] /= 2;
            }
            int n = recurse(mycounters);
            n = (n * 2) + npress;
            if (n < min) min = n;
skip:
        }
        ce->value = min;

        return min;
    }
}
