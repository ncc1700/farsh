#include "parser.h"
#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../process/process.h"

static char* inlinecmd[] = {"echo", "pkill", "cd", "pwd", "dir"};


static inline int match_to_inline_command(int index, char* arg){
    switch(index){
        case 0:
            if(arg == NULL) return 0;
            puts(arg);
            return 0;
        case 1:{
            if(arg == NULL) return 0;
            char* str = strtok(arg, "\n");
            int pid = get_pid_from_image(str);
            if(pid == 0){
                puts("get_pid_from_image(): Couldn't find the process");
                return 0;
            }
            int result = kill(pid, 0);
            if(result != 0){
                perror("kill()");
            }
            return 0;
        }
        case 2:{
            int res = chdir(arg);
            if(res == -1){
                perror("chdir()");
            }
            return 0;
        }
        case 3:{
            char* res = getcwd(NULL, 0);
            if(res == NULL){
                perror("getcwd()");
                return 0;
            }
            puts(res);
            free(res);
            return 0;
        }
        case 4:{
            DIR *dir = NULL;
            if(arg == NULL){
                char* res = getcwd(NULL, 0);
                if(res == NULL){
                    perror("getcwd()");
                    return 0;
                }
                dir = opendir(res);
                free(res);
            } else dir = opendir(arg);
            if(dir == NULL){
                perror("opendir()");
                return 0;
            }
            struct dirent* entry;
            int i = 1;
            while((entry = readdir(dir)) != NULL){
                char* file = strtok(entry->d_name, "\n");
                if(entry->d_type == DT_DIR){
                    printf("[(DIR) %s ] ", file);
                } else printf("[%s] ", file);
                if(i % 5 == 0) puts("");
                i++;
            }
            closedir(dir);
        }
    }
    return -1;
}


static inline void match_and_handle_command(char* cmd, char* arg){
    int arlen = sizeof(inlinecmd) / sizeof(inlinecmd[0]);

    for(int i = 0; i < arlen; i++){
        if(strcmp(inlinecmd[i], cmd) == 0){
            match_to_inline_command(i, arg);
            return;
        }
    }
    printf("Not found!\n");
}



void parse_and_handle_string(char* cmdline){
    char* cmd = strtok(cmdline, " ");
    char* arg = strtok(NULL, "\n");

    match_and_handle_command(cmd, arg);
}