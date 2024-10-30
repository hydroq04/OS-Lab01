#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[2]) {
  if (argc != 2) {
    write(1, "Usage: sleep <number of ticks>\n", 32);
    exit(1);
  }
  else {
    write(1, "\n", 2);
    sleep(atoi(argv[1]));
  }
  exit(0);
}
