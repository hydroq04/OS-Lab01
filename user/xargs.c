#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void clear(char** x_argv, int start) {
    for (int i = start; i < MAXARG; ++i) {
        x_argv[i] = 0;
    }
}

int
main(int argc, char* argv[])
{
    if (argc > MAXARG) {
        printf("Too many arguments!\n");
        exit(1);
    }
    else if (argc < 2) {
        printf("Usage: xargs [cmd]\n");
        exit(1);
    }
    else {
        int x_argc = 0;
        char* x_argv[MAXARG];
        x_argv[0] = 0;

        // Get the command
        for (x_argc = 0; x_argc < argc - 1; ++x_argc) {
            x_argv[x_argc] = argv[x_argc + 1];
        }

        char reader;
        char buffer[1024];
        int start = 0;
        int pos = 0;

        while(read(0, &reader, 1)) {
            // Clear excess space
            if (reader == ' ') {
                buffer[pos] = 0;
                x_argv[x_argc] = &buffer[start];
                ++x_argc;
                ++pos;
                start = pos;
            }
            else if (reader == '\n') {
                int pid = fork();
                buffer[pos] = 0;
                x_argv[x_argc] = &buffer[start];
                if (pid == 0) {
                    exec(argv[1], x_argv);
                }
                else {
                    sleep(5);
                    wait((int*) 0);
                }
                --x_argc;
                clear(x_argv, x_argc);
                start = 0;
                pos = 0;
            }
            else {
                buffer[pos] = reader;
                ++pos;
            }
        }
    }
    exit(0);
}
