/* primes.c
 * main   wait all process then exit
 * firstprocess  feed 2,3,4...35 into pipeline and wait then exit
 * otherprocess  read left neighbor and write to right neighbor
 */

#include "kernel/types.h"
#include "user/user.h"

void process(int* left) {
    int right[2];
    int pid;
    int prime = 0;
    int num;
    pipe(right);  // 初始化右相邻管道

    while (read(left[0], &num, 4)) {  // 读左相邻管道
        if (!prime) {
            prime = num;
            printf("prime %d\n", num);
        } else {
            if (num % prime != 0) {
                write(right[1], &num, 4);
            }
        }
    }
    // 怎么确定到最后了呢？
    if (prime == 0) {
        close(left[0]);
        close(right[0]);
        close(right[1]);
        return;  // 需要用return,不能用exit,会导致编译器认为无限递归无法通过编译
    }
    close(left[0]);   // 关闭左管道读端
    close(right[1]);  // 关闭右管道写端

    if (!(pid = fork())) {
        process(right);
    } else {
        wait(&pid);
    }
    exit(0);
}
int main(int argc, char** argv) {
    int left[2];
    int pid;
    pipe(left);  // 初始化管道
    for (int i = 2; i <= 35; i++) {
        write(left[1], &i, 4);  // 向管道中写
    }
    close(left[1]);         // 关闭写端
    if (!(pid = fork())) {  // 子进程
        process(left);      // 从左管道读入，将素数写入右管道
    } else {
        close(left[0]);  // 关闭读端
        wait(&pid);
        exit(0);
    }
    return 0;
}