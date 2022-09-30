#include "stackUtils.h"
#include <math.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef DEBUG

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackVerifier (stack *stk)
{
    if (stk->capacity && stk->data)
    {
        checkError((stk->currentSum) == hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t)), WRONGSUM);
        checkError(stk->birthFile,               NULLPOINTER );
        checkError(stk->leftCanary   == Canary1, LEFTCANARY  );
        checkError(stk->middleCanary == Canary2, MIDDLECANARY);
        checkError(stk->rightCanary  == Canary3, RIGHTCANARY );
    }
    return NOTERROR;
}

size_t hashCalculate (char *dataStructure, size_t dataStructureSize)
{
    size_t controlSum = 5381;
    for (size_t idx = 0; idx <= dataStructureSize; idx++)
        controlSum += idx * dataStructure[idx];

    return (((controlSum << 5) + controlSum) + 1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackDumpFunction (stack *stk, const char *stkName, 
                           const char *file, int line)
{
    checkError(stk, NULLPOINTER);

    ISERROR ERROR = stackVerifier(stk);
    
    printf(BOLDMAGENTA "Stack Dump at %s:%d\n" RESET BOLD "Stack %s[%p] (Birth at %s:%ld) - " RESET, 
           file, line, stkName + 1, stk, stk->birthFile, stk->birthLine);
    
    if (ERROR == NOTERROR)
        printf(BOLDGREEN "(OK)\n" RESET);
    else
        printf(BOLDRED "(ERROR: %d)\n" RESET, (int) ERROR);

    printf("{\n" BOLD "    Capacity: %ld\n    First free index: %ld\n    Current sum: %ld\n" 
           "    Left canary: %llu\n    Middle canary: %llu\n    Right canary: %llu\n" RESET, 
           stk->capacity, stk->size, stk->currentSum,
           stk->leftCanary, stk->middleCanary, stk->rightCanary);

    if (stk->size > 0)
        for (size_t idx = 0; idx < stk->capacity; idx++)
        {
            printf("    [%ld]:  ", idx);
            elementOutput(stk->data[idx]);
            if (stk->isPoison[idx])
                printf(BOLDRED " (Poison)" RESET);

            putchar('\n');
        }

    else
        printf("    Stack have zero elements.\n");

    printf("};\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackConstructorFunction (stack *stk, size_t capacity, const char *file, size_t line)
{
    checkError(stk, NULLPOINTER);

    stk->capacity = capacity;
    stk->size     = 0;

    stk->data  = (elem_t *) calloc (capacity, sizeof(elem_t));
    stk->isPoison = (int *) calloc (capacity, sizeof(int));

    for (size_t idx = 0; idx < capacity; idx++)
    {
        stk->data[idx] = Poison;
        stk->isPoison[idx] = 1;     
    }

    #ifdef DEBUG
    stk->leftCanary   = Canary1;
    stk->middleCanary = Canary2;
    stk->rightCanary  = Canary3;
    
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));

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

    #ifdef DEBUG
    stk->currentSum = 0;
    #endif

    stk->data       = NULL;
    stk->isPoison   = NULL;

    free(stk->data);
    free(stk->isPoison);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackResize (stack *stk)
{
    checkError(stk, NULLPOINTER);

    stk->capacity *= 2;
    stk->data  = (elem_t *) realloc (stk->data,     (stk->capacity) * sizeof(elem_t));
    stk->isPoison = (int *) realloc (stk->isPoison, (stk->capacity) * sizeof(int));

    for (size_t idx = stk->size; idx < stk->capacity; idx++)
    {
        stk->data[idx] = Poison; 
        stk->isPoison[idx] = 1;
    }

    return NOTERROR;
}

ISERROR stackPush (stack *stk, elem_t element)
{
    checkError(stk, NULLPOINTER);

    #ifdef DEBUG
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif

    if (stk->size >= stk->capacity)
        stackResize(stk);

    stk->data[stk->size] = element;
    stk->isPoison[stk->size] = 0;

    #ifdef DEBUG
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif

    stk->size++;

    return NOTERROR;
}

ISERROR stackPop (stack *stk)
{
    checkError(stk,                      NULLPOINTER);
    checkError(stk->size != 0,           POPOUTEMPTY);
    
    #ifdef DEBUG
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif
   
    stk->size--;

    stk->data[stk->size] = Poison;
    stk->isPoison[stk->size] = 1;
    if (stk->size <= (stk->capacity) / 2)
    {
        stk->capacity /= 2;
        stk->data =  (elem_t *) realloc (stk->data, 1 + sizeof(elem_t) * (stk->capacity));
        stk->isPoison = (int *) realloc (stk->isPoison, 1 + sizeof(int) * (stk->capacity));
    }

    #ifdef DEBUG
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void elementOutput (int element)
{
    printf("%hd", element);
}

void elementOutput (double element)
{
    printf("%lf", element);
}

void elementOutput (char element)
{
    printf("%c", element);
}

void elementOutput (long element)
{
    printf("%ld", element);
}

void elementOutput (short element)
{
    printf("%hd", element);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
