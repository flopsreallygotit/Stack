#include "stackUtils.h"

int main ()
{
    stack stk1 = {};
    doThis(stackConstructor(&stk1, 2));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, Poison));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 2));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 3));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 4));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 5));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 6));
    doThis(stackDump(&stk1));

    doThis(stackPop(&stk1));
    doThis(stackDump(&stk1));

    doThis(stackPop(&stk1));
    doThis(stackDump(&stk1));

    doThis(stackPop(&stk1));
    doThis(stackDump(&stk1));

    doThis(stackDestructor(&stk1));
    doThis(stackDump(&stk1));

    return 0;
}
