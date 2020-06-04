#include "errors.h"
#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>

struct RBTreeNode* init_rbtree_node(struct RBTreeNode *p, struct RBTreeNode *l,
  struct RBTreeNode *r, int k, void *d, color_t c, bool s) {

  struct RBTreeNode *node = NULL;

  if ((node = malloc(sizeof(struct RBTreeNode))) == NULL )
    display_error(MEM_ERROR);


  node->parent = p;
  node->left   = l;
  node->right  = r;
  node->key    = k;
  node->data   = d;
  node->c      = c;
  node->isSen  = s;

  return node;

}

/**
Function for destroying RBTreeNodes. Accepts a pointer
to a node to be destroyed. The node's satelite data pointer
must be handled by the caller. The convention here is that the
data field of the node struct must be assigned to 0 to indicate
that the data has been handled.

NOTE: There may be memory leaks if the satelite data is not handled properly.

@param node Pointer to node to be destroyed.
**/
void dest_rbtree_node(struct RBTreeNode *node) {
  if (node->data == 0) {
    free_rbtree_node(&node);
  } else {
    display_error(INV_DELOC);
  }
}

/**
Specialized free function to avoid dangling pointers with
RBTreeNodes after they have been removed. Once the node has been
freed the reference is nullified to ensure we don't have a dangling
pointer.

NOTE: There may be other references to the same block of data, though. We must
be careful to avoid any such situation. This may be particuarly relevant to
the sentinel node where it will be the child of many nodes in the RBT.

@param node Double pointer to RBTreeNode to be freed.
**/
void free_rbtree_node(struct RBTreeNode **node) {
  if(node != NULL) {
    free(*node);
    *node = NULL;
  }
}

/**
Function to construct a new RBT with intial key k and initial satelite
data pointer d. Function creates a root node and the sentinel node. The
sentinel node can always be accessed by using root->parent. This sentinel
will be useful in simplifying the implementation of the RBT operations.

NOTE: All operations will need to take into account the sentinel node.

@param k Initial key of the root data.
@param d Inital satelite data pointer, becomes root node's associated data.
@return Pointer to root node of the rbtree.
**/
struct RBTreeNode* init_rbtree(int k, void *d) {
  struct RBTreeNode *sentinel = init_rbtree_node(NULL, NULL, NULL, 0, NULL, BLACK, true); //Allocate sentinel node.
  struct RBTreeNode *root = init_rbtree_node(sentinel, NULL, NULL, k, d, BLACK, false);   //Allocate root node with root->p = sentinel.
  return root;
}

/**
Function to free memory used by entire RBT. The function
post-order walks the tree, freeing each node.

CAUTION: This function assumes that all data associated to
each node has already been destroyed.

@param root Root of RBT to be de-allocated.
**/
void dest_rbtree(struct RBTreeNode *root) {

  if (root->left != NULL)
    dest_rbtree(root->left);
  if (root->right != NULL)
    dest_rbtree(root->right);

  root->data = 0;
  dest_rbtree_node(root);

}

/**
Insert data into the red-black tree. Ordereding is
given by the integer key associated with the satelite
data.

@param k Key associated with data. Used for searching, ordering, etc.
@param data Data associated with the node. void* for generic data.
@return Pointer to the new node inserted into the tree.
**/
struct RBTreeNode* insert(int k, void *data, struct RBTreeNode **root) {

  struct RBTreeNode *newest = init_rbtree_node(NULL, NULL, NULL, k, data, BLACK, false);
  struct RBTreeNode *walk = *root;
  struct RBTreeNode *parent = NULL;

  // Find the appropriate spot for the node.
  while (walk != NULL) {
    parent = walk;
    if (k < walk->key) {
      walk = walk->left;
    } else {
      walk = walk->right;
    }
  }
  newest->parent = parent;

  // Update pointers.
  if (parent == NULL) {
    *root = newest; //Tree was empty.
  } else if (k < parent->key) {
    parent->left = newest;
  } else {
    parent->right = newest;
  }

  return newest;
}

/**
Public delete function. Removes a node with the given key
in RB-tree rooted at *root. First the function finds a node
with the given key, then calls the delete_node function on
that node.

@param key Key of node to be removed from the tree.
@param root Pointer to the root pointer of the rbtree.
@return Pointer to data removed from the tree, or null if node with
given key does not exist.
**/
void* delete(int key, struct RBTreeNode *root) {
  struct RBTreeNode *result = search(key, root);
  if (result == NULL)
    return result;
  else
    return delete_node(&root, result);
}

/**
Private utility function for deleting nodes. Removes the node
pointed to by node from the rbtree.

@param root Pointer to the root pointer of the RBT where the deletion
is to take place.
@param node Pointer to RBTreeNode pointer to be removed from the tree.
@return pointer to satelite data associated to the node being removed
from the tree.
**/
void* delete_node(struct RBTreeNode **root, struct RBTreeNode *node) {

  struct RBTreeNode *deleted = node; //Save pointer to the node.
  struct RBTreeNode *succ    = NULL; //ptr to sucessor of node.
  void *response             = NULL; //ptr to hold the response.

  if (node->left == NULL) {
    transplant(root, node, node->right); // Case I
  } else if (node->right == NULL) {
    transplant(root, node, node->left); // Case II
  } else { // Cases III, IV
    succ = minimum(node->right);
    if (succ->parent != node) {
      transplant(root, succ, succ->right);
      succ->right = node->right;
      succ->right->parent = succ;
    }
    transplant(root, node, succ);
    succ->left = node->left;
    succ->left->parent = succ;
  }

  deleted->parent = deleted; //Convention for deleted node.
  deleted->right = NULL;
  deleted->left = NULL;
  response = deleted->data;
  deleted->data = 0; // Convention for deleted node.

  dest_rbtree_node(deleted); // Delete the memory for the node.

  return response;

}

/**
Private utility function used in the delete procedure. Reaplaces
the subtree rooted at node dest with subtree rooted at node src.

NOTE: I suspect there is a memory leak here, i.e. dest.

@param root Root of RB-tree where transplant is taking place.
@param dest Destination of replacement.
@param src Source of replacement.
**/
void transplant(struct RBTreeNode **root, struct RBTreeNode *dest,
  struct RBTreeNode *src) {

    if (dest->parent == NULL) {
      *root = src; // dest is the root.
    } else if (dest == dest->parent->left) {
      dest->parent->left = src; // dest is the left child of its parent.
    } else {
      dest->parent->right = src; // dest is the right child of its parent.
    }

    if (src != NULL) {
      src->parent = dest->parent; // Src parent pointer must be updated.
    }

  }

/**
Perform a recursive search of the RB-tree rooted at root.
Uses the BST property for fast searches. Let h = height(T),
then the running time of search is O(h).

@param key Key associated to the node being searched.
@param root Root of the RB-tree being searched.
@return Pointer to node with given key, or null if search fails.
**/
struct RBTreeNode* search(int key, struct RBTreeNode *root) {
  validate(root, false);

  struct RBTreeNode *walk = root;

  if (walk == NULL) {
    return NULL;
  } else if (key < walk->key) {
    search(key, walk->left);
  } else if (key > walk->key) {
    search(key, walk->right);
  } else {
    return walk;
  }
}

/**
Return a pointer to the node of the RB-tree with minimum key value. The search
is done by recursively following the left pointers of each node from the root.

@param root Root of the tree to get minimum key.
@return Pointer to node of the tree with minimum key, or null if the tree is
empty.
**/
struct RBTreeNode* minimum(struct RBTreeNode *root) {
  validate(root, false);
  if (root == NULL || root->left == NULL) // Short circuting.
    return root;
  else
    minimum(root->left);
}

/**
Return a pointer to the node of the RB-tree with maximum key value. The search
is done by recursively following the right pointers of each node from the root.

@param root Root of the tree to get minimum key.
@return Pointer to node of the tree with maximum key, or null if the tree is
empty.
**/
struct RBTreeNode* maximum(struct RBTreeNode *root) {
  validate(root, false);
  if (root == NULL || root->right == NULL) // Short circuting.
    return root;
  else
    maximum(root->right);
}

/**
Return a pointer to the node of the RB-tree with key that is the
predecessor of the key given node.

@param node Pointer to node of RB-tree to find predecessor of.
@return pointer to predecessor node, or null.
**/
struct RBTreeNode* predecessor(struct RBTreeNode *node) {
  validate(node, true);

  if (node->left != NULL) {
    return maximum(node->left);
  }

  struct RBTreeNode *trace = node->parent; //Ascending the tree.
  while(trace != NULL && node == trace->left) {
    node = trace;
    trace = trace->parent;
  }
  return trace;

}

/**
Return a pointer to the node of the RB-tree with key that is the
successor of key of the given node.

@param node Pointer to node of RB-tree to find successor of.
@return pointer to successor node, or null.
**/
struct RBTreeNode* successor(struct RBTreeNode *node) {
  validate(node, true);

  if (node->right != NULL) {
    return minimum(node->right);
  }

  struct RBTreeNode *trace = node->parent; // Ascending the tree.
  while(trace != NULL && node == trace->right) {
    node = trace;
    trace = trace->parent;
  }
  return trace;

}

/**
Validate a the RBTreeNode pointed to by node.

Convention: A node removed from the tree will have its parent pointer
assigned to itself. This is used to indicate a defunct node of the tree.

Some functions allow a null pointer as node, others do not, so
the chkNull parameter here specifies if a null node is considered
invalid.

@param node Pointer to RBTreeNode to be validated
@param chkNull Consider null pointers invalid?
**/
void validate(struct RBTreeNode *node, bool chkNull) {

  switch (chkNull) {

    case true:
      if (node == NULL || node->parent == node)
        display_error(INV_NODE);
      break;

    case false:
      if (node != NULL && node->parent == node)
        display_error(INV_NODE);
      break;
  }
}

/**
Implementation of the Left-Rotate(T, x) algorithm. Used for re-balancing
the RBT after update operations. This operation is a structural update, i.e.
by re-arranging pointers. The running time is O(1). Furthermore, a left-rotate
preserves the BST properties.

left_rotate assumes that node->right != s, the senitnel.

@param root Root of the RBT containing node.
@node Node of the tree to Left-Rotate.
**/
void left_rotate(struct RBTreeNode **root, struct RBTreeNode *node) {

  struct RBTreeNode *r = node->right; // r replaces node at node's position.

  node->right = r->left; // Left subtree of r becomes node's right subtree.

  if (r->left->isSen == false) // Update parent pointer of r's left child.
    r->left->parent = node;

  r->parent = node->parent; // r's parent becomes x's former parent.

  if (node->parent->isSen == true) {        //Node was root.
    *root = r;
  } else if (node == node->parent->left) {  // Node is a left child.
    node->parent->left = r;
  } else {                                  // Node must be the right child.
    node->parent->right = r;
  }

  r->left = node; // Place node in it's proper place.
  node->parent = r;

}

/**
Implementation of the Right-Rotate(T, x) algorithm. Used for re-balancing
the RBT after update operations. This operation is a structural update, i.e.
by re-arranging pointers. The running time is O(1). Furthermore, a right-rotate
preserves the BST properties.

right_rotate assumes that node->left != s, the senitnel.

@param root Root of the RBT containing node.
@node Node of the tree to Right-Rotate.
**/
void right_rotate(struct RBTreeNode **root, struct RBTreeNode *node) {

  struct RBTreeNode *r = node->left; // r places node in at node's position.


}
