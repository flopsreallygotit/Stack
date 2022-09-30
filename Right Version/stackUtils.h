#ifndef STACKUTILS
#define STACKUTILS

#include "universalUtils.h"
#include "configStack.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum ISERROR
{
    /// Shows that function fulfilled without errors.
    NOTERROR    = 0x00,

    /// Pointer is NULL.
    NULLPOINTER = 0x10,

    /// Variables are not equal or inequality unfulfilled.
    INEQUALITY  = 0x20,

    /// Value of variable is NAN, inf or overflow max limit of it type.
    ISNOTFINITE = 0x30,

    /// Wrong sum.
    WRONGSUM    = 0x40,

    /// Pop from empty stack.
    POPOUTEMPTY = 0x50,

    /// Left canary died.
    LEFTCANARY  = 0x60,

    /// Right canary died.
    RIGHTCANARY = 0x70,

    /// Stack is wrong.
    WRONGSTACK  = 0x80,

    /// General error.
    ERROR       = 0x90
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int notZeroSymbol (void);

int calculateMaxLength (void);

unsigned long long canaryGenerate (void);

size_t hashCalculate  (char *dataStructure, size_t dataStructureSize);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackVerifier (stack *stk);

ISERROR stackDumpFunction(stack *stk, const char *stkName, 
                          const char *file, int line);

#define stackDump(stk) \
    stackDumpFunction(stk, #stk, __FILE__, __LINE__)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackConstructorFunction (stack *stk, size_t capacity, 
                                  const char *file, size_t line);

#define stackConstructor(stk, cpty) \
    stackConstructorFunction(stk, cpty, __FILE__, __LINE__)

ISERROR stackDestructor (stack *stk);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR stackResize (stack *stk);
ISERROR stackPush   (stack *stk, elem_t element);
ISERROR stackPop    (stack *stk);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void elementOutput (int    element);

void elementOutput (double element);

void elementOutput (char   element);

void elementOutput (long   element);

void elementOutput (short  element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Save shell for functions.
/// @param[in] condition Condition that you need to check.
#define doThis(condition) \
    checkError(((condition) == NOTERROR), ERROR); 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif