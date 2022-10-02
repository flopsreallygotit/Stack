#ifndef CONFIGSTACK
#define CONFIGSTACK

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stdio.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define STRUCTCANARY
#define DATACANARY
#define HASH
#define BIRTHINFO

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Stack element type.
typedef int elem_t;

/// @brief Stack poison element type.
const elem_t Poison = 0x666DED;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef unsigned long long canary_t;
int calculateMaxLength (void);
unsigned long long canaryGenerate (void);

#ifdef STRUCTCANARY
const canary_t Canary1 = canaryGenerate();
const canary_t Canary2 = canaryGenerate();
#endif

#ifdef DATACANARY
const canary_t Canary3 = canaryGenerate();
const canary_t Canary4 = canaryGenerate();
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// @brief Stack struct.
typedef struct stack
{
    #ifdef STRUCTCANARY
    canary_t leftCanary;
    #endif

    elem_t *data;
    int    *isPoison;

    ssize_t  capacity;
    ssize_t  size;

    #ifdef HASH
    size_t  currentSum;
    #endif

    #ifdef BIRTHINFO
    size_t      birthLine;
    const char *birthFile;
    #endif

    #ifdef STRUCTCANARY
    canary_t rightCanary;
    #endif
} stack;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
