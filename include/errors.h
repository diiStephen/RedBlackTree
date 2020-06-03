#ifndef ERRORS_H
#define ERRORS_H

/****** ERROR MESSAGES ******/

#define MEM_ERROR "Could not allocate memory.\n"
#define INV_NODE "Invalid RB-Tree Node.\n"
#define NULL_NODE "Node is null!\n"
#define INV_DELOC "Cannot deallocate node, data present.\n"

#define FAIL_EXIT 1

/****** FUNCTION PROTOTYPES ******/

void display_error(char*);



#endif
