#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_DEPTH 20

typedef struct {
    BOOL isLastChild[MAX_DEPTH];
    INT fileCount;
    INT dirCount;
} TreeContext;

typedef TreeContext* PTreeContext;

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

/**
 * Setting up console for display.
 */
VOID SetupConsole() {
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

/**
 * Printing prefixes properly.
 * @param ctx => ThreeContext ptr
 * @param level => current level
 */
VOID PrintPrefix(PTreeContext ctx, int level) {
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            printf(ctx->isLastChild[i] ? "└───" : "├───");
        } 
        else {
            printf(ctx->isLastChild[i] ? "    " : "│   ");
        }
    }
}

/**
 * Main TREE function
 * @param path => path for search (dir)
 * @param level => current level
 * @param ctx => ThreeContext
 */
VOID Tree(IN const char* path, IN INT level, IN PTreeContext ctx){
    WIN32_FIND_DATA findData; 
    HANDLE hFind = NULL;

    char searchPath[MAX_PATH];
    char nextPath[MAX_PATH];

    sprintf(searchPath, "%s\\*", path); //building search path.

    hFind = FindFirstFile(searchPath, &findData);
    if(hFind == INVALID_HANDLE_VALUE)
        return;
    
    int count = 0;

    while(FindNextFile(hFind, &findData)){
        if(strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;
        
        count++;
    }

    FindClose(hFind);
    hFind = FindFirstFile(searchPath, &findData);

    if(hFind == INVALID_HANDLE_VALUE)
        return;
    
    int currentItem = 0;

    while(FindNextFile(hFind, &findData)){
        if(strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;
        
        currentItem++;
        ctx->isLastChild[level] = (currentItem == count);

        PrintPrefix(ctx, level);
        printf("%s\n", findData.cFileName);

        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            sprintf(nextPath, "%s\\%s", path, findData.cFileName);
            ctx->dirCount++;
            Tree(nextPath, level + 1, ctx);
        }
        else {
            ctx->fileCount++;
        }
    }

    FindClose(hFind);
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

    SetupConsole();
    TreeContext ctx = {0};
    RtlSecureZeroMemory(&ctx, sizeof(TreeContext));

    printf("%s\n", path);
    Tree(path, 0, &ctx);

    printf("\n");
    printf("%d directories, %d files\n", ctx.dirCount, ctx.fileCount);

_end:
    if(path)
        free(path);
    
    return EXIT_SUCCESS;
}