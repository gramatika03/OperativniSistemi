#ifndef PROJEKAT_H_SCHEDULER
#define PROJEKAT_H_SCHEDULER

#include "MemoryAllocator.hpp"

class TCB;

struct Elem{
    TCB* elem;
    Elem* next;
};

class Scheduler{
private:
    Elem* tail = nullptr;
    static bool added;
    static TCB* idle;
    static void fun(void* a);

public:
    static Scheduler& instanceof();

    void addFirst(TCB* tcb);

    TCB* removeLast();

    Elem* head = nullptr;
};



#endif