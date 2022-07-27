#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Coroutine.h"

struct Coroutine
{
    Coaction action;
    int      status;
    void*    previous;
    void*    current;
    void*    data;
    Coaction onFree;
    void*    onFreeData;
};

WINAPI VOID coFiberProc(LPVOID data)
{
    Coroutine* c = (Coroutine*) data;

    c->status = CO_RUNNING;
    c->action(c->data);
    c->status = CO_DEAD;

    SwitchToFiber(c->previous);
}

Coroutine* coCreate(Coaction action)
{
    Coroutine* c = (Coroutine*) HeapAlloc(GetProcessHeap(), 0, sizeof(Coroutine));

    void* previous = GetCurrentFiber();

    if (previous == (void*) 0x1e00)
        previous = ConvertThreadToFiber(0);

    c->action     = action;
    c->status     = CO_SUSPENDED;
    c->previous   = previous;
    c->current    = CreateFiber(0, &coFiberProc, c);
    c->data       = 0;
    c->onFree     = 0;
    c->onFreeData = 0;

    return c;
}

int coStatus(Coroutine* c)
{
    return c->status;
}

void* coResume(Coroutine* c, void* data)
{
    if (c->status == CO_DEAD)
        return 0;

    c->data = data;
    SwitchToFiber(c->current);
    return c->data;
}

void* coYield(void* data)
{
    Coroutine* c = (Coroutine*) GetFiberData();

    c->data = data;
    SwitchToFiber(c->previous);
    return c->data;
}

void coOnFree(Coaction action, void* data)
{
    Coroutine* c = (Coroutine*) GetFiberData();

    c->onFree     = action;
    c->onFreeData = data;
}

void coFree(Coroutine* c)
{
    if (c->onFree)
        c->onFree(c->onFreeData);

    DeleteFiber(c->current);
    HeapFree(GetProcessHeap(), 0, c);
}
