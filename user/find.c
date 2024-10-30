#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char* root, char* name) {
  char buf[512], *p;
  struct dirent de;
  struct stat st;
  
  // Open starting directory
  int fd;
  if ((fd = open(root, O_RDONLY)) < 0) {
    printf("find: cannot open %s\n",root);
    return;

  }

  // Avoid long directory name
  if (strlen(root) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf("find: path %s too long\n", root);
    close(fd);
    return;
  }

  // Add seperator '/' to sub-directory
  strcpy(buf, root);
  p = buf + strlen(buf);
  *p = '/';

  // Get file/folder stats
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
      continue;
    }
    char temp[512], *t;
    strcpy(temp, buf);
    t = temp + strlen(temp);
    memmove(t,de.name,DIRSIZ);
  
  // Invalid stat
  if (stat(temp, &st) < 0) {
    printf("error %s\n", temp);
    return;
  }

  switch (st.type) {
    case T_DEVICE:

    // Print file directory
    case T_FILE:    
    if(strcmp(de.name, name) == 0){
      printf("%s\n", temp);
    }
    break;

    // Recursion
    case T_DIR:
    find(temp, name);
      break;
    }
    
  }

  close(fd);
}

int main(int argc, char* argv[]){
  if (argc <= 1) {
    fprintf(1, "usage: find <dir> <fname 1> [fname 2] [...]\n");
    exit(1);
  }
  for (int i = 2; i < argc; ++i) {
    find(argv[1], argv[i]);
  }

  exit(0);
}