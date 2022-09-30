#include "stackUtils.h"

int main ()
{
    /// You can change StackConfig.h and main.cpp GL && HF.
    stack stk1 = {};
    doThis(stackConstructor(&stk1, 1));
    doThis(stackDump(&stk1));

    doThis(stackPush(&stk1, 2));
    doThis(stackDump(&stk1));

    doThis(stackPop(&stk1));
    doThis(stackDump(&stk1));

    return 0;
}
