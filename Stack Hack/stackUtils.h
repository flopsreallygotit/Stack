#ifndef STACKUTILS
#define STACKUTILS

#include "universalUtils.h"
#include "configStack.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum ISERROR
{
    /// Shows that function fulfilled without errors.
    NOTERROR        = 0x00,

    /// Pointer is NULL.
    NULLPOINTER     = 0x10,

    /// Variables are not equal or inequality unfulfilled.
    INEQUALITY      = 0x20,

    /// Value of variable is NAN, inf or overflow max limit of it type.
    ISNOTFINITE     = 0x30,

    /// Wrong sum. 
    WRONGSUM        = 0x40,

    /// Pop from empty stack.
    POPOUTEMPTY     = 0x50,

    /// Left canary died.
    LEFTCANARY      = 0x60,

    /// Right canary died.
    RIGHTCANARY     = 0x70,

    /// Data left canary.
    DATALEFTCANARY  = 0x80,

    /// Data right canary died.
    DATARIGHTCANARY = 0x90,

    /// Stack is wrong.
    WRONGSTACK      = 0x100,

    /// General error.
    ERROR           = 0x110
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Calculates hash for stack.
/// @param dataStructure Stack data.
/// @param dataStructureSize Size of data.
/// @return Hash control sum.
size_t hashCalculate  (char *dataStructure, size_t dataStructureSize);

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
ISERROR simpleStackDump(stack *stk);

/// @brief Dumps stack to console.
/// @param stk Pointer to stack.
/// @param stkName Name of stack variable.
/// @param file File where stack contains.
/// @param line Line where stack contains.
ISERROR stackDumpFunction(stack *stk, const char *stkName, 
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
ISERROR stackConstructorFunction (stack *stk, size_t capacity, 
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
ISERROR stackPush   (stack *stk, elem_t element);

/// @brief Resizes stack for element pop.
/// @param stk Pointer to stack.
ISERROR stackPopResize (stack *stk);

/// @brief Deletes last pushed element from stack.
/// @param stk Pointer to stack.
ISERROR stackPop    (stack *stk);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Save shell for functions.
/// @param[in] condition Condition that you need to check.
#define doThis(condition) \
    checkError(((condition) == NOTERROR), ERROR); 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif