#include <stdlib.h>
#include <time.h>

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