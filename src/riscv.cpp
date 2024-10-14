//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/Scheduler.hpp"
#include "../test/printing.hpp"
#include "../h/ConsoleCon.hpp"

//Uzeto sa vezbi 7

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSoftwareInterrrupt() {
    uint64 fun = r_a0();
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        //mem_alloc
        if (fun == 0x01) {
            uint64 param = r_a1();
            void *ret = MemoryAllocator::instanceOf().mem_alloc(param);
            w_a0((uint64) ret);
        }
        //mem_free
        else if (fun == 0x02) {
            uint64 param = r_a1();
            int ret = MemoryAllocator::instanceOf().mem_free((void *) param);
            w_a0(ret);
        }
        //thread_create
        else if (fun == 0x11) {
            thread_t *handle = (thread_t *) r_a1();
            Body startRoutine = (Body) r_a2();
            void *arg = (void *) r_a3();

            char *stack = (char *) MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
            char *kernelStack = (char *) MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);

            TCB *a = TCB::createThread(startRoutine, arg, stack, kernelStack);
            *handle = a;

            if (startRoutine != nullptr) {
                Scheduler::instanceof().addFirst(*handle);
            }

            if (*handle == nullptr) w_a0(-1);
            else w_a0(0);
        }
        //thread_exit
        else if (fun == 0x12) {
            TCB::running->setFinished(true);
            TCB::running->freeAllWaiting();
            TCB::dispatch();
        }
        //thread_dispatch
        else if (fun == 0x13) {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
        }
        //thread_join
        else if (fun == 0x14) {
            TCB *handle = (TCB *) r_a1();
            handle->putToWait();
        }
        //sem_open
        else if (fun == 0x21) {
            SemaphoreC **handle = (SemaphoreC **) r_a1();
            unsigned int val = (unsigned int) r_a2();
            SemaphoreC *sem = SemaphoreC::createSemaphore(val);
            *handle = sem;

            if (sem == nullptr) w_a0(-1);
            else w_a0(0);
        }
        //sem_close
        else if (fun == 0x22) {
            SemaphoreC *handle = (SemaphoreC *) r_a1();
            handle->freeAll();
            int ret = MemoryAllocator::instanceOf().mem_free(handle);

            if (ret < 0) w_a0(-1);
            else w_a0(0);
        }
        //sem_wait
        else if (fun == 0x23) {
            SemaphoreC *sem = (SemaphoreC *) r_a1();
            if(sem == nullptr) w_a0(-1);

            sem->wait();
        }
        //sem_signal
        else if (fun == 0x24) {
            SemaphoreC *sem = (SemaphoreC *) r_a1();
            if(sem == nullptr) w_a0(-1);

            sem->signal();
        }
        //time_sleep
        else if (fun == 0x31) {
            time_t time = (time_t) r_a1();
            TCB::addToSleep(TCB::running, time);
        }
        //getc
        else if (fun == 0x41) {
            //char c = __getc();
            char c = ConsoleCon::instanceOf().inputBufferGet();
            w_a0((uint64) c);
        }
        //putc
        else if (fun == 0x42) {
            uint64 k = r_a1();
            char c = (char) k;
            //__putc(k);
            ConsoleCon::instanceOf().outputBufferPut(c);
        }
        //thread_create_without_scheduler
        else if (fun == 0x81) {
            thread_t *handle = (thread_t *) r_a1();
            Body startRoutine = (Body) r_a2();
            void *arg = (void *) r_a3();

            char *stack = (char *)MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
            char *kernelStack = (char *) MemoryAllocator::instanceOf().mem_alloc((DEFAULT_STACK_SIZE + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);

            TCB *a = TCB::createThread(startRoutine, arg, stack, kernelStack);
            *handle = a;

            if (*handle == nullptr) w_a0(-1);
            else w_a0(0);
        }
        //change_regime
        else if (fun == 0x82) {
            w_sstatus(sstatus);
            mc_sstatus(Riscv::SSTATUS_SPP);
            w_sepc(sepc);
            return;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }else{
        printInt(scause);
    }
}

void Riscv::handleTimerInterrupt() {    //po ugledu sa vezbi
    mc_sip(SIP_SSIP);
    TCB::checkForWaking();
    TCB::timeSliceCounter++;
    if(TCB::running && TCB::timeSliceCounter >= TCB::running->getTimeSlice()){
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
};

void Riscv::handleConsoleInterrupt() {
    int fun = plic_claim();
    if(fun == CONSOLE_IRQ){
        char status = *(char*)CONSOLE_STATUS;
        while(status & CONSOLE_RX_STATUS_BIT){
            char ch = *(char*)CONSOLE_RX_DATA;
            ConsoleCon::instanceOf().inputBufferPut(ch);
            status = *(char*)CONSOLE_STATUS;
        }
    }
    plic_complete(fun);
}