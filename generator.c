#include <stdio.h>

#include "coroutine.h"

void generate(void* _)
{
    int i = 1;

    // Loop forever
    while (1)
        coYield((void*) i++);
}

int main()
{
    Coroutine* c = coCreate(generate);

    // Take 3 numbers
    for (int i = 0; i < 3; ++i)
        printf("%d\n", coResume(c, 0));

    coFree(c);
}
