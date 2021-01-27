#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>     /* Strings */

/**
 * Program: myShel
 * Author: Pham Sky Truong
 * Date of Last Revision: Jan 20th, 2021
 * Summary: A Linux C Shell 
 * References: Example files from CIS*3110
 */


/**
 * Count number of args in the command 
 */
int getArgsCount(char *line) {
    const char s[2] = " ";
    char *token;
    int count = 0;
    char ptr[strlen(line)];
    strcpy(ptr, line);

    /* remove trailing \n */
    strtok(ptr, "\n");

    /* get the first token */
    token = strtok(ptr, s);

    /* walk through other tokens */
    while(token != NULL) {
        count++;
        token = strtok(NULL, s);
    }
    return count;
}

/** 
 * Return 2D array of args
 */
char **getArgs(char *line, int count) {
    const char s[2] = " ";
    char *token;
    char **args = malloc(count * sizeof(char*));
    int index = 0;
    char ptr[strlen(line)];
    strcpy(ptr, line);

    /* remove trailing \n */
    strtok(ptr, "\n");

    /* get the first token */
    token = strtok(ptr, s);
    args[index] = token;

    /* walk through other tokens */
    while(token != NULL) {
        index++;
        token = strtok(NULL, s);
        args[index] = token;
    }
    args[index+1] = NULL;
    return args;
}

char *getLine() {
    char *line = NULL;
    // int ret = 0;
    size_t length;

    
}

int main(int argc, char *argv[]) {
    /**
     * Variable Declarations
     */ 
    pid_t childpid;
    int status = 0;
    char *line = NULL;
    // int ret = 0;
    size_t length;
    char **args = NULL;
    int count;

    line = (char *)malloc(256);

    do {
        printf("> ");
        // ret = getline(&line, &length, stdin);
        getline(&line, &length, stdin);

        /**
         * Create a new process
         */
        childpid = fork();
        
        /**
         * Code for parent and child process
         */
        if(childpid >= 0) { /* fork succeeded*/

            /**
             * Child Process
             */
            if(childpid == 0) {
                count = getArgsCount(line);
                args = getArgs(line, count);
                status = execvp(args[0], args);
                exit(status);
            } else {
            /**
             * Parent Process
             */
                waitpid(childpid, &status, 0);
                printf("PARENT: Child's exit code is %d\n",WEXITSTATUS(status));
            }
            
        } else {
            perror("fork");
            exit(-1);
        }
    

    } while (strncmp(line,"exit",4) != 0);

    printf("myShell terminating...\n \n");
    printf("[Process completed]\n");

    free(line);
    free(args);

    exit(0);
}
