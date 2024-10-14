//
// Created by os on 8/3/23.
//

#include "../h/SemaphoreC.hpp"
#include "../h/syscall_c.h"
#include "../h/Scheduler.hpp"

SemaphoreC *SemaphoreC::createSemaphore(unsigned int init) {
    SemaphoreC* newSemaphore = (SemaphoreC*)MemoryAllocator::instanceOf().mem_alloc((sizeof(SemaphoreC) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    if(newSemaphore)newSemaphore->setVal(init);
    if(newSemaphore) newSemaphore->head = newSemaphore->tail = nullptr;
    return newSemaphore;
}

void SemaphoreC::addLast(TCB *tcb) {
    //Elem* novi = (Elem*)mem_alloc(sizeof(Elem));//(Elem*)MemoryAllocator::instanceOf().mem_alloc((sizeof(Elem) + MEM_BLOCK_SIZE - 1 / MEM_BLOCK_SIZE));
    Elem* novi = (Elem*)MemoryAllocator::instanceOf().mem_alloc((sizeof(Elem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    novi->elem = tcb;
    novi->next = nullptr;
    if(head == nullptr) head = novi;
    else tail->next = novi;
    tail = novi;
}

TCB *SemaphoreC::removeFirst() {
    if(head == nullptr) return nullptr;
    Elem* tek = head;
    head = head->next;
    TCB* tcb = tek->elem;
    MemoryAllocator::instanceOf().mem_free(tek);
    if(head == nullptr) tail = nullptr;
    return tcb;
}

int SemaphoreC::wait() {    //po ugledu na zadatke sa kolokvijuma
    TCB* oldRunning = TCB::running;
    val = val - 1;
    if(val < 0){
        addLast(oldRunning);
        oldRunning->setBlocked(true);
        thread_dispatch();
    }
    return 0;
}

int SemaphoreC::signal() {  //po ugledu na zadatke sa kolokvijuma
    val = val + 1;
    if(val <= 0){
        TCB* tek = removeFirst();
        tek->setBlocked(false);
        Scheduler::instanceof().addFirst(tek);
    }
    return 0;
}

void SemaphoreC::freeAll() {
    while(head != nullptr){
        Elem* old = head;
        TCB* tek = head->elem;
        tek->blocked = false;
        head = head->next;
        MemoryAllocator::instanceOf().mem_free(old);
        Scheduler::instanceof().addFirst(tek);
    }
}
