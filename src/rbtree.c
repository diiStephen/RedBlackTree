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

struct RBTreeNode* init_rbtree(int k, void* d) {

  /* Construct a new Red-Black tree by returning a root node. */

  struct RBTreeNode *root = init_rbtree_node(NULL, NULL, NULL, k, d, BLACK);

  return root;

}
