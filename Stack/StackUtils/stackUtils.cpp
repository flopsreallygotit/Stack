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
} stack_;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackVerifier (stack *stk)
{
    if (stk->capacity && stk->data)
    {
        if (stk->size > stk->capacity)
            dumpAndReturn(stk, WRONGSIZE);

        if (stk->capacity <= 0 || stk->size < 0)
            dumpAndReturn(stk, WRONGSIZE);

        #ifdef HASH
        if ((stk->currentSum) != hashCalculate((char *) stk->data, 
                   (stk->capacity) * sizeof(elem_t)))
            dumpAndReturn(stk, WRONGSUM);
        #endif

        #ifdef BIRTHINFO
        if (stk->birthFile == NULL)
            dumpAndReturn(stk, NULLPOINTER);
        #endif

        #ifdef STRUCTCANARY
        if (stk->leftCanary  != Canary1)
            dumpAndReturn(stk, LEFTCANARY);

        if (stk->rightCanary != Canary2)
            dumpAndReturn(stk, RIGHTCANARY);
        #endif

        #ifdef DATACANARY
        movePointerOnCanaryCount(&(stk->data), -1);
        if (*((canary_t *) stk->data) != Canary3)
            dumpAndReturn(stk, DATALEFTCANARY);

        movePointerOnCanaryCount(&(stk->data), 1);
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

#ifdef HASH
size_t hashCalculate (char *dataStructure, ssize_t dataStructureSize)
{
    size_t controlSum = 5381;
    for (ssize_t idx = 1; idx < dataStructureSize; idx++)
        controlSum += idx * dataStructure[idx];

    return (((controlSum << 5) + controlSum) + 1);
}
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef DATACANARY
ISERROR movePointerOnCanaryCount (elem_t **data, int canaryCount)
{
    CHECKERROR(data, NULLPOINTER);

    canary_t *newData = (canary_t *) *data;
    newData += canaryCount;
    *data = (elem_t *) newData;

    return NOTERROR;
}

ISERROR saveCanary (canary_t canary, canary_t *canaryPointer)
{
    CHECKERROR(canaryPointer, NULLPOINTER);

    *canaryPointer = canary;

    return NOTERROR;
}
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR simpleStackDump(stack *stk)
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

        for (ssize_t idx = 0; idx < stk->capacity; idx++)
        {
            printf("    [%lu]:  ", idx);
            elementOutput(stk->data[idx]);
            if (stk->isPoison[idx])
                printf(BOLD RED " (Poison)" RESET);

            putchar('\n');
        }
    }

    else
        printf("    Stack have zero elements.\n");

    printf("};\n\n");

    return NOTERROR;
}

ISERROR stackDumpFunction (stack *stk, const char *stkName, 
                           const char *file, int line)
{
    CHECKERROR(stk, NULLPOINTER);

    ISERROR ERROR = stackVerifier(stk);
    
    printf(BOLD MAGENTA "Stack Dump at %s:%d\n" RESET BOLD "Stack %s[%p] " RESET, 
           file, line, stkName + 1, stk);

    #ifdef BIRTHINFO
    printf(BOLD "Birth at %s:%ld) - " RESET, stk->birthFile, stk->birthLine);
    #endif
    
    if (ERROR == NOTERROR)
        printf(BOLD GREEN "(OK)\n" RESET);
    else
        printf(BOLD RED "(ERROR: %x)\n" RESET, (int) ERROR);

    simpleStackDump(stk);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stack *stackConstructorFunction (ssize_t capacity, const char *file, size_t line)
{
    stack *stk = (stack *) calloc (1, sizeof(stack));

    if (capacity < 0)
        return NULL;

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

    saveCanary(Canary3, (canary_t *) stk->data);

    movePointerOnCanaryCount(&(stk->data), 1);

    stk->data += capacity;
    saveCanary (Canary4, (canary_t *) stk->data);
    stk->data -= capacity;

    #else
    stk->data = (elem_t *) calloc (stk->capacity, sizeof(elem_t));
    #endif

    stk->isPoison = (int *) calloc (stk->capacity, sizeof(int));

    for (ssize_t idx = 0; idx < stk->capacity; idx++)
    {
        stk->data[idx] = Poison;
        stk->isPoison[idx] = 1;     
    }

    #ifdef STRUCTCANARY
    stk->leftCanary  = Canary1;
    stk->rightCanary = Canary2;
    #endif
    
    #ifdef HASH
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    #endif

    #ifdef BIRTHINFO
    stk->birthLine = line;
    stk->birthFile = file;
    #endif

    return stk;
}

void stackDestructor (stack *stk)
{
    if (stk)
        return;

    if (stackVerifier(stk) != NOTERROR)
        return;

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

    free(stk->data);
    free(stk->isPoison);

    stk->data        = NULL;
    stk->isPoison    = NULL;

    free(stk);
    stk = NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackPushResize (stack *stk)
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

    stk->isPoison = (int *) recalloc (stk->isPoison, (stk->capacity) * sizeof(int));

    for (ssize_t idx = stk->size; idx < stk->capacity; idx++)
    {
        stk->data[idx] = Poison; 
        stk->isPoison[idx] = 1;
    }

    return NOTERROR;
}

ISERROR stackPush (stack *stk, elem_t element)
{
    CHECKERROR(stk,                            NULLPOINTER);
    CHECKERROR(isfinite(element),              ISNOTFINITE);
    CHECKERROR(stackVerifier(stk) == NOTERROR, WRONGSTACK );

    stackPushResize(stk);

    stk->data[stk->size] = element;
    stk->isPoison[stk->size] = 0;

    #ifdef HASH
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    #endif

    CHECKERROR(stackVerifier(stk) == NOTERROR, WRONGSTACK);

    stk->size++;

    return NOTERROR;
}

ISERROR stackPopResize (stack *stk)
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

    stk->isPoison = (int *) recalloc (stk->isPoison, sizeof(int) * (stk->capacity));

    return NOTERROR;
}

ISERROR stackPop (stack *stk, elem_t *element)
{
    CHECKERROR(stk,                            NULLPOINTER);
    CHECKERROR(stk->size > 0,                  POPOUTEMPTY);
    CHECKERROR(stackVerifier(stk) == NOTERROR, WRONGSTACK );
   
    stk->size--;

    if (element)
        *element = stk->data[stk->size];

    stk->data[stk->size] = Poison;
    stk->isPoison[stk->size] = 1;
    stackPopResize(stk);

    #ifdef HASH
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    #endif

    CHECKERROR(stackVerifier(stk) == NOTERROR, WRONGSTACK);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void elementOutput (const int element)
{
    printf("%d", element);
    return;
}

void elementOutput (const double element)
{
    printf("%lf", element);
    return;
}

void elementOutput (const char element)
{
    printf("%c", element);
    return;
}

void elementOutput (const long element)
{
    printf("%ld", element);
    return;
}

void elementOutput (const short element) 
{
    printf("%hd", element);
    return;
}

void elementOutput (const void *element) 
{
    const elem_t Element = *(const elem_t *) element;
    printf("%x", Element);
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
