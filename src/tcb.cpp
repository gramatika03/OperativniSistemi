#include "../h/tcb.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/Scheduler.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
TCB::ElemSleep* TCB::headSleep = nullptr;

TCB* TCB::createThread(Body body, void* arg, char* stack, char* kernelStack) {
    TCB* newTCB = (TCB*) MemoryAllocator::instanceOf().mem_alloc((sizeof(TCB) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    if(newTCB == nullptr) return nullptr;
    newTCB->finished = false;
    newTCB->body = body;
    newTCB->argument = arg;
    newTCB->stack = (uint64) stack;
    newTCB->userStack = (uint64)&(stack[DEFAULT_STACK_SIZE]);
    newTCB->context.ra = (uint64) &threadWrapper;
    newTCB->context.sp = (uint64)&(stack[DEFAULT_STACK_SIZE]);
    newTCB->userSP = newTCB->userStack;
    newTCB->blocked = false;
    newTCB->head = nullptr;
    newTCB->timeSlice = DEFAULT_TIME_SLICE;
    return newTCB;
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->argument);
    running->setFinished(true);
    running->freeAllWaiting();
    thread_dispatch();
}

void TCB::dispatch(){
    TCB* old = running;
    if(!old->isFinished() && !old->blocked){
        Scheduler::instanceof().addFirst(old);
    };
    running = Scheduler::instanceof().removeLast();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::putToWait() {
    if(this->isFinished()) return;
    ElemWait* novi = (ElemWait*)MemoryAllocator::instanceOf().mem_alloc((sizeof(ElemWait) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    novi->elem = TCB::running;
    novi->next = head;
    head = novi;
    TCB::running->blocked = true;
    thread_dispatch();
    //dispatch();
}

void TCB::freeAllWaiting() {
    while(head != nullptr){
        ElemWait* tek = head;
        head = head->next;
        TCB* tekTCB = tek->elem;
        MemoryAllocator::instanceOf().mem_free(tek);
        tekTCB->blocked = false;
        Scheduler::instanceof().addFirst(tekTCB);
    }
}


void TCB::addToSleep(TCB *tcb, uint64 time) {
    if(time == 0) return;
    ElemSleep* novi = (ElemSleep*)MemoryAllocator::instanceOf().mem_alloc((sizeof(ElemSleep) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    novi->next = headSleep;
    novi->elem = tcb;
    novi->sleepTime = time;
    headSleep = novi;
    tcb->blocked = true;
    thread_dispatch();
    //dispatch();
}

void TCB::checkForWaking() {
    if(headSleep == nullptr) return;
    ElemSleep* tek = headSleep, *prev = nullptr;
    bool changed = false;
    while(tek){
        if(--tek->sleepTime == 0){
            if(tek == headSleep) headSleep = headSleep->next;
            ElemSleep* old = tek;
            tek = tek->next;
            if(prev) prev->next = old->next;
            TCB* tcb = old->elem;
            MemoryAllocator::instanceOf().mem_free(old);

            tcb->blocked = false;
            Scheduler::instanceof().addFirst(tcb);
            changed = true;

        }
        if(!changed) {
            prev = tek;
            tek = tek->next;
        }
        changed = false;
    }
}