#ifndef CONFIGSTACK
#define CONFIGSTACK

#define DEBUG

/// @brief Stack element type.
typedef int elem_t;

/// @brief Stack poison element type.
const elem_t Poison = 0x600DDED;

#ifdef DEBUG

/// @brief Canary type for stack.
typedef unsigned long long canary_t;
const canary_t Canary1 = -1;
const canary_t Canary2 = (Canary1 * 10 - Canary1 % 10) * 10;
const canary_t Canary3 = (Canary2 * 10 - Canary2 % 10) * 10;

/// @brief Stack struct.
typedef struct stack
{
    canary_t leftCanary; 

    elem_t *data;
    int    *isPoison;

    canary_t middleCanary;

    size_t  capacity;
    size_t  size;

    size_t  currentSum;

    size_t      birthLine;
    const char *birthFile;

    canary_t rightCanary;
} stack;

#else

/// @brief Stack struct.
typedef struct stack
{
    elem_t *data;
    int    *isPoison;

    size_t  capacity;
    size_t  size;
} stack;

#endif

#endif