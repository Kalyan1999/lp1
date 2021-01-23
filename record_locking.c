// Process Synchronization on I/O using Record Locking

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 64
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc,  char *argv[]) {
    if(argc != 2) {
        printf("Invalid arguments.\nUsage: %s <file>\n", argv[0]);
        return -1;
    }

    struct flock fl = {F_UNLCK, SEEK_SET, 0, 100, 0};
    int fsize, offset, fd, pid, read_size;
    char buf[BUF_SIZE];

    if((fd = open(argv[1], O_RDWR)) == -1) {
        printf("Unable to open %s: %s\n", argv[1], strerror(errno));
        return -1;
    }
    
    printf("Press any key to Lock the File\n"); 
    printf("***********************************\n");
    getchar();

    fl.l_type = F_WRLCK;
    fl.l_pid = getpid(); 
    if(fcntl(fd, F_SETLK, &fl) == -1) {
        printf("Cannot Set Exclusive Lock on %s: %s\n", argv[1], strerror(errno));
        close(fd);
        return -1;
    } else if(fl.l_type != F_UNLCK && fl.l_type != F_RDLCK)
        printf("%s has been Exclusively Locked by Process: %d\n", argv[1], fl.l_pid);
    else
        printf("%s is NOT Locked\n", argv[1]);


    printf("Press any key to release the lock\n"); 
    printf("***********************************\n");
    getchar();

    fl.l_type = F_UNLCK;
    printf("File has been Unlocked \n");
    
    fsize = lseek(fd, 0, SEEK_END);
    offset = fsize - MIN(BUF_SIZE, fsize);
    lseek(fd, offset, SEEK_SET);

    read_size = read(fd, buf, MIN(BUF_SIZE, fsize));
    buf[read_size] = '\0';
    printf("Last %d bytes:\n********************************************\n%s\n", read_size, buf);
    
    close(fd);
    return 0;
}
