#ifndef COROUTINE
#define COROUTINE

typedef void (*Coaction)(void*);
typedef struct Coroutine Coroutine;

#define CO_SUSPENDED 0
#define CO_RUNNING   1
#define CO_DEAD      2

Coroutine* coCreate(Coaction);
int coStatus(Coroutine*);
void* coResume(Coroutine*, void*);
void* coYield(void*);
void coOnFree(Coaction, void*);
void coFree(Coroutine*);

#endif
