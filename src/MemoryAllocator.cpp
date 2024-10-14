//
// Created by os on 6/6/23.
//


#include "../h/MemoryAllocator.hpp"

MemoryAllocator& MemoryAllocator::instanceOf() {
    static MemoryAllocator singleton;
    return singleton;
}

void MemoryAllocator::joinSegments(MemBlock* first, MemBlock* second){
    if(first == nullptr || second == nullptr) return;
    if((char*) first + sizeof(MemBlock) + first->sz != (char *) second) return;
    //join first and second segment
    first->next = second->next;
    if(second->next) second->next->prev= first;
    first->sz = first->sz + second->sz + sizeof(MemBlock);
}


int MemoryAllocator::mem_free(void *ptr) {  //po ugledu na zadatak sa kolokvijuma
    if(ptr == 0) return 0;


    MemBlock* ptrf = (MemBlock*) ((char*)ptr - sizeof(MemBlock));
    MemBlock* cur = head;

    //check to see if segment should be added at the beginning
    if(cur == nullptr){
        head = ptrf;
    }
    else if((char*)ptrf < (char*)cur){
        ptrf->next = head;
        head->prev = ptrf;
        ptrf->prev = nullptr;
        head = ptrf;
    }else {
        //check to see after which segment new segment should be added
        MemBlock* prev = nullptr;
        while(cur && (uint64)ptrf > (uint64)cur) {
            prev = cur;
            cur = cur->next;
        }
        prev->next = ptrf;
        ptrf->prev = prev;
        ptrf->next = cur;
        if(cur) cur->prev = ptrf;
    }

    joinSegments(ptrf, ptrf->next);
    joinSegments(ptrf->prev, ptrf);
    return 0;
}


void *MemoryAllocator::mem_alloc(size_t size) { //po ugledu na zadatak sa kolokvijuma
    if(size <= 0) return nullptr;
    size = size * MEM_BLOCK_SIZE;
    MemBlock* cur  = head;
    while(cur && cur->sz < size) {
        cur = cur->next;
    }

    //cur is either null(not enough memory available) or pointer to a MemBlock big enough
    if(cur == nullptr) return nullptr;

    MemBlock* nextBlk = cur->next, *prevBlk = cur->prev;

    size_t remainingSize = cur->sz - size;
    //check to see if remained number of bytes is bigger than size of one MemBlock
    if(remainingSize < MEM_BLOCK_SIZE + sizeof(MemBlock)) {
        if(prevBlk) prevBlk->next = nextBlk;
        else head = cur->next;
        if(nextBlk) nextBlk->prev = prevBlk;
    }else{
        cur->sz = size;
        MemBlock* newBlk = (MemBlock*)((char*)cur + size + sizeof(MemBlock));
        newBlk->next = nextBlk;
        newBlk->prev = prevBlk;
        newBlk->sz = remainingSize - sizeof(MemBlock);
        if(prevBlk) prevBlk->next = newBlk;
        else head = newBlk;
        if(nextBlk) nextBlk->prev = newBlk;
    }

    cur->next = nullptr;
    cur->prev = nullptr;

    return (char*)cur + sizeof(MemBlock);
}