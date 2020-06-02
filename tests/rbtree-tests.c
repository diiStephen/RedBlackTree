#include "rbtree.h"
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>

void testOne();
void printTree(struct RBTreeNode*);
void nullAction(int);

int main(int argc, char** argv) {

  /* Note: Tests, at the moment, contain memory leaks. */

  printf("Beginning Red-Black Tree Tests!\n");
  testOne();

  return 0;
}

void testOne() {

  static struct sigaction act;
  act.sa_handler = nullAction;
  sigfillset(&(act.sa_mask));

  sigaction(SIGSEGV, &act, NULL);

  struct RBTreeNode *root   = init_rbtree(0, NULL);
  struct RBTreeNode *result = NULL;
  struct RBTreeNode *succ   = NULL;
  struct RBTreeNode *pred   = NULL;

  insert(10, NULL, &root);
  insert(4, NULL, &root);
  insert(20, NULL, &root);
  insert(-4, NULL, &root);
  insert(100, NULL, &root);
  insert(101, NULL, &root);
  insert(13, NULL, &root);
  printTree(root);

  result = search(10, root);
  printf("\n\nsearch(10, root) = [%p]: %d \n", result, result->key);

  result = search(20, root);
  printf("search(20, root) = [%p]: %d \n", result, result->key);

  result = search(-4, root);
  printf("search(14, root) = [%p]: %d \n", result, result->key);

  result = minimum(root);
  printf("Minimum key is: %d @ %p\n", result->key, result);

  result = maximum(root);
  printf("Maximum key is: %d @ %p\n", result->key, result);

  result = search(20, root);
  succ = successor(result);
  printf("Result: [%p]:%d\nSuccessor: [%p]:%d\n", result, result->key, succ, succ->key);

  result = search(-4, root);
  succ = successor(result);
  printf("Result: [%p]:%d\nSuccessor: [%p]:%d\n", result, result->key, succ, succ->key);

  result = search(13, root);
  pred = predecessor(result);
  printf("Result: [%p]:%d\nPredecessor: [%p]:%d\n", result, result->key, pred, pred->key);

}

void printTree(struct RBTreeNode* root) {

  /* Inorder traversal of the tree. */

  if (root != NULL) {
    printTree(root->left);
    printf("[%p] Key = %d\n", root, root->key);
    printTree(root->right);

  }
}

void nullAction(int signo) {
  printf("CAUGHT: %d\nMust be a SEGFAULT\n", signo);
  exit(1);
}
