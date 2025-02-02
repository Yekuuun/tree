#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char *argv[]){
    if(argc > 2){
        printf("[!] tree accept only one argument : <path_to_folder>\n");
        return EXIT_FAILURE;
    }

    char* path = NULL;

    if(argc == 2){
        const char* arg = argv[1];
        
        //check validity.

        int argLen = strlen(arg);
        path = malloc((argLen + 1) * sizeof(char));

        if(path == NULL){
            printf("[!] Error allocating memory.\n");
            return EXIT_FAILURE;
        }
        
        memcpy(path, arg, argLen);
        path[argLen] = '\0';
    }

_end:
    if(path)
        free(path);
    
    return EXIT_SUCCESS;
}