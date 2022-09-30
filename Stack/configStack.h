#ifndef CONFIGSTACK
#define CONFIGSTACK

typedef int elem_t;
const elem_t Poison = 0x600DDED;

typedef unsigned long long canary_t;
const canary_t Canary1 = -1;
const canary_t Canary2 = (Canary1 * 10 - Canary1 % 10) * 10;

typedef struct stack
{
    canary_t leftCanary;

    elem_t *data;
    int    *isPoison;

    size_t  capacity;
    size_t  size;

    size_t  currentSum;

    size_t      birthLine;
    const char *birthFile;

    canary_t rightCanary;
} stack;

#endif