#include <stdio.h>

#include "coroutine.h"

void status(void* _)
{
    coYield(0);
}

int main()
{
    Coroutine* c = coCreate(status);

    printf("status = %d\n", coStatus(c));

    coResume(c, 0);

    printf("status = %d\n", coStatus(c));

    coResume(c, 0);

    printf("status = %d\n", coStatus(c));

    coFree(c);

    return 0;
}
