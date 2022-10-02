#include "stackUtils.h"

int main ()
{
    stack stk1 = {};
    doThis(stackConstructor(&stk1, 0));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 1));
    doThis(stackDump(&stk1));

    doThis(stackPop(&stk1, NULL));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, Poison));
    doThis(stackDump(&stk1));

    doThis(stackPop(&stk1, NULL));
    doThis(stackDump(&stk1));

    doThis(stackDestructor(&stk1));
    doThis(stackDump(&stk1));

    return 0;
}
