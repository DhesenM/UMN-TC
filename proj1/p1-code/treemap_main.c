// main function for a treemap

#include <stdio.h>
#include <string.h>
#include "treemap.h"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
	  echo=1;
  }

  // print out the initial option list.
  printf("TreeMap Editor\n");
  printf("Commands:\n");
  printf("  quit:            exit the program\n");
  printf("  print:           shows contents of the tree in reverse sorted order\n");
  printf("  add <key> <val>: inserts the given key/val into the tree, duplicate keys are ignored\n");
  printf("  get <key>:       prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
  printf("  clear:           eliminates all key/vals from the tree\n");
  printf("  preorder:        prints contents of the tree in pre-order which is how it will be saved\n");
  printf("  save <file>:     writes the contents of the tree in pre-order to the given file\n");
  printf("  load <file>:     clears the current tree and loads the one in the given file\n");
  
  // initialize 3 char[] to scan the commands, keys, and vals.
  char val[128];
  char key[128];
  char cmd[128];
  int success;          // initialize success to determine if command succeeded (1 yes 0 no)
  treemap_t tree;       // initialize tree 
  treemap_init(&tree);

  while(1){
	  printf("TM> ");                     // print prompt
	  success = fscanf(stdin,"%s",cmd);   // read a command
	  if(success==EOF){                   // check for end of input
		  printf("\n");                   // found end of input
		  break;                          // break from loop
	  }

	  if( strcmp("quit", cmd)==0 ){       // check for quit command
		  if(echo){
			  printf("quit\n");
		  }
		  break;                          // break from loop
	  }

	  else if( strcmp("print", cmd)==0 ){ // print command
		  if(echo){
			  printf("print\n");
		  }
		  treemap_print_revorder(&tree);
	  }

      else if( strcmp("add", cmd)==0 ){   // add command
		  fscanf(stdin,"%s %s",key,val);  // read key and val to add
		  if(echo){
			  printf("add %s %s\n",key,val);
		  }
		  success = treemap_add(&tree,key,val);
		  if(!success){                   // print modified existing if not succeeded
			  printf("modified existing key\n");
		  }
	  }

	  else if( strcmp("get", cmd)==0 ){   // get command
		  char *found;
		  fscanf(stdin,"%s",key);         // read key of the tree
          if(echo){ 
			  printf("get %s\n",key);
		  }
		  found = treemap_get(&tree,key); // initialize found as val of the tree
		  if(found){                      
			  printf("FOUND: %s\n",found);
		  }
		  else{
			  printf("NOT FOUND\n");
		  }
	  }
	  
	  else if( strcmp("clear", cmd)==0 ){   // clear command
		  if(echo){
			  printf("clear\n");
		  }
		  treemap_clear(&tree);
	  }

	  else if( strcmp("preorder", cmd)==0 ){// preorder command
		  if(echo){
			  printf("preorder\n");
		  }
		  treemap_print_preorder(&tree);
	  }

	  else if( strcmp("save", cmd)==0 ){    // save command
		  fscanf(stdin,"%s",cmd);
		  if(echo){
			  printf("save %s\n",cmd);
		  }
		  treemap_save(&tree,cmd);
	  }
	  
	  else if( strcmp("load", cmd)==0 ){    // load command
		  fscanf(stdin,"%s",cmd); 
		  if(echo){
			  printf("load %s\n",cmd);
		  }
		  treemap_load(&tree,cmd);
		  success = treemap_load(&tree,cmd);
		  if(!success){                     // print error messages if not succeeded
			  printf("ERROR: could not open file '%s'\nload failed\n",cmd);
		  }
	  }

	  else{                                 // unknown command
		  if(echo){
			  printf("%s\n",cmd);
		  }
		  printf("unknown command %s\n",cmd);
	  }
  }  

  // end main while loop
  treemap_clear(&tree);                     // clean up the tree
  return 0;
}
