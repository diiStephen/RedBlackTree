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

6. end -- shutdown the test program.

*/

#include "rbtree.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CMD_L 4
#define EXT_F 1

#define gather(cmd, param)\
  do{\
    printf("[C]: ");\
    scanf("%s", cmd);\
    if (strcmp(cmd,"end") != 0) {\
      scanf("%d", &param);\
    }\
  } while (0)



int main(int argc, char** argv) {

  printf("Starting test engine!\n");
  char* cmd = malloc(sizeof(char)*CMD_L);
  if (cmd == NULL) {
    fprintf(stderr, "Error allocating memory for cmd buffer!\n");
    exit(EXT_F);
  }
  int param = 0;

  gather(cmd, param);
  while(strcmp(cmd, "end")) {
    printf("Commands: %s\n", cmd);
    //execute
    gather(cmd,param);
  }

  return 0;
}
