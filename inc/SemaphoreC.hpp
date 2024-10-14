//
// Created by os on 8/3/23.
//

#ifndef PROJECT_BASE_V1_1_4_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_4_SEMAPHORE_HPP

#include "MemoryAllocator.hpp"
#include "tcb.hpp"

class SemaphoreC {
private:

    struct Elem{
        TCB* elem;
        Elem* next;
    };

    int val;

    unsigned int getVal(){return val;};
    void setVal(int init){val = init;}

    Elem* head = nullptr;
    Elem* tail = nullptr;

    void addLast(TCB* tcb);
    TCB* removeFirst();

public:
    static SemaphoreC* createSemaphore(unsigned int init);

    int wait();

    int signal();

    void freeAll();
};


#endif //PROJECT_BASE_V1_1_4_SEMAPHORE_HPP
