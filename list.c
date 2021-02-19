
#include <stdio.h>
#include <block.h>
#include <list.h>
#include <utils.h>
#include <math.h>

List* createList() {
  List* newList = (List*) calloc(1, sizeof(List));
  newList->listSize = 0;
  newList->blocks = NULL;
  newList->totalBlocks = 0;

  return newList;
}

void freeBlockArray(List* list) {
  if(list) {
    uint64_t totalBlocks = list->totalBlocks;
    for(uint64_t i = 0; i < totalBlocks; ++i) {
      freeBlock(list->blocks[i]);
    }
    free(list->blocks);
  }
}

void freeList(List* list) {
  freeBlockArray(list);
  free(list);
}

void printListDetails(List* list) {
  if(list) {
    printf(
      "[ LIST INFO ]\n"
      " List Size: %llu\n"
      " List Total Blocks: %llu\n"
      " Blocks Array Address: %p\n"
      , list->listSize
      , list->totalBlocks
      , list->blocks
    );
  } else {
    printf("List is not initialized! [NULL]\n");
  }
}

void resizeBlockArray(List* list, uint64_t newSize) {
  if(list) {
    Block** newBlocks = (Block**) calloc(newSize, sizeof(Block*));
    uint64_t copySize = list->totalBlocks <= newSize ? list->totalBlocks : newSize;
    
    for(uint64_t i = 0; i < copySize; ++i) {
      newBlocks[i] = list->blocks[i];
    }
    
    free(list->blocks);
    
    newBlocks[newSize - 1] = createBlock((uint64_t)pow(2, newSize - 1));
    
    list->blocks = newBlocks;
    list->totalBlocks = newSize;
  }
}

void listInsert(List* list, void* data, uint64_t dataSize) {
  if(list) {
//    printf("Data Size: %llu\n", dataSize);
//    printf("Capacity: %llu\n", (uint64_t)pow(2, list->totalBlocks - 1));
    
    if(list->blocks) {
      if(list->listSize == (uint64_t)pow(2, list->totalBlocks - 1)) {
        resizeBlockArray(list, list->totalBlocks + 1);
      }
    } else {
      resizeBlockArray(list, 1);
    }
    
    ++list->listSize;
    
    // Some factor work.
    uint64_t index = log2(list->listSize);
    uint64_t memoryBlockIndex = list->listSize - pow(2, index);
    
    //printListDetails(list);
    
    //printf(" Index: %llu\n Memory Block Index: %llu\n", index, memoryBlockIndex);
    
    defaultBlock(&list->blocks[index]->memoryBlocks[memoryBlockIndex]);
    copyMemoryToBlock(&list->blocks[index]->memoryBlocks[memoryBlockIndex], data, dataSize);
    
    //printf(" DONE INSERT!\n\n");
  }
}

MemoryBlock* listGet(List* list, uint64_t elemIndex) {
  if(list) {
    if(elemIndex < list->listSize) {
      ++elemIndex;
      
      uint64_t index = log2(elemIndex);
      uint64_t memoryBlockIndex = elemIndex - pow(2, index);
      
      return &list->blocks[index]->memoryBlocks[memoryBlockIndex];
    }
  }
  return NULL;
}

void listRemove(List* list) {
  if(list && list->listSize > 0) {
    uint64_t index = log2(list->listSize);
    uint64_t memoryBlockIndex = list->listSize - pow(2, index);
    
    MemoryBlock* end = listGet(list, list->listSize - 1);
    freeAllocatedMemoryBlockSpace(end);
    end->address = NULL;
    end->size = 0;
    
    --list->listSize;
  }
}
