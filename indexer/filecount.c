#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "indexer.h"

int fileCounter(char *dirname) {
  int n = 0;
  struct dirent *d;
  DIR *dir = opendir(dirname);
  if (dir == NULL) //Not a directory or doesn't exist
    return 1;
  while ((d = readdir(dir)) != NULL) {
    n++;
  }
  closedir(dir);
  return n - 3; //for mac os only
}

int main(int argc, char **argv){
  int num;

  printf("number of files is: %d\n", fileCounter(argv[1]));
  return 0;
}