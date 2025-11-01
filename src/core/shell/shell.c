#include "shell.h"
#include "../parser/parser.h"
#include <stdio.h>
#include <string.h>





void begin_shell(){
    char buffer[512];
    printf("-> ");
    while(fgets(buffer, 512, stdin)){
        char* bnonewline = strtok(buffer, "\n");
        parse_and_handle_string(bnonewline);
        printf("-> ");
    }
}
