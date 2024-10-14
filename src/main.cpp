//
// Created by os on 7/26/23.
//

#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../h/Scheduler.hpp"
#include "../test/printing.hpp"
#include "../h/BufferCon.hpp"
#include "../h/ConsoleCon.hpp"


class Klasa: public PeriodicThread{
public:
    void periodicActivation() override{
        putc('a');
    }
    Klasa(time_t per): PeriodicThread(per){}
};

class Klasa1: public PeriodicThread{
public:
    void periodicActivation() override{
        putc('b');
    }
    Klasa1(time_t per): PeriodicThread(per){}
};

void fun(void*){}

void putcBody(void*);
void userMain();
int main() {

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap | 1); //dozvoljavanje vektorisanog prekida i upis u stvec registra
                                                        //adrese funkcije na koju se skace

    TCB* threadMain;
    thread_create(&threadMain, nullptr, nullptr);
    TCB::running = threadMain;  //pravljenje main niti

    //pravljenje niti za ispis
    TCB* c = TCB::createThread(putcBody, nullptr,
                               (char *) MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE),
                               (char *) MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE));
    Scheduler::instanceof().addFirst(c);
    ConsoleCon::instanceOf();

    switchToUserRegime();
    userMain();
    /*Klasa* k = new Klasa(20);
    k->start();
    time_sleep(100);
    k->terminate();
    k->join();*/

    ConsoleCon::instanceOf().emptyBufferOut();

    delete threadMain;
    delete c;

    return 0;
}