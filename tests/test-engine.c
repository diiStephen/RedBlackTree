/*

Test engine for rbtree library. Commands accepted from stdin
for building an RBT and performing queries. As commands are accepted
from stdin, redirection can be used to create test files.

As this is for test purposes, only ints are allowed as
parameters to commands.

Accepted commads and their formats:

1. ins x -- insert x into the tree.

2. del x -- remove x from the tree.

3. srh x -- search for x in tree.

4. max -- display max key in the tree.

5. min -- display min key in the tree.

6. prt -- print tree in-order.

7. rot -- print details of root node.

8. hit -- print height of the tree.

9. end -- shutdown the test program.

*/

#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Constants
#define CMD_L 4
#define EXT_F 1

//Macros
#define gather(cmd, param)\
  do{\
    printf("[C]: ");\
    scanf("%s", cmd);\
    if (strcmp(cmd,"end") != 0 && strcmp(cmd,"prt") != 0 && strcmp(cmd,"rot") != 0 && strcmp(cmd,"hit") != 0) {\
      scanf("%d", &param);\
    }\
  } while (0)

#define print_node(node)\
  do{\
    int key = 0;\
    color_t clr = BLACK;\
    char* color = NULL;\
    if (node != NULL) {\
      key = node->key;\
      clr = node->c;\
      if (clr == BLACK){\
        color = "BLACK";\
      } else {\
        color = "RED";\
      }\
      printf("Node@[%p]: Key = %d, Color = %s\n", (void*)node, key, color);\
    } else {\
        printf("Node@[Null]\n");\
      }\
    } while (0)

// Prototypes.
void exec_ins(struct RBTreeNode **, int);
void exec_del(struct RBTreeNode **, int);
void exec_srh(struct RBTreeNode *, int);
void exec_prt(struct RBTreeNode *);
void inorder(struct RBTreeNode *);
void exec_rot(struct RBTreeNode *);
void exec_hit(struct RBTreeNode *);

int main(int argc, char** argv) {

  printf("Starting Test Engine!\n");
  char* cmd = malloc(sizeof(char)*CMD_L);
  if (cmd == NULL) {
    fprintf(stderr, "Error allocating memory for cmd buffer!\n");
    exit(EXT_F);
  }
  int param = 0;

  printf("Initalizing RBT!\n");
  struct RBTreeNode *root = NULL;

  gather(cmd, param);
  while(strcmp(cmd, "end")) {
    printf("Commands: %s\n", cmd);

    if (strcmp(cmd,"ins") == 0) {
      exec_ins(&root, param);
    } else if (strcmp(cmd,"del") == 0){
      exec_del(&root, param);
    } else if (strcmp(cmd,"srh") == 0) {
      exec_srh(root, param);
    } else if (strcmp(cmd,"prt") == 0) {
      exec_prt(root);
    } else if (strcmp(cmd,"rot") == 0) {
      exec_rot(root);
    } else if (strcmp(cmd,"hit") == 0) {
      exec_hit(root);
    } else {
      printf("Unreconized.\n");
    }

    gather(cmd,param);
  }

  // Testing different memory free techniques.
  printf("Shutting down!\n");
  dest_rbtree(&root);
  free(cmd);
  return 0;
}

void exec_ins(struct RBTreeNode **root, int p) {
  if(*root == NULL)
    *root = init_rbtree(p, NULL);
  else
    insert(root, p, NULL);
}

void exec_del(struct RBTreeNode **root, int p) {
  search_and_delete(root, p);
}

void exec_srh(struct RBTreeNode *root, int p) {
  struct RBTreeNode *res = search(p, root);
  if(res == NULL)
    printf("Search failed.\n");
  else
    print_node(res);
}


void exec_prt(struct RBTreeNode* root) {
  if(root != NULL)
    inorder(root);
  else
    printf("Empty tree!\n");
}

// Inorder traversal.
void inorder(struct RBTreeNode* root) {
  if (root->isSen != true) {
    inorder(root->left);
    print_node(root);
    inorder(root->right);
  }
}

void exec_rot(struct RBTreeNode *root) {
  if (root == NULL)
    printf("Empty tree!\n");
  else
    print_node(root);
}

void exec_hit(struct RBTreeNode *root) {
  int h = 0;
  h = height(root);
  printf("height(T) = %d\n", h);
}
