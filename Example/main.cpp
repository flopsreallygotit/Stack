#include "stackUtils.h"

int main()
{
    stack *stk = stackConstructor(10);

    stackPush(stk, 1);
    stackDump(stk);

    stackDestructor(stk);

    return 0;
}