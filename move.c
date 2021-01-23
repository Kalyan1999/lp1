// Moves <source file> into <destination file>

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Invalid arguments.\nUsage: move <source file> <destination file>\n");
        return -1;
    }

    if((link(argv[1], argv[2])) == -1) {
        printf("Unable to move file: %s\n", argv[1]);
        return -1;
    }
    if((unlink(argv[1])) == -1) {
        printf("Unable to move file: %s\n", argv[1]);    
        unlink(argv[2]);
        return -1;
    }

    printf("Successfully moved %s to %s\n", argv[1], argv[2]);
    return 0;
}