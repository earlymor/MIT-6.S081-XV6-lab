/* pingpong.c
 * child   print "pid: received ping" then exit
 * parent  print "pid: received pong" then exit
 */

#include "kernel/types.h"
#include "user/user.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDEER_FILENO 2

int main(int argc, char** argv) {
    int p[2];
    int pid;
    char* str1 = "ping";
    char* str2 = "pong";
    if (pipe(p) != 0) {
        fprintf(STDEER_FILENO, "pipe create error\n");
        exit(1);
    }
    pid = fork();
    if (pid < 0) {  // error
        fprintf(STDEER_FILENO, "fork() error\n");
        exit(1);
    } else if (pid == 0) {  // child
        char buf[10];
        read(p[0], buf, 5);
        printf("%d: received %s\n",getpid(), buf);
        write(p[1], str2, 5);
        exit(0);
    } else {  // parent
        char buf[10];
        write(p[1], str1, 5);
        read(p[0], buf, 5);
        printf("%d: received %s\n",getpid(), buf);
        exit(0);
    }
}