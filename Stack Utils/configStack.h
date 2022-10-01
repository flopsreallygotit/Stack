#ifndef CONFIGSTACK
#define CONFIGSTACK

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define DEBUG

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Stack element type.
typedef int elem_t;

/// @brief Stack poison element type.
const elem_t Poison = 0x666DED;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef DEBUG

int calculateMaxLength (void);
unsigned long long canaryGenerate (void);

/// @brief Canary type for stack.
typedef unsigned long long canary_t;

const canary_t Canary1 = canaryGenerate();
const canary_t Canary2 = -1;
const canary_t Canary3 = -1;
const canary_t Canary4 = canaryGenerate();

/// @brief Stack struct.
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

    // TODO add printer func void *(printer_func(elem_t));

    canary_t rightCanary;
} stack;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif