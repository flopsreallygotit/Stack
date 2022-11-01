#ifndef STACKUTILS
#define STACKUTILS

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "universalUtils.h"
#include "configStack.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum ISERROR
{
    /// Shows that function fulfilled without errors.
    NOTERROR        = 0,

    /// Pointer is NULL.
    NULLPOINTER     = 1,

    /// Value of variable is NAN, inf or overflow max limit of it type.
    ISNOTFINITE     = 2,

    /// Wrong sum. 
    WRONGSUM        = 3,

    /// Pop from empty stack.
    POPOUTEMPTY     = 4,

    /// Left canary died.
    LEFTCANARY      = 5,

    /// Right canary died.
    RIGHTCANARY     = 6,

    /// Data left canary.
    DATALEFTCANARY  = 7,

    /// Data right canary died.
    DATARIGHTCANARY = 8,

    /// Stack is wrong.
    WRONGSTACK      = 9,

    /// Stack size is wrong or capacity is bigger than MAXCAPACITY
    WRONGSIZE       = 10,

    /// General error.
    ERROR           = 11,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Dumps lesser info than default stack file using no additional info.
/// @param stk Stack.
ISERROR simpleStackDump (stack *stk);

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Shell for Stack Constructor Function.
#define stackConstructor(cpty) \
    stackConstructorFunction(cpty, __FILE__, __LINE__)

/// @brief Constructs stack.
/// @param stk Pointer to stack.
/// @param capacity Stack capacity.
/// @param file File where stack contains.
/// @param line Line where stack contains.
stack *stackConstructorFunction (size_t capacity, 
                                 const char *file, size_t line);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Destructs stack.
/// @param stk Pointer to stack.
void stackDestructor (stack *stk);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes element from stack.
/// @param stk Pointer to stack.
/// @param element Element.
ISERROR stackPush (stack *stk, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Deletes last pushed element from stack.
/// @param stk Pointer to stack.
ISERROR stackPop  (stack *stk, elem_t *element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
