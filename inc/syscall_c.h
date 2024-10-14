//z niti) 2 C API funkcije kao običnog potprograma.
// Created by os on 7/29/23.
//

#ifndef PROJEKAT_SYSCALL_C_H
#define PROJEKAT_SYSCALL_C_H

#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "tcb.hpp"
#include "MemoryAllocator.hpp"
#include "SemaphoreC.hpp"

typedef TCB* thread_t;
typedef SemaphoreC* sem_t;
typedef unsigned long time_t;

void callInterrupt(...);

//Memory Allocator
void* mem_alloc(size_t size);
int mem_free(void* ptr);

//Thread
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t handle);
int time_sleep(time_t time);
int thread_create_without_scheduler(thread_t* handle, void(*start_routine)(void*), void* arg);

//Semaphore
int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

char getc();
void putc(char);

void switchToUserRegime();

#endif //PROJEKAT_SYSCALL_C_H
