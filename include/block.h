
#if !defined(__BLOCK_H)
#define __BLOCK_H

#include <stdlib.h>
#include <memory_block.h>

typedef struct {
  uint64_t blockSize;
  MemoryBlock* memoryBlocks;
} Block;

Block* createBlock(uint64_t blockSize);

void freeBlock(Block* block);

#endif
