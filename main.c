#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char path[1024];
    getcwd(path, sizeof(path));
    printf("%s$ ",path);

    return 0;
}
