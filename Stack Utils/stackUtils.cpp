#include "stackUtils.h"
#include "universalUtils.h"
#include <math.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef DEBUG

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackVerifier (stack *stk)
{
    if (stk->capacity && stk->data)
    {
        checkError((stk->currentSum) == hashCalculate((char *) stk->data, 
                   (stk->capacity) * sizeof(elem_t)), WRONGSUM);

        checkError(stk->birthFile,               NULLPOINTER   );
        checkError(stk->leftCanary   == Canary1, LEFTCANARY    ); // TODO add descriptional error output somewhere
        checkError(stk->rightCanary  == Canary2, RIGHTCANARY   );

        movePointerOnCanaryCount(&(stk->data), -1);
        checkError(*((canary_t *) stk->data)                 == Canary3, DATALEFTCANARY );
        //checkError(*((canary_t *) stk->data + stk->capacity) == Canary4, DATARIGHTCANARY);
        movePointerOnCanaryCount(&(stk->data),  1);
    }

    return NOTERROR;
}

size_t hashCalculate (char *dataStructure, size_t dataStructureSize)
{
    size_t controlSum = 5381;
    for (size_t idx = 1; idx < dataStructureSize; idx++)
        controlSum += idx * dataStructure[idx];

    return (((controlSum << 5) + controlSum) + 1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
        printf(BOLDRED "(ERROR: %x)\n" RESET, (int) ERROR);

    printf("{\n" BOLD "    Capacity: %ld\n    First free index: %ld\n    Current sum: %ld\n" 
           "    Left canary: %llx\n    Right canary: %llx\n" RESET, 
           stk->capacity, stk->size, stk->currentSum,
           stk->leftCanary, stk->rightCanary);

    if (stk->size > 0)
    {
        movePointerOnCanaryCount(&(stk->data), -1);

        printf(BOLD "    Left data canary: %llx\n" RESET, *(canary_t *) stk->data);

        movePointerOnCanaryCount(&(stk->data),  1);

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackConstructorFunction (stack *stk, size_t capacity, const char *file, size_t line)
{
    checkError(stk, NULLPOINTER);

    stk->capacity = capacity;
    stk->size     = 0;

    #ifdef DEBUG
    stk->data = (elem_t *) recalloc (stk->data, capacity * sizeof(elem_t) + 2 * sizeof(canary_t));

    saveCanary(Canary3, (canary_t *) stk->data);
    saveCanary(Canary4, (canary_t *) stk->data + capacity);

    movePointerOnCanaryCount(&(stk->data), 1);
    #else
    stk->data  = (elem_t *) calloc (capacity, sizeof(elem_t));
    #endif

    stk->isPoison = (int *) calloc (capacity, sizeof(int));

    for (size_t idx = 0; idx < capacity; idx++)
    {
        stk->data[idx] = Poison;
        stk->isPoison[idx] = 1;     
    }

    #ifdef DEBUG
    stk->leftCanary   = Canary1;
    stk->rightCanary  = Canary2;
    
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

    movePointerOnCanaryCount(&(stk->data), -1);
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

    stk->capacity *= 2;

    #ifdef DEBUG
    movePointerOnCanaryCount(&(stk->data), -1);

    stk->data = (elem_t *) recalloc (stk->data, (stk->capacity) * sizeof(elem_t) + 2 * sizeof(canary_t));

    canary_t *oldCanary = (canary_t *) stk->data + (stk->capacity) / 2;
    *oldCanary = 0;
    saveCanary(Canary4, (canary_t *) stk->data + stk->capacity);

    movePointerOnCanaryCount(&(stk->data),  1);
    #else
    stk->data  = (elem_t *) recalloc (stk->data,     (stk->capacity) * sizeof(elem_t));
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

    #ifdef DEBUG // TODO add debug levels
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif

    stackPushResize(stk);

    stk->data[stk->size] = element;
    stk->isPoison[stk->size] = 0;

    #ifdef DEBUG
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif

    stk->size++;

    return NOTERROR;
}

ISERROR stackPopResize (stack *stk)
{
    if (stk->size > (stk->capacity) / 2)
        return NOTERROR;

    stk->capacity /= 2;

    #ifdef DEBUG
    movePointerOnCanaryCount(&(stk->data), -1);

    stk->data = (elem_t *) recalloc (stk->data, sizeof(elem_t) * (stk->capacity) + 2 * sizeof(canary_t));
    saveCanary(Canary4, (canary_t *) stk->data + stk->capacity);

    movePointerOnCanaryCount(&(stk->data),  1);
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
    
    #ifdef DEBUG
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif
   
    stk->size--;

    stk->data[stk->size] = Poison;
    stk->isPoison[stk->size] = 1;
    stackPopResize(stk);

    #ifdef DEBUG
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
    #endif

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void elementOutput (const int element)
{
    printf("%d", element);
}

void elementOutput (const double element)
{
    printf("%lf", element);
}

void elementOutput (const char element)
{
    printf("%c", element);
}

void elementOutput (const long element)
{
    printf("%ld", element);
}

void elementOutput (const short element) 
{
    printf("%hd", element);
}

void elementOutput (const void *element) 
{
    const elem_t Element = *(elem_t *) element;
    printf("%x", Element);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
