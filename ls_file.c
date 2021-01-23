// Displays file info like 'ls -ls'

#include <stdio.h>
#include <math.h>
#include "ls_helper.h"

#define MAX(a,b) (((a)>(b))?(a):(b))

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        printf("Invalid arguments.\nUsage: ls_file <file> [<file> <file>]\n");
        return -1;
    }

    // Variables for formatting
    int p_blockno = 0;
    int p_nlink = 0;
    int p_size = 0;
    file_info infos[argc-1];

    for(int i = 1; i < argc; i++) {
        if(getFileInfo(&infos[i-1], argv[i]) < 0) {
            printf("Unable to retrive File Details");
            return -1;
        }

        p_blockno = MAX((int)log10(infos[i-1].block_no)+1, p_blockno);
        p_nlink = MAX((int)log10(infos[i-1].nlink)+1, p_nlink);
        p_size = MAX((int)log10(infos[i-1].size)+1, p_size);
    }

    for(int i = 0; i < (argc-1); i++) {
        printf("%*ld %c%s %*ld %s %s %*ld %s %s\n", p_blockno, infos[i].block_no, 
            infos[i].type, infos[i].perms, p_nlink, infos[i].nlink, infos[i].uname, 
            infos[i].gname, p_size, infos[i].size, infos[i].time_str, infos[i].name);
    }
}