#include "../h/ConsoleCon.hpp"

ConsoleCon &ConsoleCon::instanceOf() {
    static ConsoleCon instanca;
    return instanca;
}

void ConsoleCon::outputBufferPut(char c) {
    outputBuffer->put(c);
}

void ConsoleCon::emptyBufferOut() {
    while(outputBuffer->getCnt() != 0){
        thread_dispatch();
    }
}

char ConsoleCon::inputBufferGet() {
    return inputBuffer->get();
}

void ConsoleCon::inputBufferPut(char c) {
    inputBuffer->put(c);
}

char ConsoleCon::outputBufferGet() {
    return outputBuffer->get();
}

void putcBody(void* arg){
    while(true){
        while((*(char*)CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT){
            *(char*)CONSOLE_TX_DATA = ConsoleCon::instanceOf().outputBufferGet();
        }
    }
}