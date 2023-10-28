#include "kernel/types.h"
#include "user/user.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDEER_FILENO 2

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(STDEER_FILENO, "sleep: missing action object\n");
        exit(1);
    } else if (argc > 2) {
        fprintf(STDEER_FILENO, "too much arguments\n");
        exit(1);
    } else {
        int ticks = atoi(argv[1]);
        sleep(ticks);
        exit(0);
    }
}