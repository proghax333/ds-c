
#include <stdio.h>
#include <list.h>
#include <utils.h>

/*
  Test Struct
*/
typedef struct {
  char name[128];
  uint64_t age, weight;
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
