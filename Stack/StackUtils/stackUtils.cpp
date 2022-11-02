#include <math.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "stackUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct stack_
{
    #ifdef STRUCTCANARY

    canary_t leftCanary;

    #endif

    elem_t *data;

    size_t  capacity;
    size_t  size;

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
}stack_;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef DATACANARY

static ISERROR movePointerOnCanaryCount (elem_t **data, int canaryCount)
{
    CHECKERROR(data, NULLPOINTER);

    canary_t *newData = (canary_t *) *data;
    newData += canaryCount;
    *data = (elem_t *) newData;

    return NOTERROR;
}

static ISERROR saveCanary (canary_t canary, canary_t *canaryPointer)
{
    CHECKERROR(canaryPointer, NULLPOINTER);

    *canaryPointer = canary;

    return NOTERROR;
}

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef HASH

static size_t hashCalculate (stack *stk)
{
    size_t controlSum = 5381;

    for (size_t idx = 1; idx < stk->capacity; idx++)
        controlSum += idx * (stk->data[idx]);

    return (((controlSum << 5) + controlSum) + 1);
}

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define dumpAndReturn(stkPointer, returnValue) \
{                                              \
    simpleStackDump(stkPointer);               \
    return returnValue;                        \
}

static ISERROR stackVerifier (stack *stk)
{
    if (stk->capacity && stk->data)
    {
        if (stk->size > stk->capacity)
            dumpAndReturn(stk, WRONGSIZE);

        #ifdef MAXCAPACITY
        if (stk->capacity >= maxCapacity)
            dumpAndReturn(stk, WRONGSIZE);
        #endif

        #ifdef HASH

        if ((stk->currentSum) != hashCalculate(stk))
            dumpAndReturn(stk, WRONGSUM);

        #endif

        #ifdef BIRTHINFO

        if (stk->birthFile == NULL)
            dumpAndReturn(stk, NULLPOINTER);

        #endif

        #ifdef STRUCTCANARY

        if (stk->leftCanary  != Canary1)
            dumpAndReturn(stk, LEFTCANARY );

        if (stk->rightCanary != Canary2)
            dumpAndReturn(stk, RIGHTCANARY);

        #endif

        #ifdef DATACANARY

        movePointerOnCanaryCount(&(stk->data), -1);

        if (*((canary_t *) stk->data) != Canary3)
            dumpAndReturn(stk, DATALEFTCANARY );

        movePointerOnCanaryCount(&(stk->data),  1);

        stk->data += stk->capacity;

        if (*((canary_t *) stk->data) != Canary4)
            dumpAndReturn(stk, DATARIGHTCANARY);

        stk->data -= stk->capacity;

        #endif
    }

    else if (stk->capacity)
        return WRONGSTACK;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void elementOutput (const int element)
{
    printf("%d", element);
    return;
}

static void elementOutput (const double element)
{
    printf("%lg", element);
    return;
}

static void elementOutput (const char *element)
{
    printf("%s", element);
    return;
}

static void elementOutput (const char element)
{
    printf("%c", element);
    return;
}

static void elementOutput (const long element)
{
    printf("%ld", element);
    return;
}

static void elementOutput (const short element) 
{
    printf("%hd", element);
    return;
}

static void elementOutput (const void *element) 
{
    const elem_t Element = *(const elem_t *) element;
    printf("%x", Element);
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR simpleStackDump (stack *stk)
{
    CHECKERROR(stk, NULLPOINTER);

    printf("{\n" BOLD "    Capacity: %lu\n    First free index: %lu\n" RESET,
           stk->capacity, stk->size);

    #ifdef HASH

    printf(BOLD "    Current sum:  %ld\n" RESET, stk->currentSum);

    #endif

    #ifdef STRUCTCANARY

    printf(BOLD "    Left canary:  %llx\n    Right canary: %llx\n" RESET, 
           stk->leftCanary, stk->rightCanary);

    #endif

    if (stk->size > 0)
    {
        #ifdef DATACANARY

        movePointerOnCanaryCount(&(stk->data), -1);

        printf(BOLD "    Left data canary:  %llx\n" RESET, *(canary_t *) stk->data);

        movePointerOnCanaryCount(&(stk->data),  1);

        stk->data += stk->capacity;
        printf(BOLD "    Right data canary: %llx\n" RESET, *(canary_t *) stk->data);
        stk->data -= stk->capacity;

        #endif

        for (size_t idx = 0; idx < stk->capacity; idx++)
        {
            printf("    [%lu]:  ", idx);

            elementOutput(stk->data[idx]);

            if (idx >= stk->size)
                printf(BOLD RED " (Poison)" RESET);

            putchar('\n');
        }
    }

    else
        printf("    Stack have zero elements.\n");

    printf("};\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackDumpFunction (stack *stk, const char *stkName, 
                           const char *file, int line)
{
    CHECKERROR(stk, NULLPOINTER);

    ISERROR ERROR = stackVerifier(stk);
    
    printf(BOLD MAGENTA "\nStack Dump at %s:%d\n" RESET BOLD "Stack %s[%p] " RESET, 
           file, line, stkName + 1, stk);

    #ifdef BIRTHINFO

    printf(BOLD "Birth at %s:%ld) - " RESET, stk->birthFile, stk->birthLine);

    #endif
    
    if (ERROR == NOTERROR)
        printf(BOLD GREEN "(OK)\n" RESET);

    else
        printf(BOLD RED "(ERROR: %x)\n" RESET, (int) ERROR);

    simpleStackDump(stk);

    return ERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stack *stackConstructorFunction (size_t capacity, const char *file, size_t line)
{
    stack *stk = (stack *) calloc (1, sizeof(stack));

    if (stk == NULL)
    {
        PUTERROR("Can't allocate memory for stack.");
        return NULL;
    }

    #ifdef MAXCAPACITY

    if (capacity <= maxCapacity)
        stk->capacity = capacity;
    else
        return NULL;

    #else

    stk->capacity = capacity;

    #endif

    stk->size = 0;

    #ifdef DATACANARY

    stk->data = (elem_t *) recalloc (stk->data, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    if (stk->data == NULL)
    {
        PUTERROR("Can't allocate memory for stack data.");
        free(stk);

        return NULL;
    }

    saveCanary(Canary3, (canary_t *) stk->data);

    movePointerOnCanaryCount(&(stk->data), 1);

    stk->data += capacity;
    saveCanary (Canary4, (canary_t *) stk->data);
    stk->data -= capacity;

    #else

    stk->data = (elem_t *) calloc (stk->capacity, sizeof(elem_t));

    if (stk->data == NULL)
    {
        PUTERROR("Can't allocate memory for stack data.");
        free(stk);

        return NULL;
    }

    #endif

    for (size_t idx = 0; idx < stk->capacity; idx++)
        stk->data[idx] = Poison;
    
    #ifdef STRUCTCANARY

    stk->leftCanary  = Canary1;
    stk->rightCanary = Canary2;

    #endif
    
    #ifdef HASH

    stk->currentSum = hashCalculate(stk);

    #endif

    #ifdef BIRTHINFO

    stk->birthLine = line;
    stk->birthFile = file;

    #endif

    return stk;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void stackDestructor (stack *stk)
{
    if (stk == NULL)
    {
        PUTWARNING("You are trying to destruct nullpointer.");
        
        return;
    }

    if (stackVerifier(stk) != NOTERROR)
    {
        PUTERROR("Can't destruct stack because it's wrong.");

        return;
    }

    stk->capacity    = 0;
    stk->size        = 0;

    #ifdef HASH

    stk->currentSum  = 0;

    #endif

    #ifdef DATACANARY

    movePointerOnCanaryCount(&(stk->data), -1);

    #endif

    #ifdef STRUCTCANARY

    stk->leftCanary  = 0;
    stk->rightCanary = 0;

    #endif

    #ifdef BIRTHINFO

    stk->birthFile = NULL;
    stk->birthLine = 0;

    #endif

    free(stk->data);
    stk->data = NULL;

    free(stk);
    stk = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR stackPushResize (stack *stk)
{
    CHECKERROR(stk, NULLPOINTER);

    if (stk->size < stk->capacity)
        return NOTERROR;

    if (stk->capacity)
        stk->capacity *= 2;
    else
        stk->capacity += 1;

    #ifdef DATACANARY

    movePointerOnCanaryCount(&(stk->data), -1);

    stk->data = (elem_t *) recalloc (stk->data, (stk->capacity) * sizeof(elem_t) + 2 * sizeof(canary_t));

    movePointerOnCanaryCount(&(stk->data),  1);

    stk->data += stk->capacity;
    saveCanary(Canary4, (canary_t *) stk->data);
    stk->data -= stk->capacity;

    #else

    stk->data = (elem_t *) recalloc (stk->data, (stk->capacity) * sizeof(elem_t));

    #endif

    for (size_t idx = stk->size; idx < stk->capacity; idx++)
        stk->data[idx] = Poison; 
    
    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackPush (stack *stk, elem_t element)
{
    CHECKERROR(stk,                            NULLPOINTER);
    CHECKERROR(isfinite(element),              ISNOTFINITE);
    CHECKERROR(stackVerifier(stk) == NOTERROR, WRONGSTACK );

    stackPushResize(stk);

    stk->data[stk->size] = element;

    #ifdef HASH

    stk->currentSum = hashCalculate(stk);

    #endif

    stk->size++;

    return stackVerifier(stk);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR stackPopResize (stack *stk)
{
    if (stk->size > (stk->capacity) / 4)
        return NOTERROR;

    stk->capacity /= 2;

    #ifdef DATACANARY

    movePointerOnCanaryCount(&(stk->data), -1);

    stk->data = (elem_t *) recalloc (stk->data, sizeof(elem_t) * (stk->capacity) + 2 * sizeof(canary_t));

    movePointerOnCanaryCount(&(stk->data),  1);

    stk->data += stk->capacity;
    saveCanary(Canary4, (canary_t *) stk->data);
    stk->data -= stk->capacity;

    #else

    stk->data = (elem_t *) recalloc (stk->data, sizeof(elem_t) * (stk->capacity));

    #endif

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackPop (stack *stk, elem_t *element)
{
    CHECKERROR(stk,                            NULLPOINTER);
    CHECKERROR(stk->size > 0,                  POPOUTEMPTY);
    CHECKERROR(stackVerifier(stk) == NOTERROR, WRONGSTACK );
   
    stk->size--;

    if (element)
        *element = stk->data[stk->size];

    stk->data[stk->size] = Poison;
    stackPopResize(stk);

    #ifdef HASH

    stk->currentSum = hashCalculate(stk);

    #endif

    return stackVerifier(stk);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
