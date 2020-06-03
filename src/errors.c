#include "errors.h"
#include<stdio.h>

void display_error(char* msg) {

  fprintf(stderr, msg);
  exit(FAIL_EXIT);

}
