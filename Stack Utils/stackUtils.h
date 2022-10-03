#ifndef STACKUTILS
#define STACKUTILS

#include "universalUtils.h"
#include "configStack.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum ISERROR
{
    /// Shows that function fulfilled without errors.
    NOTERROR        = 0,

    /// Pointer is NULL.
    NULLPOINTER     = 1,

    /// Variables are not equal or inequality unfulfilled.
    INEQUALITY      = 2,

    /// Value of variable is NAN, inf or overflow max limit of it type.
    ISNOTFINITE     = 3,

    /// Wrong sum. 
    WRONGSUM        = 4,

    /// Pop from empty stack.
    POPOUTEMPTY     = 5,

    /// Left canary died.
    LEFTCANARY      = 6,

    /// Right canary died.
    RIGHTCANARY     = 7,

    /// Data left canary.
    DATALEFTCANARY  = 8,

    /// Data right canary died.
    DATARIGHTCANARY = 9,

    /// Stack is wrong.
    WRONGSTACK      = 10,

    /// Stack size is wrong or capacity is bigger than MAXCAPACITY
    WRONGSIZE       = 11,

    /// General error.
    ERROR           = 12
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Calculates hash for stack.
/// @param dataStructure Stack data.
/// @param dataStructureSize Size of data.
/// @return Hash control sum.
size_t hashCalculate  (char *dataStructure, ssize_t dataStructureSize);

/// @brief Checks that stack is correct.
/// @param stk Pointer to stack.
/// @return Error type.
ISERROR stackVerifier (stack *stk);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Moves pointer to data on canary count.
/// @param data Data.
/// @param canaryCount Canary Count.
ISERROR movePointerOnCanaryCount (elem_t **data, int canaryCount);

#ifdef DATACANARY
/// @brief Saves canary on address that canary pointer points on.
/// @param canary Canary.
/// @param canaryPointer Pointer to canary.
ISERROR saveCanary (canary_t canary, canary_t *canaryPointer);
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Dumps lesser info than default stack file using no additional info.
/// @param stk Stack.
ISERROR simpleStackDump (stack *stk);

#define dumpAndReturn(stkPointer, returnValue) \
{                                               \
    simpleStackDump(stkPointer);                \
    return returnValue;                         \
}

/// @brief Dumps stack to console.
/// @param stk Pointer to stack.
/// @param stkName Name of stack variable.
/// @param file File where stack contains.
/// @param line Line where stack contains.
ISERROR stackDumpFunction (stack *stk, const char *stkName, 
                          const char *file, int line);

/// Shell for StackDumpFunction.
#define stackDump(stk) \
    stackDumpFunction(stk, #stk, __FILE__, __LINE__)

/// @brief Outputs element of int type.
void elementOutput (const int    element);

/// @brief Outputs element of double type.
void elementOutput (const double element);

/// @brief Outputs element of char type.
void elementOutput (const char   element);

/// @brief Outputs element of long type.
void elementOutput (const long   element);

/// @brief Outputs element of short type.
void elementOutput (const short  element);

/// @brief Outputs element hex version.
void elementOutput (const void *element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Constructs stack.
/// @param stk Pointer to stack.
/// @param capacity Stack capacity.
/// @param file File where stack contains.
/// @param line Line where stack contains.
ISERROR stackConstructorFunction (stack *stk, ssize_t capacity, 
                                  const char *file, size_t line);

/// Shell for Stack Constructor Function.
#define stackConstructor(stk, cpty) \
    stackConstructorFunction(stk, cpty, __FILE__, __LINE__)

/// @brief Destructs stack.
/// @param stk Pointer to stack.
ISERROR stackDestructor (stack *stk);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Resizes stack for element push.
/// @param stk Pointer to stack.
ISERROR stackPushResize (stack *stk);

/// @brief Pushes element from stack.
/// @param stk Pointer to stack.
/// @param element Element.
ISERROR stackPush       (stack *stk, elem_t element);

/// @brief Resizes stack for element pop.
/// @param stk Pointer to stack.
ISERROR stackPopResize  (stack *stk);

/// @brief Deletes last pushed element from stack.
/// @param stk Pointer to stack.
ISERROR stackPop        (stack *stk, elem_t *element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define doThis(functionValue)                                                        \
    if (!((functionValue) == NOTERROR))                                              \
    {                                                                                \
        fprintf(stderr, BOLDRED "Error in file %s:%d line! Error code: %d;\n" RESET, \
                __FILE__, __LINE__, (functionValue));                                \
        return -1;                                                                   \
    } 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
