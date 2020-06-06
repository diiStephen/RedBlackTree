#include "rbtree.h"
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/**
Temporary test module. This will be replaced with something
more robust once the core functionality of the RBT has been
completed. 
**/

void testOne();
void testTwo();
void testThree();
void testFour();
void printTree(struct RBTreeNode*);
void nullAction(int);
void print_node(struct RBTreeNode*);

int main(int argc, char** argv) {

  /* Note: Tests, at the moment, contain memory leaks. */

  printf("Beginning Red-Black Tree Tests!\n");
  //testOne();
  testTwo();
  testThree();
  testFour();

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
  search_and_delete(root, 100);

  printf("Deleting -4\n");
  search_and_delete(root, 100);

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
  for(int i = 0; i < 5; i++) {
    toInsert = rand() % 100;
    insert(toInsert, NULL, &root);
  }

  printTree(root);

}

void testThree() {
  printf("\nTest 3 starting!\n");
  struct RBTreeNode *root = NULL;
  int toInsert = 0;
  root = init_rbtree(toInsert, NULL);
  for (int i = 1; i <= 5; i++)
    insert(i, NULL, &root);
  printTree(root);
}

void testFour() {

  printf("\nTest 4 starting!\n");
  struct RBTreeNode *root = NULL;
  int ins = 0;
  root = init_rbtree(ins, NULL);
  insert(10, NULL, &root);
  insert(-3, NULL, &root);
  insert(12, NULL, &root);
  insert(-100, NULL, &root);
  insert(18, NULL, &root);
  search_and_delete(root, -3);
  search_and_delete(root, 12);

}

void printTree(struct RBTreeNode* root) {

  /* Inorder traversal of the tree. */

  if (root->isSen != true) {
    printTree(root->left);
    print_node(root);
    printTree(root->right);

  }
}

void nullAction(int signo) {
  printf("CAUGHT: %d\nMust be a SEGFAULT\n", signo);
  exit(1);
}

void print_node(struct RBTreeNode* node) {

  int key = 0;
  color_t clr = BLACK;
  char* color = NULL;

  if (node != NULL) {

    key = node->key;
    clr = node->c;

    if (clr == BLACK)
      color = "BLACK";
    else
      color = "RED";

    printf("Node@[%p]: Key = %d, Color = %s\n", (void*)node, key, color);

  } else {
    printf("Node@[Null]\n");
  }
}
