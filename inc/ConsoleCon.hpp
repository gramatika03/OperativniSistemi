#ifndef PROJEKAT_H_CONSOLECON
#define PROJEKAT_H_CONSOLECON

#include "../h/BufferCon.hpp"

class ConsoleCon{
private:
    BufferCon* inputBuffer;
    BufferCon* outputBuffer;

    ConsoleCon(){
        inputBuffer = new BufferCon(256);
        outputBuffer = new BufferCon(256);
    }

public:
    char outputBufferGet();
    void outputBufferPut(char c);

    char inputBufferGet();
    void inputBufferPut(char c);

    void emptyBufferOut();

    static ConsoleCon& instanceOf();
};

#endif