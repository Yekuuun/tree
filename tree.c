#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/**
 * Base function to handle path.
 * @param path => path provided as tree arg.
 */
static BOOL IsValidPath(IN const char *path){
    int len = strlen(path);

    if(len == 0 || path[0] == '\0')
        return FALSE;
    
    if(len >= MAX_PATH){
        printf("[!] Path too long.\n");
        return FALSE;
    }

    const char* invalidChars = "\\:*?\"<>|";
    if(strpbrk(path, invalidChars) != NULL){
        printf("[!] Invalid caracters. \n");
        return FALSE;
    }

    //check if path exist.
    DWORD dwAttrib = GetFileAttributes((LPCSTR)path);
    if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
        return TRUE;
    
    printf("[!] Not a valid path provided.\n");
    return FALSE;
}

//entry point.
int main(int argc, char *argv[]){
    if(argc > 2){
        printf("[!] tree accept only one argument : <path_to_folder>\n");
        return EXIT_FAILURE;
    }

    char* path = NULL;

    if(argc == 2){
        const char* arg = argv[1];
        
        //check validity.
        if(!IsValidPath(arg))
            return EXIT_FAILURE;

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