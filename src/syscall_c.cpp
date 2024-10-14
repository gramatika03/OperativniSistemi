//
// Created by os on 7/29/23.
//

#include "../h/syscall_c.h"

void callInterrupt(...){
    __asm__ volatile ("ecall");
}

void* mem_alloc(size_t size){
    uint64 numOfBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    callInterrupt(0x01, numOfBlocks);

    return (void*)Riscv::r_a0();
}

int mem_free(void* ptr){
    callInterrupt(0x02, ptr);

    return Riscv::r_a0();
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){

    callInterrupt(0x11, handle, start_routine, arg);

    return Riscv::r_a0();
}

int thread_exit(){

    callInterrupt(0x12);

    return Riscv::r_a0();
}

void thread_dispatch(){

    callInterrupt(0x13);

}

int sem_open(sem_t* handle, unsigned init) {
    callInterrupt(0x21, handle, init);

    return Riscv::r_a0();
}

int sem_close(sem_t handle){

    callInterrupt(0x22, handle);

    return Riscv::r_a0();
}

int sem_wait(sem_t id){

    callInterrupt(0x23, id);

    return Riscv::r_a0();
}

int sem_signal(sem_t id){

    callInterrupt(0x24, id);

    return Riscv::r_a0();
}

void thread_join(thread_t handle){

    callInterrupt(0x14, handle);

}

int time_sleep(time_t time){
    callInterrupt(0x31, time);
    return 0;
}

int thread_create_without_scheduler(thread_t* handle, void(*start_routine)(void*), void* arg){
    callInterrupt(0x81, handle, start_routine, arg);

    return Riscv::r_a0();
}

void switchToUserRegime(){
    callInterrupt(0x82);
}

char getc() {
    callInterrupt(0x41);
    return Riscv::r_a0();
}

void putc(char a){
    callInterrupt(0x42, a);
}