#include "errors.h"
#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>

struct RBTreeNode* init_rbtree_node(struct RBTreeNode *p, struct RBTreeNode *l,
  struct RBTreeNode *r, int k, void *d, color_t c) {

  struct RBTreeNode *node = NULL;

  if ((node = malloc(sizeof(struct RBTreeNode))) == NULL )
    display_error(MEM_ERROR);


  node->parent = p;
  node->left   = l;
  node->right  = r;
  node->key    = k;
  node->data   = d;
  node->c      = c;

  return node;

}

void dest_rbtree_node(struct RBTreeNode *node) {

  /* Weary of memory leaks here. */

  if (node->data == 0) {

    /* Node must have it's data destroyed before destroying the node. */
    /* Convention is to set the node->data ptr to 0 */

    free(node);

  } else {
    display_error(INV_DELOC);
  }
}

struct RBTreeNode* init_rbtree(int k, void *d) {
  struct RBTreeNode *root = init_rbtree_node(NULL, NULL, NULL, k, d, BLACK);
  return root;
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

  struct RBTreeNode *newest = init_rbtree_node(NULL, NULL, NULL, k, data, BLACK);
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
  validate(node);
  
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
  validate(node);

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

@param node Pointer to RBTreeNode to be validated
**/
void validate(struct RBTreeNode *node) {
  if (node == NULL || node->parent == node)
    display_error(INV_NODE);
}
