// Displays directory info like 'ls -ls'

#include <stdio.h>
#include <math.h>
#include "ls_helper.h"

#define true (1)
#define false (0)
#define MAX_ENTRIES 1024
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Invalid arguments.\nUsage: ls_dir <dir> \n");
        exit(1);
    }

    // Variables for formatting
    int p_blockno = 0;
    int p_nlink = 0;
    int p_size = 0;

    // Variables
    int len = 0, total_blocks = 0;
    int dirfd = -1;
    DIR *dir = NULL;
    struct dirent *direntry;
    file_info infos[MAX_ENTRIES];

    if((dir = opendir(argv[1])) == NULL) {
        printf("Unable to open %s.\n", argv[1]);
        exit(1);
    }

    if((dirfd = open(argv[1], O_RDONLY)) < 0) {
        printf("Unable to open %s.\n", argv[1]);
        closedir(dir);
        exit(1);
    }
    
    while(true) {
        if((direntry = readdir(dir)) == NULL)
            break;

        if(direntry->d_name[0] == '.')
            continue;
        

        if(getFileInfoAt(&infos[len], direntry->d_name, dirfd) < 0) {
            printf("Unable to retrive File Details");
            return -1;
        }
        
        total_blocks += infos[len].block_no;
        
        p_blockno = MAX((int)log10(infos[len].block_no)+1, p_blockno);
        p_nlink = MAX((int)log10(infos[len].nlink)+1, p_nlink);
        p_size = MAX((int)log10(infos[len].size)+1, p_size);
        len++;
    }

    printf("total %d\n", total_blocks);
    for(int i = 0; i < len; i++) {
        printf("%*ld %c%s %*ld %s %s %*ld %s %s\n", p_blockno, infos[i].block_no, 
            infos[i].type, infos[i].perms, p_nlink, infos[i].nlink, infos[i].uname, 
            infos[i].gname, p_size, infos[i].size, infos[i].time_str, infos[i].name);
    }

    if(dirfd != -1) {
        close(dirfd);
    }
    if(dir != NULL) {
        closedir(dir);
    }

    return 0;
}