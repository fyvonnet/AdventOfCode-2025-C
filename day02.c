#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "misc.h"

#define BUFLEN  500

bool is_valid2(uint64_t n);
bool is_valid(uint64_t n);
uint64_t sum_invalids(uint64_t s, uint64_t e, bool (*func)(uint64_t n));
void parse_token(char *s, uint64_t *a, uint64_t *b);



int main()
{
    FILE *fp = fopen("inputs/day02", "r");
    if (!fp) { perror("fopen"); exit(1); }
    char buffer[BUFLEN];
    myfgets(buffer, BUFLEN, fp);
    fclose(fp);

    uint64_t sum  = 0;
    uint64_t sum2 = 0;
    char *token = strtok(buffer, ",");
    while (token) {
        uint64_t s, e;
        parse_token(token, &s, &e);
        sum  += sum_invalids(s, e, is_valid);
        sum2 += sum_invalids(s, e, is_valid2);
        token = strtok(NULL, ",");
    }

    printf("%" PRIu64 "\n", sum);
    printf("%" PRIu64 "\n", sum2);

    exit(0);
}



uint64_t sum_invalids(uint64_t s, uint64_t e, bool (*func)(uint64_t n))
{
    uint64_t sum = 0;

    for (uint64_t n = s; n <= e; n++)
        if (!func(n)) {
            sum += n;
        }

    return sum;
}



bool is_valid(uint64_t n)
{
    char str[32];
    sprintf(str, "%" PRIu64, n);
    int len = strlen(str);

    // IDs with odd lengths can't be invalid
    if (len & 1)
        return true;

    len >>= 1;

    return (strncmp(str, str + len, len) != 0);
}



bool is_valid2(uint64_t n)
{
    char str[32];
    sprintf(str, "%" PRIu64, n);
    int len = strlen(str);

    for (int div = 1; div < len; div++)
        if ((len % div) == 0) {
            bool invalid = true;
            for (int i = div; i < len; i += div)
                if (strncmp(str, str + i, div) != 0) {
                    invalid = false;
                    break;
                }
            if (invalid) return false;
        }

    return true;
}



void parse_token(char *s, uint64_t *a, uint64_t *b)
{
    *a = atoll(s);
    while(*s != '-') s++;
    s++;
    *b = atoll(s);
}

