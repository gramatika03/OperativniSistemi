#include "../h/BufferCon.hpp"

//Uzeto iz test primera
BufferCon::BufferCon(int _cap) : cap(_cap), head(0), tail(0) {
    buffer = (char *)mem_alloc(sizeof(char) * cap);
    itemAvailable = SemaphoreC::createSemaphore(0);
    spaceAvailable = SemaphoreC::createSemaphore(_cap);
}

BufferCon::~BufferCon() {
    while (getCnt() > 0) {
        char ch = buffer[head];
        putc(ch);
        head = (head + 1) % cap;
    }

    MemoryAllocator::instanceOf().mem_free(buffer);
    delete itemAvailable;
    delete spaceAvailable;
}

void BufferCon::put(char val) {
    spaceAvailable->wait();

    buffer[tail] = val;
    tail = (tail + 1) % cap;

    itemAvailable->signal();

}

char BufferCon::get() {
    itemAvailable->wait();

    char ret = buffer[head];
    head = (head + 1) % cap;
    spaceAvailable->signal();

    return ret;
}

int BufferCon::getCnt() {
    int ret;

    if (tail >= head) ret = tail - head;
    else ret = cap - head + tail;

    return ret;
}
