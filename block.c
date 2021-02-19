
#include <include/block.h>

Block* createBlock(uint64_t blockSize) {
  Block* newBlock = (Block*) calloc(1, sizeof(Block));
  newBlock->blockSize = blockSize;
  newBlock->memoryBlocks = (MemoryBlock*) calloc(blockSize, sizeof(MemoryBlock));
  
  return newBlock;
}

void freeBlock(Block* block) {
  if(block && block->memoryBlocks) {
    for(int i = 0; i < block->blockSize; ++i) {
      freeAllocatedMemoryBlockSpace(&block->memoryBlocks[i]);
    }
    free(block->memoryBlocks);
  }
  free(block);
}