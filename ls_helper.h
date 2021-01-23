#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef struct {
    long int block_no;
    char type;
    char *perms;
    long int nlink;
    char *uname;
    char *gname;
    long int size;
    char *time_str;
    char *name;
} file_info;

int getFileInfo(file_info*, char*);
int getFileInfoAt(file_info*, char*, int);

int getFileInfoAt(file_info *info, char* pathname, int dirfd) {
    struct stat st;
    struct passwd *pwd;
    struct group *grp;
    
    char type;
    if(fstatat(dirfd, pathname, &st, 0) < 0) {
        return -1;
    }

    char *time_str = (char*) calloc(20, sizeof(char));
    char *perms = (char*) calloc(10, sizeof(char));

    pwd = getpwuid(st.st_uid);
    grp = getgrgid(st.st_gid);
    
    long int block_no = st.st_blocks / 2;
    strftime(time_str, 20, "%b %d %H:%M", localtime(&st.st_mtime));
    
    if(S_ISREG(st.st_mode))             
        type = '-';
    else if(S_ISDIR(st.st_mode)) {
        type = 'd';
        block_no = 0;
    } else if(S_ISCHR(st.st_mode))
        type = 'c';
    else if(S_ISBLK(st.st_mode))
        type = 'b';
    else if(S_ISFIFO(st.st_mode))
        type = 'p';
    else if(S_ISLNK(st.st_mode))
        type = 'l';
    else if(S_ISSOCK(st.st_mode))
        type = 's';

    perms[0] = (S_IRUSR & st.st_mode) ? 'r' : '-';
    perms[1] = (S_IWUSR & st.st_mode) ? 'w' : '-';
    perms[2] = (S_IXUSR & st.st_mode) ? 'x' : '-';
    perms[3] = (S_IRGRP & st.st_mode) ? 'r' : '-';
    perms[4] = (S_IWGRP & st.st_mode) ? 'w' : '-';
    perms[5] = (S_IXGRP & st.st_mode) ? 'x' : '-';
    perms[6] = (S_IROTH & st.st_mode) ? 'r' : '-';
    perms[7] = (S_IWOTH & st.st_mode) ? 'w' : '-';
    perms[8] = (S_IXOTH & st.st_mode) ? 'x' : '-';
    perms[9] = '\0';

    info->block_no = block_no;
    info->type = type;
    info->perms = perms;
    info->nlink = st.st_nlink;
    info->uname = pwd->pw_name;
    info->gname = grp->gr_name;
    info->size = st.st_size;
    info->time_str = time_str;
    info->name = pathname;

    return 0;
}

int getFileInfo(file_info *info, char* pathname) {
    return getFileInfoAt(info, pathname, AT_FDCWD);
}