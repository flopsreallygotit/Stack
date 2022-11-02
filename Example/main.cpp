// Simple example of stack usage.

// Including header with all stack commands.
#include "stackUtils.h"

int main()
{
    /// Constructing stack.
    stack *stk = stackConstructor(10);

    // Push 1 to stack: 1;
    stackPush(stk, 1);

    // Push 4 to stack: 4 1;
    stackPush(stk, 4);

    // Push 9 to stack: 9 4 1;
    stackPush(stk, 9);

    // Dumps info about stack.
    stackDump(stk);

    // Creating array for our values.
    elem_t myStackValues[10] = {};

    /// Pops element from stack to zero index of array: 4 1;
    stackPop(stk, &myStackValues[0]);

    /// Pops element from stack to first index of array: 1;
    stackPop(stk, &myStackValues[1]);

    /// Pops element from stack to second index of array;
    stackPop(stk, &myStackValues[2]);

    printf("My stack values: %d %d %d;\n", myStackValues[0], 
           myStackValues[1], myStackValues[2]);

    // Dumps info about stack.
    stackDump(stk);

    // Destructing stack.
    stackDestructor(stk);

    return 0;
}