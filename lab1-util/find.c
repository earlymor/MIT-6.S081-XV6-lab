#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDEER_FILENO 2

void find(char* path, char* name) {
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512], *p;
    // printf("path:%s name:%s\n", path, name);
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(STDEER_FILENO, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(STDEER_FILENO, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {  // 读取目录条目
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                if (de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                if (st.type == T_FILE) {
                    if (strcmp(de.name, name) == 0) {
                        printf("%s\n", buf);
                    }
                } else if (st.type == T_DIR) {
                    find(buf, name);
                }
            }
            return;
        default:
            fprintf(STDEER_FILENO,
                    "find: the second argument should be dirtory \n");
            return;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(STDEER_FILENO, "usage:find . b\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    return 0;
}