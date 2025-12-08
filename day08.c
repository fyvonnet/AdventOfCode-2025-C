#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "misc.h"
#include "deque.h"
#include "redblacktree.h"

#define BUFLEN      30
#define MAXNBOXES   1000
#define NPAIRS      1000

typedef struct ConnElm ConnElm;
typedef struct { int x, y, z; ConnElm *conns; bool visited; } Box;
typedef struct { Box *a, *b; double dist; } Pair;
struct ConnElm { Box *box; ConnElm *next; };

double distance(Box *a, Box *b);
int compar_pairs(const void *a, const void *b);
int compar_ptrs(const void *a, const void *b);
int compar_counts_inv(const void *a, const void *b);
void listclosestboxes(void *box, void *ptr);
redblacktree *findset(redblacktree **circuits, Box *box);



int main()
{
    Box boxes[MAXNBOXES];
    char buffer[BUFLEN];
    FILE *fp = fopen("inputs/day08", "r");
    if (!fp) { perror("fopen"); exit(1); }

    char *fgetsret = myfgets(buffer, BUFLEN, fp);
    int box;
    for (box = 0; fgetsret != NULL; box++) {
        Box *ptr = boxes + box;
        char *token = strtok(buffer, ",");
        ptr->x = atoi(token);
        token = strtok(NULL, ",");
        ptr->y = atoi(token);
        token = strtok(NULL, ",");
        ptr->z = atoi(token);
        fgetsret = myfgets(buffer, BUFLEN, fp);
        ptr->conns = NULL;
        ptr->visited = false;
    }

    int nboxes = box;
    int npairs = (nboxes * (nboxes - 1)) / 2;
    Pair *pairs = malloc(npairs * sizeof(Pair));

    Pair *ptr = pairs;
    for (int a = 0; a < nboxes - 1; a++)
        for (int b = a + 1; b < nboxes; b++) {
            ptr->a = boxes + a;
            ptr->b = boxes + b;
            ptr->dist = distance(ptr->a, ptr->b);
            ptr++;
        }

    qsort(pairs, npairs, sizeof(Pair), compar_pairs);

    redblacktree *closestboxesset = redblacktree_init(compar_ptrs);
    for (int p = 0; p < NPAIRS; p++) {
        Box *ptra = pairs[p].a;
        Box *ptrb = pairs[p].b;
        redblacktree_insert(closestboxesset, ptra);
        redblacktree_insert(closestboxesset, ptrb);
        ConnElm *elm;
        elm = malloc(sizeof(ConnElm));
        elm->next = ptra->conns;
        elm->box = ptrb;
        ptra->conns = elm;
        elm = malloc(sizeof(ConnElm));
        elm->next = ptrb->conns;
        elm->box = ptra;
        ptrb->conns = elm;
    }

    deque *closestboxes = deque_create();
    redblacktree_inorder(closestboxesset, listclosestboxes, closestboxes);
    Box *boxptr = (Box *)deque_pop_head(closestboxes);
    deque *queue = deque_create();
    redblacktree *circuits[nboxes];
    int ncircuits = 0;
    while (boxptr != NULL) {
        if (!boxptr->visited) {
            circuits[ncircuits] = redblacktree_init(compar_ptrs);
            boxptr->visited = true;
            deque_push_tail(queue, boxptr);
            while ((boxptr = deque_pop_head(queue)) != NULL) {
                redblacktree_insert(circuits[ncircuits], boxptr);
                for (ConnElm *conn = boxptr->conns; conn != NULL; conn = conn->next) {
                    boxptr = conn->box;
                    if (!boxptr->visited) {
                        boxptr->visited = true;
                        deque_push_tail(queue, boxptr);
                    }
                }
            }
            ncircuits++;
        }
        boxptr = (Box *)deque_pop_head(closestboxes);
    }

    qsort(circuits, ncircuits, sizeof(redblacktree *), compar_counts_inv);

    int product = 1;
    for (int c = 0; c < 3; c++) {
        product *= circuits[c]->nodecount;
    }
    printf("%i\n", product);

    for (int box = 0; box < nboxes; box++) {
        Box *boxptr = boxes + box;
        if (redblacktree_query(closestboxesset, boxptr) == NULL) {
            circuits[ncircuits] = redblacktree_init(compar_ptrs);
            redblacktree_insert(circuits[ncircuits], boxptr);
            ncircuits++;
        }
    }

    int xa = 0, xb = 0;
    for (int p = NPAIRS; p < npairs; p++) {
        redblacktree *circa = findset(circuits, pairs[p].a);
        redblacktree *circb = findset(circuits, pairs[p].b);
        if (circa != circb) {
            xa = pairs[p].a->x;
            xb = pairs[p].b->x;
            ncircuits--;
            if (ncircuits == 1) break;
            Box *box = redblacktree_pop_leftmost(circb);
            while (box != NULL) {
                redblacktree_insert(circa, box);
                box = redblacktree_pop_leftmost(circb);
            }
        }
    }

    printf("%i\n", xa * xb);

    exit(0);
}



double distance(Box *a, Box *b)
{
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2) + pow(a->z - b->z, 2));
}



int compar_pairs(const void *a, const void *b)
{
    Pair *aa = (Pair *)a;
    Pair *bb = (Pair *)b;

    if (aa->dist > bb->dist) return 1;
    else return -1;
}



int compar_ptrs(const void *a, const void *b)
{
    if (a > b) return 1;
    else if (a < b) return -1;
    else return 0;
}



int compar_counts_inv(const void *a, const void *b)
{
    redblacktree *aa = *(redblacktree **)a;
    redblacktree *bb = *(redblacktree **)b;
    return bb->nodecount - aa->nodecount;
}



void listclosestboxes(void *box, void *ptr)
{
    deque *closestboxes = (deque *)ptr;
    deque_push_tail(closestboxes, box);
}



redblacktree *findset(redblacktree **circuits, Box *box)
{
    int c;
    for (c = 0; redblacktree_query(circuits[c], box) == NULL; c++);
    return circuits[c];
}

