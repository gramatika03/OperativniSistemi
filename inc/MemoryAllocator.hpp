#ifndef PROJEKAT_MEMORYALLOCATOR_H
#define PROJEKAT_MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator{
private:
    struct MemBlock{
        MemBlock* next;
        MemBlock* prev;
        size_t sz;
    };
    MemBlock* head;
    void joinSegments(MemBlock* first, MemBlock* second);
public:
    MemoryAllocator(){
        head = (MemBlock*)HEAP_START_ADDR;
        head->next = nullptr;
        head->sz = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR - sizeof(MemBlock);
    }
    void* mem_alloc(size_t size);
    int mem_free(void* ptr);
    static MemoryAllocator& instanceOf();
};

#endif
