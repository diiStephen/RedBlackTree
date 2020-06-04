#include "rbtree.h"
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void testOne();
void testTwo();
void printTree(struct RBTreeNode*);
void nullAction(int);
void print_node(struct RBTreeNode*);

int main(int argc, char** argv) {

  /* Note: Tests, at the moment, contain memory leaks. */

  printf("Beginning Red-Black Tree Tests!\n");
  //testOne();
  testTwo();

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
  printf("\n\nsearch(10, root) = [%p]: %d \n", (void*)result, result->key);

  result = search(20, root);
  printf("search(20, root) = [%p]: %d \n", (void*)result, result->key);

  result = search(-4, root);
  printf("search(14, root) = [%p]: %d \n", (void*)result, result->key);

  result = minimum(root);
  printf("Minimum key is: %d @ %p\n", result->key, (void*)result);

  result = maximum(root);
  printf("Maximum key is: %d @ %p\n", result->key, (void*)result);

  result = search(20, root);
  succ = successor(result);
  printf("Result: [%p]:%d\nSuccessor: [%p]:%d\n", (void*)result, result->key,
    (void*)succ, succ->key);

  result = search(-4, root);
  succ = successor(result);
  printf("Result: [%p]:%d\nSuccessor: [%p]:%d\n", (void*)result, result->key,
    (void*)succ, succ->key);

  result = search(13, root);
  pred = predecessor(result);
  printf("Result: [%p]:%d\nPredecessor: [%p]:%d\n", (void*)result, result->key,
    (void*)pred, pred->key);

  printf("Testing delete functions!\n");

  printf("Deleting: 100\n");
  delete(100, root);

  printf("Deleting -4\n");
  delete(-4, root);

  printTree(root);

}

void testTwo() {

  //Currently broken as certain functions have not been updated to deal
  // with the sentinel.

  printf("Starting test 2!\n\n");

  struct RBTreeNode *root   = NULL;
  struct RBTreeNode *result = NULL;
  int toInsert = 0;

  printf("Building random tree!\n");
  
  toInsert = rand() % 100;
  root = init_rbtree(toInsert, NULL);
  for(int i = 0; i < 10; i++) {
    toInsert = rand() % 100;
    insert(toInsert, NULL, &root);
  }

  printTree(root);

}

void printTree(struct RBTreeNode* root) {

  /* Inorder traversal of the tree. */

  if (root != NULL) {
    printTree(root->left);
    printf("[%p] Key = %d\n", (void*)root, root->key);
    printTree(root->right);

  }
}

void nullAction(int signo) {
  printf("CAUGHT: %d\nMust be a SEGFAULT\n", signo);
  exit(1);
}

void print_node(struct RBTreeNode* node) {
  if (node != NULL) {
    printf("Node@[%p]: Key = %d\n", (void*)node, node->key);
  } else {
    printf("Node@[Null]\n");
  }
}
