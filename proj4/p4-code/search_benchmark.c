#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "search.h"

// print these if the input blank or the function fails
void fail() {
  printf("usage: ./search_benchmark <minpow> <maxpow> <repeats> [which]\n");
  printf(" which is a combination of:\n");
  printf("  a : Linear Array Search\n");
  printf("  l : Linked List Search\n");
  printf("  b : Binary Array Search\n");
  printf("  t : Binary Tree Search\n");
  printf("  (default all)\n");
}

// main function for Timing Search Algorithms
int main(int argc, char *argv[]) {
  
  if(argc < 4 || argc > 5) {     // fails and return 0 if the number of inputs < 4 or > 5
    fail();
    return 0;
  }

  for(int i = 1; i < 4; i++) {   // fails and return 0 if the first three inputs are not integer
    if(atoi(argv[i]) == 0){
      fail();
      return 0;
    }
  }

  int run_linear_array = 0;      // initialize variables that will determine the types of timing search
  int run_binary_search = 0;
  int run_linked_list_search = 0;
  int run_tree_search = 0;
  int minpow = atoi(argv[1]);    // initialize the first three arguments as minpow, maxpow and repeats
  int maxpow = atoi(argv[2]);
  int repeats = atoi(argv[3]);
  
  if(minpow <= 0 || maxpow < minpow || repeats <= 0) {
    fail();                      // fails and return 0 if the inputs < 0 or maxpow < minpow
    return 0;
  }
  
  printf(" %6s ","LENGTH");      // print LENGTH and SEARCHES in the title line, 
  printf("%8s ","SEARCHES");     // which needs to be printed out under all conditions

  if(argv[4] == NULL) {          // run all algorithms if not specified
    run_linear_array = 1;
    run_binary_search = 1;
    run_linked_list_search = 1;
    run_tree_search = 1;

    printf("%10s ","array");     // print all algorithms
    printf("%10s ", "list");
    printf("%10s ", "binary");
    printf("%10s ", "tree");
    printf("\n");
  }
  
  else {
    char* algs_string = argv[4]; // set argv[4] to a string to find out which algorithm to use
    // seperate all for loops to follow the arbitrary order
    for(int i = 0; i < strlen(algs_string); i++) {    // fails if argv[4] is wrong
      if(algs_string[i] != 'a' && algs_string[i] != 'b' &&
         algs_string[i] != 'l' && algs_string[i] != 't') {
        fail();
        return 0;
      }
    }
    for(int i = 0; i < strlen(algs_string); i++) {    // print "array" in the title if argv[4] = 'a'
      if(algs_string[i] == 'a') {
        run_linear_array = 1;                         // set run_linear_array as 1
        printf("%10s ","array");
      }
    }
    for(int i = 0; i < strlen(algs_string); i++) {    // print "list" in the title if argv[4] = 'b'
      if(algs_string[i] == 'l') {
        run_binary_search = 1;                        // set run_binary_search as 1
        printf("%10s ","list");
      }
    }
    for(int i = 0; i < strlen(algs_string); i++) {    // print "binary" in the title if argv[4] = 'l'
      if(algs_string[i] == 'b') {
        run_linked_list_search = 1;                   // set run_linked_list_search as 1
        printf("%10s ","binary");
      }
    }
    for(int i = 0; i < strlen(algs_string); i++) {    // print "tree" in the title if argv[4] = 't'
      if(algs_string[i] == 't') {
        run_tree_search = 1;                          // set run_tree_search as 1
        printf("%10s ","tree");
      }
    }
    printf("\n");                                     // jump to the next line
  }
  
  int cur_length = 1 << minpow;                         // length = 2 ^ minpow
  int cur_search_size = cur_length * 2 * repeats;       // search = length * 2 * repeats
  for(int i = minpow; i <= maxpow; i++) { 
    printf(" %6d %8d ", cur_length, cur_search_size);   // print under LENGTH and SEARCH

    if(run_linear_array) {                              // run loops to time linear search in an array
      clock_t begin, end;                               // intialize begin, end
      int* linear_array = make_evens_array(cur_length); // setup array
      begin = clock();                                  // start timer
      for(int j = 0; j < repeats; j++) {                // do searches using for loop
        for(int k = 0; k < cur_length * 2; k++) {     
          linear_array_search(linear_array, cur_length, k);
        }
      }
      end = clock();                                    // stop timer               
      double array_time = ((double) (end - begin)) / CLOCKS_PER_SEC; // find out the time
      printf("%.4e ", array_time);                      // print output
      free(linear_array);                               // free the array
    }

    if(run_linked_list_search) {                        // run loops to time search in a linked list
      clock_t begin, end;                               // intialize begin, end
      list_t* linked_list_search = make_evens_list(cur_length);
      begin = clock();                                  // start timer
      for(int j = 0; j < repeats; j++) {                // do searches using for loop
        for(int k = 0; k < cur_length * 2; k++) {
          linkedlist_search(linked_list_search, cur_length, k);
        }
      }
      end = clock();
      double linked_list_search_time = ((double) (end - begin)) / CLOCKS_PER_SEC; // find out the time
      printf("%.4e ", linked_list_search_time);
      list_free(linked_list_search);                    // free the list
    }
    
    if(run_binary_search) {                             // run loops to time binary search in an array
      clock_t begin, end;                               // intialize begin, end
      int* binary_search = make_evens_array(cur_length);// setup array
      begin = clock();                                  // start timer
      for(int j = 0; j < repeats; j++) {                // do searches using for loop
        for(int k = 0; k < cur_length * 2; k++) {
          binary_array_search(binary_search, cur_length, k);
        }
      }
      end = clock();                                    // stop timer 
      double binary_search_time = ((double) (end - begin)) / CLOCKS_PER_SEC; // find out the time
      printf("%.4e ", binary_search_time);              // print output
      free(binary_search);                              // free the array
    }

    if(run_tree_search) {                               // run loops to time search in a tree
      clock_t begin, end;                               // intialize begin, end
      bst_t* tree_search = make_evens_tree(cur_length); // setup tree search
      begin = clock();                                  // start timer
      for(int j = 0; j < repeats; j++) {                // do searches using for loop
        for(int k = 0; k < cur_length * 2; k++) {
          binary_tree_search(tree_search, cur_length, k);
        }
      }
      end = clock();                                    // stop timer
      double tree_search_time = ((double) (end - begin)) / CLOCKS_PER_SEC; // find out the time
      printf("%.4e", tree_search_time);                 // print output
      bst_free(tree_search);                            // free the tree
    }
    printf("\n");                                       // jump to the next line
    cur_length = cur_length * 2;                        // length = length * 2
    cur_search_size = cur_search_size * 2;              // search = search * 2
  }
  return 0;   // return 0
}
