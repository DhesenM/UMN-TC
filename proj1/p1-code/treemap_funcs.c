// treemap_funcs.c: Provides a small library of functions that operate on
// binary search trees mapping strings keys to string values.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "treemap.h"

/**********************************************************************
/ Initialize the given tree to have a null root and have size 0.
/**********************************************************************/
void treemap_init(treemap_t *tree){
	tree->root = NULL;
	tree->size = 0;
}

/**********************************************************************
/Inserts given key/value into a binary search tree.
/**********************************************************************/
int treemap_add(treemap_t *tree, char key[], char val[]){
	// initialize node_t start and beforestart to track the node
	node_t *start = tree->root;       
	node_t *beforestart = NULL;          

	// find if there is char key[] in the tree
	while(start != NULL){
		beforestart = start;               // track node_t start
		if(strcmp(start->key,key)==0){     // copy the value and return 0 if the keys are identical
			strcpy(start->val,val);
			return 0;
		}
		else if(strcmp(start->key,key)>0){ // go to the left branch if key is greater 
			start = start->left;
		}
		else{                              // go to the left branch if key is smaller 
			start = start->right;
		}
	}

	// initialize and allocate memory for insert
	// copy and provided key and val and let left and right branches be NULL
	node_t *insert = malloc(sizeof(node_t)*128);
	strcpy(insert -> key,key);
	strcpy(insert -> val,val);
	insert -> left = NULL;
	insert -> right = NULL;

	if(beforestart == NULL){                 // replace the root if root is null
		tree -> root = insert;
		tree -> size++;
		return 1;
	}
	else if(strcmp(beforestart->key,key)<0){ // insert on the right if beforestart's key is smaller
		beforestart -> right = insert;
	}
	else{
		beforestart -> left = insert;        // insert on the left if beforestart's key is greater
	}
	// add 1 to tree's size and return 1
	tree -> size++;
	return 1;
}

/**********************************************************************
/ Searches the tree for given 'key' and returns its associated value
/*********************************************************************/
char *treemap_get(treemap_t *tree, char key[]){
	// initialize start as treemap's root
	node_t *start = tree->root;
	// use while loop to search for the given key
	while(start != NULL){                 
		if(strcmp(start->key,key)==0){      // return the value if the key is found
			return start->val;
		}
		else if(strcmp(start->key,key)>0){ // go to the left branch if key is greater  
			start = start->left;
		}
		else{
			start = start->right;          // go to the right branch if key is smaller 
		}
	}
	return NULL;    // return NULL if no matching key is found
}

/**********************************************************************
/ Eliminate all nodes in the tree setting its contents empty.
/**********************************************************************/
void treemap_clear(treemap_t *tree){
	node_remove_all(tree->root);  // use node_remove_all(tree->root) to free nodes' memories
	tree -> size = 0;             // set tree size to 0
	tree -> root = NULL;          // set tree root to NULL
}

/**********************************************************************
/ Recursive helper function which visits all nodes in a tree and
/ frees the memory associated with them. This requires a post-order
/**********************************************************************/
void node_remove_all(node_t *cur){
	if (cur == NULL){             // return the function after all curs are NULL
		return;
	}
	// use recursive methods to free all the nodes in a tree
	node_remove_all(cur -> left); // recursing to the left
    node_remove_all(cur -> right);// recursing to the right
    free(cur);
}

/**********************************************************************
/ Prints the key/val pairs of the tree in reverse order at differing
/ levels of indentation which shows all elements and their structure
/ in the tree.
/**********************************************************************/
void treemap_print_revorder(treemap_t *tree){
	node_print_revorder(tree->root,0);
}

/**********************************************************************
/ Recursive helper function which prints all key/val pairs in the
/ tree rooted at node 'cur' in reverse order.
/**********************************************************************/
void node_print_revorder(node_t *cur, int indent){
	indent++;
	if (cur == NULL){     // return the function after all curs are printed (NULL)
		return;
	}
	node_print_revorder(cur -> right,indent);  // traverse right tree first 
	for(int i=0;i<indent-1;i++){               // print the spaces before cur
		printf("  ");
	}
	printf("%s -> %s\n",cur->key,cur->val);    // prints cur node's key/val
	node_print_revorder(cur -> left,indent);   // traverse left tree afterwards
}

/**********************************************************************
/ Print all the data in the tree in pre-order with indentation
/ corresponding to the depth of the tree.
/**********************************************************************/
void treemap_print_preorder(treemap_t *tree){
	node_write_preorder(tree->root,stdout,0);   // print all the data in the tree
}

/**********************************************************************
/ Saves the tree by opening the named file
/**********************************************************************/
void treemap_save(treemap_t *tree, char *fname){
	// writing the tree to FILE named fname in pre-order with node_write_preorder()
	FILE *file;
	file = fopen(fname, "w");
	node_write_preorder(tree->root,file,0);
	fclose(file);     // close the file
}

/**********************************************************************
/ Recursive helper function which writes/prints the tree in pre-order
/ to the given open file handle.
/**********************************************************************/
void node_write_preorder(node_t *cur, FILE *out, int depth){
	depth++;                       // depth increase by 1 on each recursive call
	node_t *track = cur;     
	if(track == NULL){
		return;
	}
	for(int i=0;i<depth-1;i++){    // print 2 spaces per depth to the file
		fprintf(out,"  ");
	}
	fprintf(out,"%s %s\n",track->key,track->val);  // writes/prints the tree in pre-order
	node_write_preorder(track->left,out,depth);    // recursing to the left
	node_write_preorder(track->right,out,depth);   // recursing to the right
}

/**********************************************************************
/ Clears the given tree then loads new elements to it from the named.
/**********************************************************************/
int treemap_load(treemap_t *tree, char *fname ){
	FILE *file;              // initialize and open FILE file
	file = fopen(fname,"r");
	if(file == NULL){        // return 0 if opening file fails
		return 0;
	}
	treemap_clear(tree);     // clear the given tree

	// initialize char elements of tree and int insert to determine if the file is EOF
	char insertkey[128];  
	char insertval[128];
	int insert = 0;
	while(insert != EOF){    // load the elements to the given tree until EOF
		insert = fscanf(file,"%s %s",insertkey,insertval);
		treemap_add(tree,insertkey,insertval);
	}
	fclose(file);            // close the file 
	return 1;                // return 1 after successfully loaded
}
