#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>     /* Strings */

#define LINE_LN 256
#define WORD_LN 50
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/**
 * Program: myShell
 * Author: Pham Sky Truong
 * Date of Last Revision: Jan 27th, 2021
 * Summary: A Linux C Shell 
 * References: Example files from CIS*3110
 */

/* Error check for args array */
void printArgs(char **args, int count) {
    if(*args != NULL) {
        for(int i = 0; i < count+1; i++) {
            printf("args[%d]: %s\n", i, args[i]);
        }
    }
}

/**
 * Count number of args in the command 
 */
int getArgsCount(char *line) {
    const char s[2] = " ";
    char *token;
    int count = 0;
    char ptr[strlen(line)+1];
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
    const char s[10] = " \t\r\n\a";
    char *token;
    char **args = malloc(sizeof(char*) * (count+1));
    int index = 0;

    if(!args) {
        fprintf(stderr, "Error: malloc error in getArgs\n");
        exit(EXIT_FAILURE);
    }
    
    /* get the first token */
    token = strtok(line, s);

    /* walk through other tokens */
    while(token != NULL) {
        args[index] = token;
        index++;
        token = strtok(NULL, s);
    }
    args[index] = NULL;

    return args;
}

/**
 * Return user command line
 */
char *getLine(void) {
    char *line = NULL;
    size_t length = LINE_LN;
    // ssize_t nread;

    line = (char *)malloc(LINE_LN);
    getline(&line, &length, stdin);

    return line;
}

/**
 * Forking process
 */
void forkProcess(char **args, int count) {
    pid_t childpid;
    int status = 0;

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

}


int main(int argc, char *argv[]) {

    char *line = NULL;
    char **args = NULL;
    int count;    

    /* Initiate prompt */
    printf("> ");
    line = getLine();
    strtok(line, "\n"); /* Remove trailing \n */


    while (strncmp(line,"exit",4) != 0) {

        /* Parse arguments */
        count = getArgsCount(line);
        args = getArgs(line, count);
        forkProcess(args, count);

        /* Free previous data */
        free(args);
        free(line);

        /* Re-prompt */
        printf("> ");
        line = getLine();
        strtok(line, "\n");

    }

    printf("myShell terminating...\n \n");
    printf("[Process completed]\n");
    
        
    if(strncmp(line,"exit",4) == 0) {
        free(line);
    }
    
    return EXIT_SUCCESS;
}
