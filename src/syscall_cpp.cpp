#include "../h/syscall_cpp.hpp"
#include "../h/Scheduler.hpp"

void* operator new(size_t size){
    return mem_alloc(size);
}

void operator delete(void* ptr){
    mem_free(ptr);
}

Thread::Thread(void (*body)(void *), void *arg): body(body), arg(arg) {
    thread_create(&myHandle, body, arg);
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    Scheduler::instanceof().addFirst(myHandle);
    return 0;
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Thread::wrapper(void *arg) {
    Thread* thr = (Thread*) arg;
    thr->run();
}

Thread::Thread() {
    thread_create_without_scheduler(&myHandle, wrapper, this);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char a) {
    ::putc(a);
}

void PeriodicThread::terminate() {
    work = false;
}

PeriodicThread::PeriodicThread(time_t period) {
    this->period = period;
}

void PeriodicThread::run() {
    while(work){
        this->periodicActivation();
        time_sleep(this->period);
    }
}
