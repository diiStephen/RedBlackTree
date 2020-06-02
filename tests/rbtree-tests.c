#include "rbtree.h"
#include<stdio.h>

int main(int argc, char** argv) {

  printf("Beginning Red-Black Tree Tests!\n");

  /* Create nodes. */
  struct RBTreeNode *root  = init_rbtree(0, NULL);
  struct RBTreeNode *child = init_rbtree_node(root, NULL, NULL, 1, NULL, BLACK);

  root->left = child;

  printf("[%p]: RBTreeNode root\n", root);
  printf("\t[%p]: RBTreeNode root->left\n", root->left);
  printf("\t\t[%p]: RBTreeNode root->left->parent\n", (root->left)->parent);
  printf("\t[%p]: RBTreeNode root->right\n", root->right);
  printf("[%p]: RBTreeNode child\n", child);

  printf("Destroying nodes!\n");
  dest_rbtree_node(child);
  dest_rbtree_node(root);
  printf("Done!\n");

  return 0;
}
