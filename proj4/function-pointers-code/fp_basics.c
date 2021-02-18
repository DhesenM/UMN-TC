// fp_basics: shows basics of creating a simple function pointer,
// assigning it different functions, and calling it.
#include <stdio.h>

int increment(int x){                    // a function that increments
  return x+1;
}

int triple(int x){                       // a function that triples
  return 3*x;
}

int main(int argc, char *argv[]){
  printf("main:      %p\n", main);       // print the 'value' associated with main, its address

  int (*func_ptr)(int) = NULL;           // initialize function pointer to NULL
  //| | |         +-> 1 arg, an int
  //| | +->name of pointer variable 
  //| +-> is a pointer to a function
  //+-> return type of function pointed to

  printf("func_ptr:  %p\n", func_ptr);

  func_ptr = increment;                  // point instead to increment()
  printf("increment: %p\n", increment);
  printf("func_ptr:  %p\n", func_ptr);
  int reti = func_ptr(3);                // call function via pointer
  printf("func_ptr(3) = %d\n", reti);

  func_ptr = triple;                     // change function pointer to triple()
  printf("triple:    %p\n", triple);
  printf("func_ptr:  %p\n", func_ptr);
  int rett = func_ptr(3);                // call function via pointer
  printf("func_ptr(3) = %d\n", rett);

  return 0;
}
   
