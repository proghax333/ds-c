

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <memory_block.h>

MemoryBlock* createMemoryBlock(uint64_t size) {
  MemoryBlock* newMemoryBlock = (MemoryBlock*) calloc(1, sizeof(MemoryBlock));
  newMemoryBlock->address = calloc(1, size);
  newMemoryBlock->size = size;

  return newMemoryBlock;
}

void defaultBlock(MemoryBlock* memoryBlock) {
  if(memoryBlock) {
    memoryBlock->address = NULL;
    memoryBlock->size = 0;
  }
}

void freeAllocatedMemoryBlockSpace(MemoryBlock* memoryBlock) {
  if(memoryBlock != NULL) {
    free(memoryBlock->address);
  }
}

void freeMemoryBlock(MemoryBlock* memoryBlock) {
  freeAllocatedMemoryBlockSpace(memoryBlock);
  free(memoryBlock);
}

bool copyMemoryToBlock(MemoryBlock* memoryBlock, void* memory, uint64_t memorySize) {
  if(memoryBlock) {
    freeAllocatedMemoryBlockSpace(memoryBlock);
    memoryBlock->address = calloc(1, memorySize);
    memoryBlock->size = memorySize;

    memcpy(memoryBlock->address, memory, memorySize);
    return true;
  } else {
    return false;
  }
}
