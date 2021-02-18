// sort_float.c: Demonstrates several interesting principles.
// 
// 1. Shows how to typedef a 'convenience' type for a function pointer
// then caste several functions with differing argument types with the
// convenience type.
//
// 2. Establishes an struct with a function pointer as a field and
// populates it with other functions.
//
// 3. Shows how to iterate over the struct array and call all the
// functions pointed to in the table.
//
// 4. Further demonstrates use of the library qsort() function which
// provides a general purpose quicksort routine which only needs a
// data array, size information, and a comparison function to work on
// data.
//
// 5. Demonstrates an earlier property of IEEE 754 floating point
// numbers: their carefully selected bit layout allows them to be
// sorted via integer comparisons.  A floating point array is
// established but the intcmp / intcmp_r routines can sort
// them. Generally integer operations take less time than floating
// point operations allowing for floating point arrays to be sorted
// faster via this 'type punning' technique. Note that this only works
// for all positive floating point numbers; negative floats sort a bit
// differently when it interpretted as integers.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int intcmp(int *xp, int *yp){           // compare two integers
  return -(*xp < *yp) + (*xp > *yp);    // a bit of trickiness to do int comparisons and generate -1,0,+1
}

int intcmp_rev(int *xp, int *yp){       // reverse integer compare
  return -(-(*xp < *yp) + (*xp > *yp)); 
}

int floatcmp(float *xp, float *yp){     // compare two floating point values
  return -(*xp < *yp) + (*xp > *yp);    // a bit of trickiness to do int comparisons and generate -1,0,+1
}

int floatcmp_rev(float *xp, float *yp){ // reverse float compare
  return -(-(*xp < *yp) + (*xp > *yp)); 
}

// TYPEDEF FOR FUNCTION POINTER
// 
// Convenience type to enable casting of typed comparisong functions
// to the void-ish functions expected by qsort(). The preceding
// int/float commparison functions can be caste to the (cmp_t) type to
// pass them to qsort().
// 
typedef int (*cmp_t) (const void *, const void *);
//      |    ||       |             |
//      |    ||       |             +-> 2nd arg (const void *)
//      |    ||       +-> 1st arg (const void *)    
//      |    |+-> 'cmp_t' is name of new type
//      |    +-> type is a pointer to a function
//      +-> function returns an integer

typedef struct {                        // type to hold combo of string name and function pointer
  char *name;                           // name of the comparison to use
  cmp_t func;                           // function to use, takes (void*) args, returns an int
} compare_func_t;

compare_func_t cfuncs[] = {                            // table of comparison functions
  {.name="intcmp"       , .func=(cmp_t) intcmp      }, // castes required to coerce
  {.name="intcmp_rev"   , .func=(cmp_t) intcmp_rev  }, // different types of functions
  {.name="floatcmp"     , .func=(cmp_t) floatcmp    }, // to unifying type of struct
  {.name="floatcmp_rev" , .func=(cmp_t) floatcmp_rev}, // field
  {.name=NULL}                                         // 'null' terminate the table
};

int main(int argc, char *argv[]){

  for(int i=0; cfuncs[i].name != NULL; i++){           // iterate over table of comparison functions
    float arr[10] =                                    // array of floating point numbers
      {1.1, 6.6, 5.5, 2.2, 3.3,                        // randomly ordered each iteration
       9.0, 4.4, 7.7, 8.8, 0.5}; 
    printf("sorting using '%s'\n", cfuncs[i].name);    // print name of function

    qsort((void*) arr, 10, sizeof(float),              // same 3 args each iter to qsort()
          cfuncs[i].func);                             // last arg varies: use a different compare func

    for (int i = 0; i < 10; ++i){                      // print out the array as sorted by
      printf("%.1f ", arr[i]);                         // the qsort()/compare call above
    }
    printf("\n");
  }
  return 0; 
} 
