#include <stdio.h>
#include <string.h>

char *myfgets(char * restrict s, int size, FILE *stream)
{
    *s = '\0';
    char* r = fgets(s, size, stream);
    int l = strlen(s);

    if ((l > 0) && (s[l - 1] == '\n'))
        s[l - 1] = '\0';

    return r;
}

int modulo(int a, int b)
{
    return ((a % b) + b) % b;
}

void *binsearch(void *base, int nmemb, int size, int (*compar)(const void *, const void *), void *target)
{
    int l = 0;
    int r = nmemb - 1;

    while (l <= r) {
        int m = l + ((r - l) >> 1);
        void *ptr = base + (m * size);
        int diff = compar(ptr, target);
        if (diff < 0)
            l = m + 1;
        else if (diff > 0)
            r = m - 1;
        else
            return ptr;
    }
    return NULL;
}

