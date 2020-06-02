#include "rbtree.h"
#include<stdio.h>

void testOne();
void printTree(struct RBTreeNode*);

int main(int argc, char** argv) {

  /* Note: Tests, at the moment, contain memory leaks. */

  printf("Beginning Red-Black Tree Tests!\n");
  testOne();

  return 0;
}

void testOne() {

  struct RBTreeNode *root = init_rbtree(0, NULL);

  insert(10, NULL, &root);
  insert(4, NULL, &root);
  insert(20, NULL, &root);
  insert(-4, NULL, &root);
  printTree(root);

  struct RBTreeNode *result = search(10, root);
  printf("\n\nsearch(10, root) = [%p]: %d \n", result, result->key);

  result = search(20, root);
  printf("search(20, root) = [%p]: %d \n", result, result->key);


}

void printTree(struct RBTreeNode* root) {

  /* Inorder traversal of the tree. */

  if (root != NULL) {
    printTree(root->left);
    printf("[%p] Key = %d\n", root, root->key);
    printTree(root->right);

  }
}
