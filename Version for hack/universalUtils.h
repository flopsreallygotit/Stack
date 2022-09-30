#ifndef UNIVERSAL_UTILS
#define UNIVERSAL_UTILS

#include <stdio.h>
#include <stdarg.h>

#define RESET       "\033[0m"
#define BOLD        "\033[1m"     
#define BLACK       "\033[30m"      
#define RED         "\033[31m"      
#define GREEN       "\033[32m"      
#define YELLOW      "\033[33m"      
#define BLUE        "\033[34m"      
#define MAGENTA     "\033[35m"      
#define CYAN        "\033[36m"      
#define WHITE       "\033[37m"  
#define BOLDBLACK   "\033[1m\033[30m"      
#define BOLDRED     "\033[1m\033[31m"      
#define BOLDGREEN   "\033[1m\033[32m"      
#define BOLDYELLOW  "\033[1m\033[33m"      
#define BOLDBLUE    "\033[1m\033[34m"      
#define BOLDMAGENTA "\033[1m\033[35m"      
#define BOLDCYAN    "\033[1m\033[36m"      
#define BOLDWHITE   "\033[1m\033[37m"  

/// @brief Returns returnValue if condition is wrong.
/// @param[in] condition Condition that you need to check.
/// @param[out] returnValue Value that function returns.
#define checkError(condition, returnValue)                                                               \
    if (!(condition))                                                                                    \
    {                                                                                                    \
        fprintf(stderr, BOLDRED "Error in file %s:%d line! %s\n" RESET, __FILE__, __LINE__, #condition); \
        return (returnValue);                                                                            \
    }

/// @brief Logs value. Function have fprintf format.
/// @param[in] argumentsFormat String with specificators.
void logFunction (const char *file, size_t line, const char *argumentsFormat, ...) __attribute__ ((format (printf, 3, 4)));

/// @brief Shell for logging function.
#define logThis(__FORMAT__, __ARGS__, ...) \
    logFunction(__FILE__, __LINE__, __FORMAT__, __ARGS__);

/// @brief Reallocates memory for pointer and nulling it.
/// @param ptr Pointer.
/// @param newSize New pointer memory size.
void *recalloc (void *ptr, size_t newSize);

#endif
