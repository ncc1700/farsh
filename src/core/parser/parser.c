#include "parser.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "../../process/process.h"

static char* inlinecmd[] = {"echo", "pkill"};



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