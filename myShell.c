#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>     /* Strings */
#include <signal.h>     /* Signals */
#include <fcntl.h>

#define LINE_LN 256
#define FILE_LN 50

/**
 * Program: myShell
 * Author: Pham Sky Truong
 * Date of Last Revision: Feb 2nd, 2021
 * Summary: A Linux C Shell 
 * References: Example files from CIS*3110
 */

/** 
 * Extra function
 * Error check for args array 
 */
void printArgs(char **args, int count) {
    if(*args != NULL) {
        for(int i = 0; i < count+1; i++) {
            printf("args[%d]: %s\n", i, args[i]);
        }
    }
}

/**
 * Check if user wants to exit shell
 */
int runShell(char *input) {
    if(strncmp(input,"exit",4) == 0 || 
        strncmp(input,"quit",4) == 0 || 
        strncmp(input,"q",1) == 0) {
            return 0;
        }
    return 1;
}

/**
 * Count number of args in the command 
 */
void getArgsInfo(char *line, int *argsCount, int *hasPipe, int *argsPipeCount) {
    const char s[2] = " ";
    char *token;
    int temp = 0;
    (*argsCount) = 0;
    (*hasPipe) = 0;
    (*argsPipeCount) = 0;
    char ptr[strlen(line)+1];
    strcpy(ptr, line);

    /* remove trailing \n */
    strtok(ptr, "\n");

    /* get the first token */
    token = strtok(ptr, s);

    /* walk through other tokens */
    while(token != NULL) {
        if(strncmp(token,"|",1) == 0) {
            *hasPipe = 1;
            temp = (*argsCount);
        }
        (*argsCount)++;
        token = strtok(NULL, s);
    }
    if(*hasPipe) {
        *argsPipeCount = (*argsCount) - temp - 1;
        *argsCount = (*argsCount) - (*argsPipeCount)-1;
    }
}

/** 
 * Return 2D array of args
 */
char **getArgs(char *line, int count, int *isBackground, int *reOutFile, char *outFile, int *reInFile, char *inFile) {
    const char s[10] = " \t\r\n\a";
    char *token;
    char **args = malloc(sizeof(char*) * (count+1));
    char temp[2];
    int index = 0;
    *isBackground = 0; // Default foreground process
    *reOutFile = 0; // No redirecting to output file
    *reInFile = 0; // No redirecting from input file

    if(!args) {
        fprintf(stderr, "Error: malloc error in getArgs\n");
        exit(EXIT_FAILURE);
    }
    
    /* get the first token */
    token = strtok(line, s);

    /* walk through other tokens */
    while(token != NULL) {
        if(strncmp(token,"&",1) == 0) { // Asserts background process
            *isBackground = 1;
        } else if(strncmp(token,">",1) == 0) {
            *reOutFile = 1;
            strcpy(temp,">");
        } else if(strncmp(token,"<",1) == 0) {
            *reInFile = 1;
            strcpy(temp,"<");
        } else if(*reOutFile == 1 && strncmp(temp,">",1) == 0) { 
            strcpy(outFile,token);
        } else if(*reInFile == 1 && strncmp(temp,"<",1) == 0) { 
            strcpy(inFile,token);
        } else {
            args[index] = token;
            index++;
        }
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

    line = (char *)malloc(LINE_LN);
    getline(&line, &length, stdin);

    return line;
}

/**
 * Signal handler to reap child processes
 */
void killZombies(int signo) {
    pid_t childPid;
    int status;

    while((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[%d] Reaped\n", childPid);
    }
}

/**
 * Fork and execute command, no pipe
 */
void execArgs(char **args, int isBackground, int reOutFile, char *outFile, char reInFile, char *inFile) {
    pid_t childPid;
    int status;

    /**
     * Set up signal handler
     */
    struct sigaction sigact;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

    sigact.sa_handler = killZombies;
    if(sigaction(SIGCHLD, &sigact, NULL) < 0) {
        perror("sigaction()");
        exit(1);
    }
    sigaction(SIGCHLD, &sigact, NULL);

    /**
     * Create a new process
     */
    if((childPid = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /**
     * Child Process
     */
    if(!childPid) { // childPid == 0
        if(isBackground) {
            printf("[%d] Started\n", getpid());
        }

        // Redirect I/O files
        if(reOutFile) {
            freopen(outFile, "w", stdout);
        }
        if(reInFile) {
            freopen(inFile, "r", stdin);
        }

        if(execvp(args[0], args) == -1) {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Parent Process
     */
    // Child is NOT a background process, parent will be blocked
    // and wait until child exits or terminated by a signal
    if(!isBackground) {
        do {
            waitpid(childPid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    // Child is A background process, parent continues
    // Use a signal handler to check and reap child

    // pid_t somekidpid;
    // if((somekidpid = waitpid(-1, &status, WNOHANG)) > 0) {
    //     printf("[%d] Reaped\n", somekidpid);
    // }

    sigact.sa_handler = SIG_DFL;
    sigaction(SIGCHLD, &sigact, NULL);
}

/**
 * Fork and execute command with pipe
 */
void execArgsPipe(char **args, char **argsPipe, int reOutFile, char *outFile, char reInFile, char *inFile, int reOutFilePipe, char *outFilePipe, int reInFilePipe, char *inFilePipe) {
    pid_t childPid[2];
    int status;
    int pipeFd[2];

    /**
     * Create pipe
     */
    if(pipe(pipeFd) < 0) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    /**
     * Create the first process
     */
     if((childPid[0] = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    /**
     * Child Process
     */
    if(!childPid[0]) {

        // Redirect I/O files
        if(reOutFile) {
            freopen(outFile, "w", stdout);
        }
        if(reInFile) {
            freopen(inFile, "r", stdin);
        }

        // Child closes the write end of the pipe
        // then set stdout to pipe
        close(pipeFd[0]);
        dup2(pipeFd[1], fileno(stdout));
        close(pipeFd[1]);

        if(execvp(args[0], args) == -1) {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
        
    }

    /**
     * Parent Process
     */
    if(childPid[0]) {
        /*
         * Create the next process
         */
        if((childPid[1] = fork()) < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        /* Child process */
        if(!childPid[1]) { 

            // Redirect I/O files
            if(reOutFilePipe) {
                freopen(outFilePipe, "w", stdout);
            }
            if(reInFilePipe) {
                freopen(inFilePipe, "r", stdin);
            }

            // Parent closes the read end of the pipe
            // then set stdin to pipe
            close(pipeFd[1]);
            dup2(pipeFd[0],fileno(stdin));
            close(pipeFd[0]);

            if(execvp(argsPipe[0], argsPipe) == -1) {
                perror(argsPipe[0]);
                exit(EXIT_FAILURE);
            }
        }
        
        /* Parent process */
        close(pipeFd[0]);
        close(pipeFd[1]);
        do {
            waitpid(childPid[1], &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    // pid_t somekidpid;
    // if((somekidpid = waitpid(-1, &status, WNOHANG)) > 0) {
    //     printf("[%d] Reaped\n", somekidpid);
    // }

    // sigact.sa_handler = SIG_DFL;
    // sigaction(SIGCHLD, &sigact, NULL);
}


int main(int argc, char *argv[]) {

    char *line = NULL;
    char **args = NULL;
    char outFile[FILE_LN], inFile[FILE_LN];
    int run = 1;
    int argsCount, isBackground, reOutFile, reInFile;

    char **argsPipe = NULL;
    int hasPipe;
    char outFilePipe[FILE_LN], inFilePipe[FILE_LN];
    int argsPipeCount, isBackgroundPipe, reOutFilePipe, reInFilePipe;

    /* Initiate prompt */
    printf("> ");
    line = getLine();
    strtok(line, "\n"); /* Remove trailing \n */


    while(run) {

        /* Parse arguments */
        getArgsInfo(line, &argsCount, &hasPipe, &argsPipeCount);

        /* If there's pipe, break into 2 strings to be parsed */
        char *firstCmd = line, *secondCmd = line;
        firstCmd = strsep(&secondCmd,"|");

        args = getArgs(firstCmd, argsCount, &isBackground, &reOutFile, outFile, &reInFile, inFile);

        if(hasPipe) {
            argsPipe = getArgs(secondCmd, argsPipeCount, &isBackgroundPipe, &reOutFilePipe, outFilePipe, &reInFilePipe, inFilePipe);
        }

        /* Fork and exec */
        if(!hasPipe) {
            execArgs(args, isBackground, reOutFile, outFile, reInFile, inFile);
        } else {
            execArgsPipe(args, argsPipe, reOutFile, outFile, reInFile, inFile, reOutFilePipe, outFilePipe, reInFilePipe, inFilePipe);
        }

        /* Free previous data */
        free(args);
        free(line);
        if(hasPipe) {
            free(argsPipe);
        }

        /* Re-prompt */
        printf("> ");
        line = getLine();
        strtok(line, "\n");

        /* Continue? */
        run = runShell(line);
    }

    printf("myShell terminating...\n \n");
    printf("[Process completed]\n");
    
        
    if(!run) {
        free(line);
    }
    
    return EXIT_SUCCESS;
}
