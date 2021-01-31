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
 * Date of Last Revision: Jan 30th, 2021
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
int getArgsCount(char *line) {
    const char s[10] = " \t\r\n\a";
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
 * References: https://fedemengo.github.io/blog/2018/02/SIGCHLD-handler.html
 */
void killZombies(int signo) {
    pid_t childpid;
    int status;

    while((childpid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("[%d] Reaped\n", childpid);
    }
}

/**
 * Forking process
 */
void forkProcess(char **args, int isBackground, int reOutFile, char *outFile, char reInFile, char *inFile) {
    pid_t childpid;
    int status;

    /**
     * Set up signal handler
     */
    struct sigaction sigact;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);

    sigact.sa_handler = killZombies;
    if(sigaction(SIGCHLD, &sigact, NULL) < 0) {
        perror("sigaction() failed");
        exit(1);
    }
    sigaction(SIGCHLD, &sigact, NULL);

    /**
     * Create a new process
     */
    childpid = fork();
    
    /**
     * Child Process
     */
    if(childpid == 0) {
        if(isBackground) {
            printf("[%d] Started\n", getpid());
        }

        // Source: http://digi-cron.com:8080/programmations/c/lectures/8-dup.html
        int in, out;

        // Redirect stdin
        if(reInFile) {
            if((in = open(inFile, O_RDONLY)) == -1) {
                perror("open inFile");
                exit(3);
            }
        }
        close(fileno(stdin));
        dup(in);

        // Redirect stdout
        if(reOutFile) {
            if((out = open(outFile, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) { // 0600 = owner has read permission
                perror("open outFile");
                exit(3);
            }
        }
        dup2(out, fileno(stdout));

        // if(reOutFile) {
        //     freopen(outFile, "w", stdout);
        // }
        // if(reInFile) {
        //     freopen(inFile, "r", stdin);
        // }

        status = execvp(args[0], args);
        exit(status);

    } else if(childpid < 0) {
        perror("fork");
        exit(-1);
    }

    /**
     * Parent Process
     */
    // Child is NOT a background process, parent will be blocked
    // and wait until child exits or terminated by a signal
    if(!isBackground) {
        // isBackground = 0;
        do {
            waitpid(childpid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));

    // Child is A background process, parent continues
    // Use a signal handler to check and reap child
    } else {
        sigact.sa_handler = SIG_DFL;
        sigaction(SIGCHLD, &sigact, NULL);
    }
}


int main(int argc, char *argv[]) {

    char *line = NULL;
    char **args = NULL;
    char outFile[FILE_LN];
    char inFile[FILE_LN];
    int count;    
    int run = 1;
    int isBackground;
    int reOutFile;
    int reInFile;

    /* Initiate prompt */
    printf("> ");
    line = getLine();
    strtok(line, "\n"); /* Remove trailing \n */


    while(run) {

        /* Parse arguments */
        count = getArgsCount(line);
        args = getArgs(line, count, &isBackground, &reOutFile, outFile, &reInFile, inFile);

// if(reOutFile == 1) {
// printf("reOutFile: %d\n", reOutFile);
// printf("outFile: %s\n", outFile);
// }
// if(reInFile == 1) {
// printf("reInFile: %d\n", reInFile);
// printf("inFile: %s\n", inFile);
// }

        forkProcess(args, isBackground, reOutFile, outFile, reInFile, inFile);

        /* Free previous data */
        free(args);
        free(line);

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
