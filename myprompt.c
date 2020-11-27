#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h> //fork()
#include <time.h> //for date
#include <string.h> //command comparisons

#include <stdio.h>

#include "mypro.h"

int main(int argc, char *argv[]) {

  printf("M.L.A.J. prompt [version 0.1.0]\n2020.2 O.S. class\n\n");

  prompt_loop();

  return 0;

}
