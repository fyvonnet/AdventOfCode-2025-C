#include <stdlib.h>
#include "redblacktree.h"



static redblacktree_node *create_node(redblacktree *T, void *data)
{
    redblacktree_node *z = malloc(sizeof(redblacktree_node));
    z->left = z->right = z->p = T->nil;
    z->data = data;

    return z;
}



extern redblacktree *redblacktree_init(int (*compar)(const void*, const void*))
{
    redblacktree *T = malloc(sizeof(redblacktree));
    redblacktree_node *nil = create_node(T, NULL);

    nil->color = NODE_COLOR_BLACK;
    //nil->data = -42;
    T->compar = compar;
    T->root = T->nil = nil;
    T->nodecount = 0;

    return T;
}



static redblacktree_node *NODE_COLOR_search(redblacktree *T, void *data)
{
    redblacktree_node *x = T->root;
    int c;

    while ((x != T->nil) && ((c = T->compar(data, x->data)) != 0)) {
        if (c < 0)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}



extern void *redblacktree_query(redblacktree *T, void *data)
{
    redblacktree_node *n = NODE_COLOR_search(T, data);

    if (n != T->nil) return n->data;
    else return NULL;
}



static void inorder_rec(redblacktree *T, redblacktree_node *x, void (*func)(void *, void *), void *ptr)
{
    if (x != T->nil) {
        inorder_rec(T, x->left, func, ptr);
        func(x->data, ptr);
        inorder_rec(T, x->right, func, ptr);
    }
}



extern void redblacktree_inorder(redblacktree *T, void (*func)(void *, void *), void *ptr)
{
    inorder_rec(T, T->root, func, ptr);
}



static redblacktree_node *tree_minimum(redblacktree_node *nil, redblacktree_node *x)
{
    while (x->left != nil)
        x = x->left;

    return x;
}



extern int redblacktree_nodecount(redblacktree *T)
{
    return T->nodecount;
}



static void rb_delete(redblacktree *T, redblacktree_node *z);
extern void *redblacktree_pop_leftmost(redblacktree *T)
{
    if (T->root == T->nil)
        return NULL;

    redblacktree_node *leftmost_node = tree_minimum(T->nil, T->root);
    void *leftmost_data = leftmost_node->data;

    rb_delete(T, leftmost_node);

    return leftmost_data;
}



static void right_rotate(redblacktree *T, redblacktree_node *x)
{
    redblacktree_node *y = x->left;
    x->left = y->right;

    if (y->right != T->nil)
        y->right->p = x;

    y->p = x->p;

    if (x->p == T->nil)
        T->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right= x;
    x->p = y;
}



static void left_rotate(redblacktree *T, redblacktree_node *x)
{
    redblacktree_node *y = x->right;
    x->right = y->left;

    if (y->left != T->nil)
        y->left->p = x;

    y->p = x->p;

    if (x->p == T->nil)
        T->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;
}



//////////////
// DELETION //
//////////////



static void rb_transplant(redblacktree *T, redblacktree_node *u, redblacktree_node *v)
{
    if (u->p == T->nil)
        T->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;

    v->p = u->p;
}



static void rb_delete_fixup(redblacktree *T, redblacktree_node *x)
{
    redblacktree_node *w;
    while ((x != T->root) && (x->color == NODE_COLOR_BLACK)) {
        if (x == x->p->left) {
            w = x->p->right;
            if (w->color == NODE_COLOR_RED) {
                w->color = NODE_COLOR_BLACK;
                x->p->color = NODE_COLOR_RED;
                left_rotate(T, x->p);
                w = x->p->right;
            }
            if ((w->left->color == NODE_COLOR_BLACK) && (w->right->color == NODE_COLOR_BLACK)) {
                w->color = NODE_COLOR_RED;
                x = x->p;
            }
            else {
                if (w->right->color == NODE_COLOR_BLACK) {
                    w->left->color = NODE_COLOR_BLACK;
                    w->color = NODE_COLOR_RED;
                    right_rotate(T, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = NODE_COLOR_BLACK;
                w->right->color = NODE_COLOR_BLACK;
                left_rotate(T, x->p);
                x = T->root;
            }
        }
        else {
            w = x->p->left;
            if (w->color == NODE_COLOR_RED) {
                w->color = NODE_COLOR_BLACK;
                x->p->color = NODE_COLOR_RED;
                right_rotate(T, x->p);
                w = x->p->left;
            }
            if ((w->right->color == NODE_COLOR_BLACK) && (w->left->color == NODE_COLOR_BLACK)) {
                w->color = NODE_COLOR_RED;
                x = x->p;
            }
            else {
                if (w->left->color == NODE_COLOR_BLACK) {
                    w->right->color = NODE_COLOR_BLACK;
                    w->color = NODE_COLOR_RED;
                    left_rotate(T, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = NODE_COLOR_BLACK;
                w->left->color = NODE_COLOR_BLACK;
                right_rotate(T, x->p);
                x = T->root;
            }
        }
    }
    x->color = NODE_COLOR_BLACK;
}



static void rb_delete(redblacktree *T, redblacktree_node *z)
{
    redblacktree_node *x, *y = z;
    enum redblacktree_node_color y_original_color = y->color;

    T->nodecount--;

    if (z->left == T->nil) {
        x = z->right;
        rb_transplant(T, z, z->right);
    }
    else if (z->right == T->nil) {
        x = z->left;
        rb_transplant(T, z, z->left);
    }
    else {
        y = tree_minimum(T->nil, z->right);
        y_original_color = y->color;
        x = y->right;


        if (y != z->right) {
            rb_transplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        else
            x->p = y;

        rb_transplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if (y_original_color == NODE_COLOR_BLACK)
        rb_delete_fixup(T, x);

    free(z);
}



extern void *redblacktree_delete(redblacktree *T, void *data)
{
    redblacktree_node *n = NODE_COLOR_search(T, data);

    if (n == T->nil) return NULL;

    rb_delete(T, n);

    return n;
}



///////////////
// INSERTION //
///////////////


static void rb_insert_fixup(redblacktree *T, redblacktree_node *z)
{
    redblacktree_node *y;

    while (z->p->color == NODE_COLOR_RED) {
        if (z->p == z->p->p->left) {
            y = z->p->p->right;
            if (y->color == NODE_COLOR_RED) {
                z->p->color = NODE_COLOR_BLACK;
                y->color = NODE_COLOR_BLACK;
                z->p->p->color = NODE_COLOR_RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->right) {
                    z = z->p;
                    left_rotate(T, z);
                }
                z->p->color = NODE_COLOR_BLACK;
                z->p->p->color = NODE_COLOR_RED;
                right_rotate(T, z->p->p);
            }
        }
        else {
            y = z->p->p->left;
            if (y->color == NODE_COLOR_RED) {
                z->p->color = NODE_COLOR_BLACK;
                y->color = NODE_COLOR_BLACK;
                z->p->p->color = NODE_COLOR_RED;
                z = z->p->p;
            }
            else {
                if (z == z->p->left) {
                    z = z->p;
                    right_rotate(T, z);
                }
                z->p->color = NODE_COLOR_BLACK;
                z->p->p->color = NODE_COLOR_RED;
                left_rotate(T, z->p->p);
            }
        }
    }
    T->root->color = NODE_COLOR_BLACK;
}



static void *rb_insert(redblacktree *T, redblacktree_node *z)
{
    redblacktree_node *y = T->nil;
    redblacktree_node *x = T->root;
    int c = 0;

    while (x != T-> nil) {
        y = x;
        c = T->compar(z->data, x->data);
        if (c < 0)
            x = x->left;
        else if (c > 0)
            x = x->right;
        else
            return x->data;
    }

    z->p = y;
    T->nodecount++;

    if (y == T->nil)
        T->root = z;
    else if (c < 0)
        y->left = z;
    else
        y->right = z;

    z->color = NODE_COLOR_RED;

    rb_insert_fixup(T, z);

    return NULL;
}



extern void *redblacktree_insert(redblacktree *T, void *data)
{
    redblacktree_node *z = create_node(T, data);
    void *r = rb_insert(T, z);

    if (r) free(z);
    return r;
}



extern void *redblacktree_insert_or_free(redblacktree *T, void *data)
{
    redblacktree_node *z = create_node(T, data);
    void *r = rb_insert(T, z);

    if (r) {
        free(z);
        free(data);
        return r;
    }
    else 
        return data;
}

