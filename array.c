
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stdint.h>

#include <string.h>

#include <math.h>

//#include "leak_detector_c.h"

typedef struct {
  void* address;
  size_t size;
} MemoryBlock;

MemoryBlock* createMemoryBlock(size_t size) {
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

bool copyMemoryToBlock(MemoryBlock* memoryBlock, void* memory, size_t memorySize) {
  if(memoryBlock) {
    freeAllocatedMemoryBlockSpace(memoryBlock);
    memoryBlock->address = calloc(1, memorySize);
    memoryBlock->size = memorySize;
    
    //printf("Copying... Size: %llu\n", memorySize);

    memcpy(memoryBlock->address, memory, memorySize);
  } else {
    return false;
  }
}

typedef struct {
  size_t blockSize;
  MemoryBlock* memoryBlocks;
} Block;

Block* createBlock(size_t blockSize) {
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

typedef struct {
  size_t listSize;
  Block** blocks;
  size_t totalBlocks;
} List;

List* createList() {
  List* newList = (List*) calloc(1, sizeof(List));
  newList->listSize = 0;
  newList->blocks = NULL;
  newList->totalBlocks = 0;

  return newList;
}

void freeBlockArray(List* list) {
  if(list) {
    size_t totalBlocks = list->totalBlocks;
    for(size_t i = 0; i < totalBlocks; ++i) {
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

void resizeBlockArray(List* list, size_t newSize) {
  if(list) {
    Block** newBlocks = (Block**) calloc(newSize, sizeof(Block*));
    size_t copySize = list->totalBlocks <= newSize ? list->totalBlocks : newSize;
    
    for(size_t i = 0; i < copySize; ++i) {
      newBlocks[i] = list->blocks[i];
    }
    
    free(list->blocks);
    
    newBlocks[newSize - 1] = createBlock((size_t)pow(2, newSize - 1));
    
    list->blocks = newBlocks;
    list->totalBlocks = newSize;
  }
}

double log2(double x) {
  return log(x) / log(2);
}

void listInsert(List* list, void* data, size_t dataSize) {
  if(list) {
//    printf("Data Size: %llu\n", dataSize);
//    printf("Capacity: %llu\n", (size_t)pow(2, list->totalBlocks - 1));
    
    if(list->blocks) {
      if(list->listSize == (size_t)pow(2, list->totalBlocks - 1)) {
        resizeBlockArray(list, list->totalBlocks + 1);
      }
    } else {
      resizeBlockArray(list, 1);
    }
    
    ++list->listSize;
    
    // Some factor work.
    size_t index = log2(list->listSize);
    size_t memoryBlockIndex = list->listSize - pow(2, index);
    
    //printListDetails(list);
    
    //printf(" Index: %llu\n Memory Block Index: %llu\n", index, memoryBlockIndex);
    
    defaultBlock(&list->blocks[index]->memoryBlocks[memoryBlockIndex]);
    copyMemoryToBlock(&list->blocks[index]->memoryBlocks[memoryBlockIndex], data, dataSize);
    
    //printf(" DONE INSERT!\n\n");
  }
}

MemoryBlock* listGet(List* list, size_t elemIndex) {
  if(list) {
    if(elemIndex < list->listSize) {
      ++elemIndex;
      
      size_t index = log2(elemIndex);
      size_t memoryBlockIndex = elemIndex - pow(2, index);
      
      return &list->blocks[index]->memoryBlocks[memoryBlockIndex];
    }
  }
  return NULL;
}

void listRemove(List* list) {
  if(list && list->listSize > 0) {
    size_t index = log2(list->listSize);
    size_t memoryBlockIndex = list->listSize - pow(2, index);
    
    MemoryBlock* end = listGet(list, list->listSize - 1);
    freeAllocatedMemoryBlockSpace(end);
    end->address = NULL;
    end->size = 0;
    
    --list->listSize;
  }
}

#define cast(type, address) ((type)address)

/*
  Test Struct
*/
typedef struct {
  char name[128];
  size_t age, weight;
} Student;

void printStudent(Student* student) {
  if(student) {
    printf(
      "Name: %s\n"
      "Age: %llu\n"
      "Weight: %llu\n"
      , student->name
      , student->age
      , student->weight);
  }
}

/*
  MAIN FUNCTION!
  
  THIS IS WHERE THE PROGRAM GETS STARTED BOIS!
*/
int main() {
  //atexit(report_mem_leak);
  List* list = createList();
  
  // Insert an integer
  int number = 10;
  listInsert(list, &number, sizeof(number));
  
  // Insert a string
  char* string = "Hello World!";
  listInsert(list, string, strlen(string) + 1);
  
  // Insert a double
  double pi = 3.14159;
  listInsert(list, &pi, sizeof(pi));
  
  // Insert a student
  Student s = {
    .name = "John Doe",
    .age = 19,
    .weight = 80
  };
  listInsert(list, &s, sizeof(s));
  
  printf("int = %d\n", *cast(int*, listGet(list, 0)->address));
  printf("char* = %s\n", cast(char*, listGet(list, 1)->address));
  printf("double = %lf\n\n", *cast(double*, listGet(list, 2)->address));
  printStudent(cast(Student*, listGet(list, 3)->address));
  
  freeList(list);
  return 0;
}
