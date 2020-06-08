#ifndef RBTREE_H
#define RBTREE_H

/* Simple Red-Black tree implementation. */


/****** CONSTANTS AND TYPE DEFINITIONS ******/

#define true  1
#define false 0
typedef int bool;

// Colorings for nodes.
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
void dest_rbtree_node(struct RBTreeNode **);

/* Specialized free function to help avoid dangling pointers. */
void free_rbtree_node(struct RBTreeNode **);

/* Constructor for Red-Black tree itself. */
struct RBTreeNode* init_rbtree(int, void *);

/* Public destructor wrapper for for RB-Tree. */
void dest_rbtree(struct RBTreeNode **);

/* Private destructor for RBT. */
void dest_rbtree_(struct RBTreeNode **);

/****** UPDATE FUNCTIONS ******/

/* Insertion function. */
struct RBTreeNode* insert(struct RBTreeNode **, int, void *);

/* Correct RBT properties after inserts. */
void insert_fixup(struct RBTreeNode **, struct RBTreeNode *);

/* Search and delete function . */
void* search_and_delete(struct RBTreeNode **, int);

/* Delete function. */
void* delete_node(struct RBTreeNode **, struct RBTreeNode *);

/* Correct RBT properties after deletion. */
void delete_fixup(struct RBTreeNode **, struct RBTreeNode *);

/* Transplant utility function. Used for inserts/deletes */
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

/* Compute the height of the RBT -- O(n) operation. */
int height(struct RBTreeNode *);

/****** UTILITY FUNCTIONS ******/

/* Validate nodes accepted as parameters. */
void validate(struct RBTreeNode *, bool);

/* Left-Rotate operation. */
void left_rotate(struct RBTreeNode **, struct RBTreeNode *);

/* Right-Rotate operation. */
void right_rotate(struct RBTreeNode **, struct RBTreeNode *);

#endif
