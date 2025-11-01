#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <dirent.h>


int get_pid_from_image(const char* image){
    DIR* dir = opendir("/proc");
    if(dir == NULL) return 0;
    struct dirent* entry;
    while((entry = readdir(dir)) != NULL){
        char filepath[512]; // idk why 512
        char comm[512];
        if(entry->d_type != DT_DIR) continue;
        sprintf(filepath, "/proc/%s/comm", entry->d_name);
        int pid = atoi(entry->d_name);
        if(pid == 0){
            continue;
        } 
        
        int fd = open(filepath, O_RDONLY);
        if(fd == -1){
            continue;
        }
        int res = read(fd, comm, 512);
        if(res == -1){
            close(fd);
            continue;
        }
        char* parsecomm = strtok(comm, "\n");
        if(strcmp(image, parsecomm) == 0){
            close(fd);
            closedir(dir);
            return pid;
        }
        close(fd);
    }
    closedir(dir);
    return 0;
}