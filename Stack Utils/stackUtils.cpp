#include "stackUtils.h"
#include "universalUtils.h"
#include <math.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackVerifier (stack *stk)
{
    if (stk->capacity && stk->data)
    {
        if (stk->size > stk->capacity)
        {
            simpleStackDump(stk);
            return WRONGSTACK;
        }
        #ifdef HASH
        if ((stk->currentSum) != hashCalculate((char *) stk->data, 
                   (stk->capacity) * sizeof(elem_t)))
        {
            simpleStackDump(stk);
            return WRONGSUM;
        }
        #endif

        #ifdef BIRTHINFO
        if (stk->birthFile == NULL)
        {
            simpleStackDump(stk);
            return NULLPOINTER;
        }
        #endif

        #ifdef STRUCTCANARY
        if (stk->leftCanary != Canary1)
        {
            simpleStackDump(stk);
            return LEFTCANARY;
        } 

        if (stk->rightCanary != Canary2)
        {
            simpleStackDump(stk);
            return RIGHTCANARY;
        }
        #endif

        #ifdef DATACANARY
        movePointerOnCanaryCount(&(stk->data), -1);
        if (*((canary_t *) stk->data) != Canary3)
        {
            simpleStackDump(stk);
            return DATALEFTCANARY;
        }

        movePointerOnCanaryCount(&(stk->data), 1);
        stk->data += stk->capacity;

        if (*((canary_t *) stk->data) != Canary4)
        {
            simpleStackDump(stk);
            return  DATARIGHTCANARY;
        }

        stk->data -= stk->capacity;
        #endif
    }

    return NOTERROR;
}

#ifdef HASH
size_t hashCalculate (char *dataStructure, size_t dataStructureSize)
{
    size_t controlSum = 5381;
    for (size_t idx = 1; idx < dataStructureSize; idx++)
        controlSum += idx * dataStructure[idx];

    return (((controlSum << 5) + controlSum) + 1);
}
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef DATACANARY
ISERROR movePointerOnCanaryCount (elem_t **data, int canaryCount)
{
    checkError(data, NULLPOINTER);

    canary_t *newData = (canary_t *) *data;
    newData += canaryCount;
    *data = (elem_t *) newData;

    return NOTERROR;
}

ISERROR saveCanary (canary_t canary, canary_t *canaryPointer)
{
    checkError(canaryPointer, NULLPOINTER);

    *canaryPointer = canary;

    return NOTERROR;
}
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR simpleStackDump(stack *stk)
{
    checkError(stk, NULLPOINTER);

    printf("{\n" BOLD "    Capacity: %ld\n    First free index: %ld\n" RESET,
           stk->capacity, stk->size);

    #ifdef HASH
    printf(BOLD "    Current sum: %ld\n" RESET, stk->currentSum);
    #endif

    #ifdef STRUCTCANARY
    printf(BOLD "    Left canary: %llx\n    Right canary: %llx\n" RESET, 
           stk->leftCanary, stk->rightCanary);
    #endif

    if (stk->size > 0)
    {
        #ifdef DATACANARY
        movePointerOnCanaryCount(&(stk->data), -1);

        printf(BOLD "    Left data canary: %llx\n" RESET, *(canary_t *) stk->data);

        movePointerOnCanaryCount(&(stk->data),  1);

        stk->data += stk->capacity;
        printf(BOLD "    Right data canary: %llx\n" RESET, *(canary_t *) stk->data);
        stk->data -= stk->capacity;
        #endif

        for (size_t idx = 0; idx < stk->capacity; idx++)
        {
            printf("    [%ld]:  ", idx);
            elementOutput(stk->data[idx]);
            if (stk->isPoison[idx])
                printf(BOLDRED " (Poison)" RESET);

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
    checkError(stk, NULLPOINTER);

    ISERROR ERROR = stackVerifier(stk);
    
    printf(BOLDMAGENTA "Stack Dump at %s:%d\n" RESET BOLD "Stack %s[%p] " RESET, 
           file, line, stkName + 1, stk);

    #ifdef BIRTHINFO
    printf(BOLD "Birth at %s:%ld) - " RESET, stk->birthFile, stk->birthLine);
    #endif
    
    if (ERROR == NOTERROR)
        printf(BOLDGREEN "(OK)\n" RESET);
    else
        printf(BOLDRED "(ERROR: %x)\n" RESET, (int) ERROR);

    simpleStackDump(stk);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackConstructorFunction (stack *stk, size_t capacity, const char *file, size_t line)
{
    checkError(stk, NULLPOINTER);

    stk->capacity = capacity;
    stk->size     = 0;

    #ifdef DATACANARY
    stk->data = (elem_t *) recalloc (stk->data, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    saveCanary(Canary3, (canary_t *) stk->data);

    movePointerOnCanaryCount(&(stk->data), 1);

    stk->data += capacity;
    saveCanary(Canary4, (canary_t *) stk->data);
    stk->data -=capacity;

    #else
    stk->data = (elem_t *) calloc (capacity, sizeof(elem_t));
    #endif

    stk->isPoison = (int *) calloc (capacity, sizeof(int));

    for (size_t idx = 0; idx < capacity; idx++)
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

    return NOTERROR;
}

ISERROR stackDestructor (stack *stk)
{
    checkError(stk, NULLPOINTER);

    stk->capacity   = 0;
    stk->size       = 0;

    #ifdef HASH
    stk->currentSum = 0;
    #endif

    #ifdef DATACANARY
    movePointerOnCanaryCount(&(stk->data), -1);
    #endif

    free(stk->data);
    free(stk->isPoison);

    stk->data       = NULL;
    stk->isPoison   = NULL;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackPushResize (stack *stk)
{
    checkError(stk, NULLPOINTER);

    if (stk->size < stk->capacity)
        return NOTERROR;

    #ifdef DATACANARY
    stk->data += stk->capacity;
    canary_t *oldCanary = (canary_t *) stk->data;
    stk->data -= stk->capacity;

    *oldCanary = 0;
    #endif

    stk->capacity *= 2;

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

    for (size_t idx = stk->size; idx < stk->capacity; idx++)
    {
        stk->data[idx] = Poison; 
        stk->isPoison[idx] = 1;
    }

    return NOTERROR;
}

ISERROR stackPush (stack *stk, elem_t element)
{
    checkError(stk,               NULLPOINTER);
    checkError(isfinite(element), ISNOTFINITE);

    checkError(stackVerifier(stk) == NOTERROR, ERROR);

    stackPushResize(stk);

    stk->data[stk->size] = element;
    stk->isPoison[stk->size] = 0;

    #ifdef HASH
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    #endif

    checkError(stackVerifier(stk) == NOTERROR, ERROR);

    stk->size++;

    return NOTERROR;
}

ISERROR stackPopResize (stack *stk)
{
    if (stk->size > (stk->capacity) / 2)
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

ISERROR stackPop (stack *stk)
{
    checkError(stk,            NULLPOINTER);
    checkError(stk->size != 0, POPOUTEMPTY);
    
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
   
    stk->size--;

    stk->data[stk->size] = Poison;
    stk->isPoison[stk->size] = 1;
    stackPopResize(stk);

    #ifdef HASH
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    #endif

    checkError(stackVerifier(stk) == NOTERROR, ERROR);

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
    const elem_t Element = *(elem_t *) element;
    printf("%x", Element);
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
