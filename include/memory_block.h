
#if !defined(__MEMORY_BLOCK_H)
#define __MEMORY_BLOCK_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  void* address;
  uint64_t size;
} MemoryBlock;

MemoryBlock* createMemoryBlock(uint64_t size);
void defaultBlock(MemoryBlock* memoryBlock);
void freeAllocatedMemoryBlockSpace(MemoryBlock* memoryBlock);
void freeMemoryBlock(MemoryBlock* memoryBlock);
bool copyMemoryToBlock(MemoryBlock* memoryBlock, void* memory, uint64_t memorySize);

#endif
