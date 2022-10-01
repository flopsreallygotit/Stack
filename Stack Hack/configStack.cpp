#include <stdlib.h>
#include <time.h>
#include "configStack.h"

int notZeroSymbol (void)
{
    int symbol = rand();
    while (symbol == 0 || symbol >= 10)
        symbol = symbol % 10 + 1;

    return symbol;
}

int calculateMaxLength (void)
{
    canary_t maxCanary = -1;
    int maxLength = 0; 

    while (maxCanary > 0)
    {
        maxCanary -= maxCanary % 10;
        maxCanary /= 10;
        maxLength++;
    }

    return maxLength;
}

canary_t canaryGenerate (void)
{
    int canaryLength = calculateMaxLength();
    canary_t canary = 0, power = 1;

    for (int i = 0; i < canaryLength - 1; i++)
    {
        canary += notZeroSymbol() * power;
        power  *= 10;
    }

    return canary + power;
}