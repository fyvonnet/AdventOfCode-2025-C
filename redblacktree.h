#pragma once

typedef struct redblacktree redblacktree;
typedef struct redblacktree_node redblacktree_node;

struct redblacktree {
    redblacktree_node *root, *nil;
    int nodecount;
    int (*compar)(const void*, const void*);
};

enum redblacktree_node_color { NODE_COLOR_RED, NODE_COLOR_BLACK };

struct redblacktree_node {
    redblacktree_node *left, *right, *p;
    enum redblacktree_node_color color;
    void *data;
};

redblacktree *redblacktree_init(int (*compar)(const void*, const void*));
void *redblacktree_query(redblacktree *T, void *data);
void redblacktree_inorder(redblacktree *T, void (*func)(void *, void *), void *ptr);
int redblacktree_nodecount(redblacktree *T);
void *redblacktree_pop_leftmost(redblacktree *T);
void *redblacktree_delete(redblacktree *T, void *data);
void *redblacktree_insert(redblacktree *T, void *data);
void *redblacktree_insert_or_free(redblacktree *T, void *data);

