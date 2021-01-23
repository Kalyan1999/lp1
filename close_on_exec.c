// Creates a file named "close-on-exec.txt" with permissions 666 and close-on-exec flag set.

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd;
    if((fd = open("close-on-exec.txt", O_WRONLY|O_CREAT, 0666)) == -1) {
        printf("Unable to open the file.");
        return -1;
    }
    
    int old_flags = fcntl(fd, F_GETFD);
    printf("Old flags: %d\n", old_flags);

    fcntl(fd, F_SETFD, FD_CLOEXEC);

    int new_flags = fcntl(fd, F_GETFD);
    printf("New flags: %d\n", new_flags);
    
    return  0;
}