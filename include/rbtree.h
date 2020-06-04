#ifndef RBTREE_H
#define RBTREE_H

/* Simple Red-Black tree implementation. */

/*
TODO:
		1. Need to add support for sentinel node. The RBTree design
			 only stores data at internal nodes. All previously NULL pointers will
			 have to be replaced to a reference to a special sentinel node. How is
			 the sentinel node marked? Informating in the node struct?


		2. Add functionality for iterator design pattern. This will make the
			 traversal algorithms easier. Return an iterator rather than take
			 function pointers.

		3. Make sure the data structure is multi-thread safe.

		4. Add doxygen documention.

		5. Compile and link as a shared object library, besides the test programs.

		6. Implementation should be modularized with the implementation hidden
		   behind a public interface.
*/

#define true 1
#define false 0
typedef int bool;

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
	bool isSen;								 /* Is this node the sentinel? */

};

/****** CONSTRUCTORS AND DESTRUCTORS ******/

/* Constructor for Red-Black tree node. */
struct RBTreeNode* init_rbtree_node(struct RBTreeNode *, struct RBTreeNode *,
	struct RBTreeNode *, int, void *, color_t, bool);

/* Destructor for Red-Black tree node. */
void dest_rbtree_node(struct RBTreeNode *);

/* Specialized free function to avoid dangling pointers. */
void free_rbtree_node(struct RBTreeNode **);

/* Constructor for Red-Black tree itself. */
struct RBTreeNode* init_rbtree(int, void *);

/* Destructor for for RB-Tree. */
void dest_rbtree(struct RBTreeNode *);

/****** UPDATE FUNCTIONS ******/

/* Insertion function. */
struct RBTreeNode* insert(int, void *, struct RBTreeNode **);

/* Delete function - public. */
void* delete(int, struct RBTreeNode *);

/* Delete function - private. */
void* delete_node(struct RBTreeNode **, struct RBTreeNode *);

/* Transplant utility function - private. */
void transplant(struct RBTreeNode **, struct RBTreeNode *, struct RBTreeNode *);

/****** ACCESSOR FUNCTIONS ******/

/* Search the tree for a given key. */
struct RBTreeNode* search(int, struct RBTreeNode *);

/* Search tree for node with minimum key. */
struct RBTreeNode* minimum(struct RBTreeNode *);

/* Search tree for node with maximum key. */
struct RBTreeNode* maximum(struct RBTreeNode *);

/* Find predecessor node. */
struct RBTreeNode* predecessor(struct RBTreeNode *);

/* Find successor node. */
struct RBTreeNode* successor(struct RBTreeNode *);

/****** UTILITY FUNCTIONS ******/

/* Validate nodes accepted as parameters. */
void validate(struct RBTreeNode *, bool);

/* Left-Rotate operation. */
void left_rotate(struct RBTreeNode **, struct RBTreeNode *);

/* Right-Rotate operation. */
void right_rotate(struct RBTreeNode **, struct RBTreeNode *);

#endif
