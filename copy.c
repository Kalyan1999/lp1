// Copies contents of <source file> into <destination file>

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Invalid arguments.\nUsage: copy <source file> <destination file>\n");
        return -1;
    }

    ssize_t retIn, retOut;
    char buffer[BUF_SIZE];

    int srcID = open(argv[1], O_RDONLY);
    if(srcID == -1) {
        printf("Unable to open %s\n", argv[1]);
        return -1;
    }
    
    int destID = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if(destID == -1) {
        printf("Unable to open %s\n", argv[2]);
        close(srcID);
        return -1;
    }

    while((retIn = read(srcID, buffer, BUF_SIZE)) > 0) {
        retOut = write(destID, buffer, retIn);
        if(retOut != retIn) {
            printf("Unable to copy files\n");
            return -1;
        }
    }

    close(srcID);
    close(destID);
    
    printf("Successfully copied %s to %s\n", argv[1], argv[2]);
    return 0;
}