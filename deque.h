#pragma once

struct deque_node {
    void *data;
    struct deque_node *next;
};
typedef struct deque_node deque_node;

struct deque {
    struct deque_node *head, *tail;
};
typedef struct deque deque;

deque *deque_create();
void* deque_pop_head(deque *d);
void* deque_peek_head(deque *d);
void deque_push_head(deque *d, void *v);
void deque_push_tail(deque *d, void *v);

