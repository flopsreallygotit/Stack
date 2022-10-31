#ifndef UNIVERSAL_UTILS_H
#define UNIVERSAL_UTILS_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stdio.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Colors for scanf and printf.
#define RESET   "\033[0m"
#define BOLD    "\033[1m"     
#define BLACK   "\033[30m"      
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define WHITE   "\033[37m"   

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Puts warning in stderr.
#define PUTWARNING(warning)                                                                \
    fprintf(stderr, BOLD YELLOW "Warning in file %s:%d line in function: %s! %s;\n" RESET, \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, warning)                              \

/// @brief Puts error in stderr.
#define PUTERROR(error)                                                               \
    fprintf(stderr, BOLD RED "Error in file %s:%d line in function: %s! %s;\n" RESET, \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, error)                           \

/// @brief Returns returnValue if condition is wrong.
/// @param condition Condition that you need to check.
/// @param value Value that function returns.
#define CHECKERROR(condition, value) \
    do                               \
    {                                \
        if (!(condition))            \
        {                            \
            PUTERROR(#condition);    \
            return (value);          \
        }                            \
    }                                \
    while (0)

/// @brief Check that function returned NOTERROR (Zero / false) value and returns -1 if it false.
#define DOTHIS(value)                        \
    do                                       \
    {                                        \
        CHECKERROR((value) == NOTERROR, -1); \
    }                                        \
    while (0)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Reallocates memory for pointer and nulling it.
/// @param ptr Pointer.
/// @param newSize New pointer memory size.
void *recalloc (void *ptr, size_t newSize);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @return sign(firstNumber - secondNumber)
int differenceSign (double firstNumber, double secondNumber);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
