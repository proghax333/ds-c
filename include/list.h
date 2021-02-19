
#if !defined(__LIST_H)
#define __LIST_H

#include <block.h>

typedef struct {
  uint64_t listSize;
  Block** blocks;
  uint64_t totalBlocks;
} List;

List* createList();
void freeBlockArray(List* list);
void freeList(List* list);

void printListDetails(List* list);
void resizeBlockArray(List* list, uint64_t newSize);

void listInsert(List* list, void* data, uint64_t dataSize);
MemoryBlock* listGet(List* list, uint64_t elemIndex);
void listRemove(List* list);

#endif
