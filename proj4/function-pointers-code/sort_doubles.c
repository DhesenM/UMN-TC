// sort_doubles.c: Somewhat simpler demo of using function
// pointers. Shows to functions that are caste to the same type
// (cmp_t) when assigned to a pointer. qsort() is used to to sort
// doubles using these comparison functions.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int longcmp(long *xp, long *yp) {       // compare longs
  return -(*xp < *yp) + (*xp > *yp);    // a bit of trickiness to do int comparisons and generate -1,0,+1
}

int doubcmp(double *xp, double *yp) {   // compare doubles
  return -(*xp < *yp) + (*xp > *yp);    // a bit of trickiness to generate -1,0,+1
}

// convenience type to enable casting of typed comparisong functions
// to the void-ish functions expected by qsort()
typedef int (*cmp_t) (const void *, const void *);

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("usage: %s {long|doub}\n",argv[0]);
    printf("specify whether to sort using 'long' comparisons or 'doub'-le comparisons\n");
    return 0;
  }
    
  cmp_t compare = NULL;               // function pointer
  if( strcmp(argv[1],"long" )==0 ){   // sort as longs
    compare = (cmp_t) longcmp;
  }
  else {
    compare = (cmp_t) doubcmp;
  }
    
  double arr[10] = {1.1, 6.6, 5.5, 2.2, 3.3, 9.0, 4.4, 7.7, 8.8, 0.5}; 
  qsort((void*) arr, 10, sizeof(double), compare); // sort using compare() function
    
  printf("sorted array: \n");                      // print sorted array
  for (int i = 0; i < 10; ++i) {
    printf("%.1f ", arr[i]);
  }
  printf("\n");

  return 0; 
} 
