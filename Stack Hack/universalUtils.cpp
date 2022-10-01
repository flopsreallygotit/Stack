#include "universalUtils.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

static FILE *logOutput = NULL;

void logFunction (const char *file, size_t line, const char *argumentsFormat,  ...)
{                                                                            
    if (!logOutput)
    {
        logOutput = fopen("logOutput.txt", "a");
        setvbuf(logOutput, NULL, _IONBF, 0); 
    }

    fprintf(logOutput, "In file %s, in %ld line | ", file, line);    

    va_list arguments;                     
    va_start(arguments, argumentsFormat);                   
    vfprintf(logOutput, argumentsFormat, arguments);                                                                 
    va_end(arguments);

    return;
}

void *recalloc (void *ptr, size_t newSize) 
{
    size_t oldSize = malloc_usable_size(ptr);

    ptr = realloc(ptr, newSize);
    char *endptr = (char *) ptr;

    for (size_t idx = oldSize; idx < newSize; idx++) 
        endptr[idx] = 0;

    return ptr;
}
