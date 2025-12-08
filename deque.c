#include <stdlib.h>
#include "deque.h"



extern deque *deque_create()
{
    deque *d = (deque *)malloc(sizeof(deque));
    d->head = d->tail = NULL;
    return d;
}



extern void* deque_peek_head(deque *d)
{
    if (d->head == NULL)
        return NULL;

    return d->head->data;
}



extern void* deque_pop_head(deque *d)
{
    if (d->head == NULL)
        return NULL;

    deque_node *n = d->head;
    void *v = d->head->data;
    d->head = d->head->next;

    if (d->head == NULL)
        d->tail = NULL;

    free(n);
    return v;
}



extern void deque_push_head(deque *d, void *v)
{
    deque_node *n = (deque_node *)malloc(sizeof(deque_node));
    n->data = v;
    n->next = NULL;

    if (d->head == NULL)
        d->head = d->tail = n;
    else {
        n->next = d->head;
        d->head = n;
    }
}



extern void deque_push_tail(deque *d, void *v)
{
    deque_node *n = (deque_node *)malloc(sizeof(deque_node));
    n->data = v;
    n->next = NULL;

    if (d->head == NULL)
        d->head = d->tail = n;
    else {
        deque_node *p = d->tail;
        d->tail = n;
        p->next = n;
    }
}

