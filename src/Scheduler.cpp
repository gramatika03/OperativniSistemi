//
// Created by os on 7/30/23.
//
#include "../h/Scheduler.hpp"
#include "../h/syscall_c.h"

bool Scheduler::added = true;
TCB* Scheduler::idle = nullptr;

Scheduler& Scheduler::instanceof() {
    static Scheduler instanca;
    if(added){
        idle = TCB::createThread(fun, nullptr, (char*)MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE),
                                 (char*)MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE));
        instanca.addFirst(idle);
        added = false;
    }
    return instanca;
}

void Scheduler::addFirst(TCB *tcb) {
    Elem* novi = (Elem*) MemoryAllocator::instanceOf().mem_alloc((sizeof(Elem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    novi->elem = tcb;
    novi->next = nullptr;
    if(head == nullptr) head = novi;
    else tail->next = novi;
    tail = novi;
}

TCB* Scheduler::removeLast() {
    if(head == nullptr) return idle;
    Elem* tek = head;
    TCB* tekElem = tek->elem;
    head = head->next;
    MemoryAllocator::instanceOf().mem_free(tek);
    if(head == nullptr) tail = nullptr;
    return tekElem;
}

void Scheduler::fun(void *a) {
    switchToUserRegime();
    while(1){
        thread_dispatch();
    }
}
