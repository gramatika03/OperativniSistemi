#ifndef PROJEKAT_H_BUFFER
#define PROJEKAT_H_BUFFER

#include "../h/syscall_c.h"
#include "../test/printing.hpp"

//buffer uzet iz datih test primera, prilagodjen upotrebi u kodu
class BufferCon {
private:
    int cap;
    char *buffer;
    int head, tail;

    sem_t spaceAvailable;
    sem_t itemAvailable;

public:
    BufferCon(int _cap);
    ~BufferCon();

    void put(char val);
    char get();

    int getCnt();

};


#endif

