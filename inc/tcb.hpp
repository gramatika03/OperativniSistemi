#ifndef PROJEKAT_H_TCB
#define PROJEKAT_H_TCB

#include "../lib/hw.h"



class TCB{
private:

    struct Context{
        uint64 ra;
        uint64 sp;
    };


    struct ElemWait{
        TCB* elem;
        ElemWait* next;
    };

    struct ElemSleep{
        TCB* elem;
        ElemSleep* next;
        uint64 sleepTime;
    };
    typedef void(*Body)(void*);

    Context context;
    Body body;
    bool finished;
    uint64 userStack;
    void* argument;
    uint64 stack;
    uint64 userSP;
    //bool waiting;
    uint64 timeSlice;
    //bool sleeping;

    static uint64 timeSliceCounter;

    ElemWait* head;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

public:

    static ElemSleep* headSleep;


    static TCB* running;

    static TCB* createThread(Body body, void* arg, char* stack, char* kernelStack);

    bool isFinished(){return finished;}

    void setFinished(bool val){finished = val;}

    static void yield();

    static void dispatch();

    void setBlocked(bool val){blocked = val;}

    bool getBlocked(){return blocked;}

    void putToWait();

    void freeAllWaiting();

    uint64 getTimeSlice(){return timeSlice;}

    friend class Riscv;

    static void checkForWaking();
    static void addToSleep(TCB *tcb, uint64 time);

    bool blocked;
};

#endif