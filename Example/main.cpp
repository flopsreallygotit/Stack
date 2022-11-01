#include "stackUtils.h"

int main()
{
    stack *stk = stackConstructor(10);

    stackPush(stk, 1);
    stackPush(stk, 4);
    stackPush(stk, 9);

    stackDump(stk);

    elem_t myStackValues[10] = {};

    stackPop(stk, &myStackValues[0]);
    stackPop(stk, &myStackValues[1]);
    stackPop(stk, &myStackValues[2]);

    printf("My stack values: %d %d %d;\n", myStackValues[0], 
           myStackValues[1], myStackValues[2]);

    stackDump(stk);

    stackDestructor(stk);

    return 0;
}