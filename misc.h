#pragma once

char *myfgets(char * restrict s, int size, FILE *stream);
int modulo(int a, int b);
void *binsearch(void *base, int nmemb, int size, int (*compar)(const void *, const void *), void *target);

