#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 1) {
    write(1, "Usage: pingpong\n", 17);
    exit(1);
  }
  else {
    int p_in[2], p_out[2];
    pipe(p_in);
    pipe(p_out);
    int pid = fork();

    if (pid == 0) {
      close(p_in[1]);
      char buf1[1];
      read(p_in[0], buf1, sizeof(buf1));
      printf("%d: received ping\n", getpid());
      close(p_in[0]);

      close(p_out[1]);
      write(p_out[0], "o", 1);
      close(p_out[0]);
    }
    else if (pid > 0) {
      close(p_in[0]);
      write(p_in[1], "i", 1);
      close(p_in[1]);
      wait((int*) 0);

      close(p_out[1]);
      char buf2[1];
      read(p_out[0], buf2, sizeof(buf2));
      printf("%d: received pong\n", getpid());
      close(p_out[0]);
    }
    else {
      write(1, "fork error\n", 12);
      exit(1);
    }
  }
  exit(0);
}