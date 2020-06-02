#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>

struct RBTreeNode* init_rbtree_node(struct RBTreeNode *p, struct RBTreeNode *l, struct RBTreeNode *r, int k, void *d, color_t c) {

  struct RBTreeNode *node = NULL;

  if ((node = malloc(sizeof(struct RBTreeNode))) == NULL ) {
    fprintf(stderr, "Could not allocate memory for new RBTreeNode.");
    exit(1);
  }

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
    fprintf(stderr, "Data has not been deallocated.\n");
    exit(1);
  }
}

struct RBTreeNode* init_rbtree(int k, void *d) {

  /* Construct a new Red-Black tree by returning a root node. */

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
  if (root == NULL || root->right == NULL)
    return root;
  else
    maximum(root->right);
}
