#include "stackUtils.h"
#include <stdlib.h>
#include <math.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int notZeroSymbol (void)
{
    int symbol = rand();
    while (symbol <= 0 || symbol >= 10)
        symbol = symbol % 10 + 1;

    return symbol;
}

int calculateMaxLength (void)
{
    unsigned long long maxULL = -1;
    int maxLength = 0;

    while (maxULL > 0)
    {
        maxULL -= maxULL % 10;
        maxULL /= 10;
        maxLength++;
    }

    return maxLength;
}

unsigned long long canaryGenerate (void)
{
    int maxLength = calculateMaxLength();
    unsigned long long canary = 0, power = 1;

    for (int i = 0; i < maxLength - 1; i++)
    {
        canary += notZeroSymbol() * power;
        power  *= 10;
    }
    
    return canary + power;
}

int maxLengthCheck (void)
{
    size_t maxSizeValue = -1;
    int length = 0;

    while (maxSizeValue)
    {
        maxSizeValue /= 10;
        length++;
    }

    return length;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

size_t hashCalculate (char *dataStructure, size_t dataStructureSize)
{
    size_t controlSum = 5381;
    for (size_t idx = 0; idx <= dataStructureSize; idx++)
        controlSum += idx * dataStructure[idx];

    return (((controlSum << 5) + controlSum) + 1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackVerifier (stack *stk)
{
    if (stk->capacity && stk->data)
    {
        checkError((stk->currentSum) == hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t)), WRONGSUM);
        checkError(stk->birthFile, NULLPOINTER);
        checkError(stk->leftCanary  != Canary1, LEFTCANARY);
        checkError(stk->rightCanary != Canary2, RIGHTCANARY);
    }
    return NOTERROR;
}

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

    printf("{\n" BOLD "    Capacity: %ld\n    First free index: %ld\n    Current sum: %ld\n" RESET, 
           stk->capacity, stk->size, stk->currentSum);

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

ISERROR stackConstructorFunction (stack *stk, size_t capacity, const char *file, size_t line)
{
    checkError(stk, NULLPOINTER);

    // stk->leftCanary  = canaryGenerate();
    // stk->rightCanary = canaryGenerate(); TODO different Canary Generate

    stk->leftCanary = Canary1;
    stk->leftCanary = Canary2;

    stk->capacity = capacity;
    stk->size     = 0;

    stk->data  = (elem_t *) calloc (capacity, sizeof(elem_t));
    stk->isPoison = (int *) calloc (capacity, sizeof(int));

    for (size_t idx = 0; idx < capacity; idx++)
    {
        stk->data[idx] = Poison;
        stk->isPoison[idx] = 1;     
    }

    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));

    stk->birthLine = line;
    stk->birthFile = file;

    return NOTERROR;
}

ISERROR stackDestructor (stack *stk)
{
    checkError(stk, NULLPOINTER);

    stk->capacity   = 0;
    stk->size       = 0;
    stk->currentSum = 0;

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
    checkError(stk,                      NULLPOINTER);
    checkError(isfinite(element),        ISNOTFINITE);
    checkError(stackVerifier(stk) == NOTERROR, ERROR);

    if (stk->size >= stk->capacity)
        stackResize(stk);

    stk->data[stk->size] = element;
    stk->isPoison[stk->size] = 0;
    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));
    stk->size++;

    return stackVerifier(stk);
}

ISERROR stackPop (stack *stk)
{
    checkError(stk,                      NULLPOINTER);
    checkError(stk->size != 0,           POPOUTEMPTY);
    checkError(stackVerifier(stk) == NOTERROR, ERROR);
   
    stk->size--;

    stk->data[stk->size] = Poison;
    stk->isPoison[stk->size] = 1;
    if (stk->size <= (stk->capacity) / 2)
    {
        stk->capacity /= 2;
        stk->data =  (elem_t *) realloc (stk->data, 1 + sizeof(elem_t) * (stk->capacity));
        stk->isPoison = (int *) realloc (stk->isPoison, 1 + sizeof(int) * (stk->capacity));
    }

    stk->currentSum = hashCalculate((char *) stk->data, (stk->capacity) * sizeof(elem_t));

    return stackVerifier(stk);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void elementOutput (int element)
{
    printf("%d", element);
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
