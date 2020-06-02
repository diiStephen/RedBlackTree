#ifndef RBTREE_H
#define RBTREE_H

/* Simple Red-Black tree implementation. */

/* TODO:
		1. Finish core functionality, i.e. insert/delete, search, etc..

		2. Add functionality for iterator design pattern. This will make the
			 traversal algorithms easier. Return an iterator rather than take
			 function pointers.

		3. Make sure the data structure is multi-thread safe.

		4. Add doxygen documention.

		5. Compile and link as a shared object library, besides the test programs.

		6. implementation should be modularized with the implementation hidden
		   behind a public interface.
*/

enum color {
	    RED,
	    BLACK
};

typedef enum color color_t;

struct RBTreeNode {

  struct RBTreeNode *parent; /* Pointer to parent node.      */
  struct RBTreeNode *left;   /* Pointer to left child node.  */
  struct RBTreeNode *right;  /* Pointer to right child node. */

  int key;                   /* int key used for ordering data. */
  void *data;                /* void pointer to satelite data.  */
  color_t c;                 /* Current color (red/black) of the node. */

};

/****** CONSTRUCTORS AND DESTRUCTORS ******/

/* Constructor for Red-Black tree node. */
struct RBTreeNode* init_rbtree_node(struct RBTreeNode *, struct RBTreeNode *, struct RBTreeNode *, int, void *, color_t);

/* Destructor for Red-Black tree node. */
void dest_rbtree_node(struct RBTreeNode *);

/* Constructor for Red-Black tree itself. */
/* Returns a pointer to a new RBTree node with NULL parent. */
struct RBTreeNode* init_rbtree(int, void *);

/****** UPDATE FUNCTIONS ******/

/* Insertion function. */
struct RBTreeNode* insert(int, void *, struct RBTreeNode **);

/****** ACCESSOR FUNCTIONS ******/

/* Search the tree for a given key. */
struct RBTreeNode* search(int, struct RBTreeNode *);

/* Search tree for node with minimum key. */
struct RBTreeNode* minimum(struct RBTreeNode *);

/* Search tree for node with maximum key. */
struct RBTreeNode* maximum(struct RBTreeNode *);

/* Find predecessor node. */
struct RBTreeNode* predecessor(int, struct RBTreeNode *);

/* Find successor node. */ 
struct RBTreeNode* successor(int, struct RBTreeNode *);


#endif
