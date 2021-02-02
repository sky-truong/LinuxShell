#include <stdio.h>       /* Input/Output */
#include <stdlib.h>      /* General Utilities */
#include <unistd.h>      /* Symbolic Constants */
#include <sys/types.h>   /* Primitive System Data Types */
#include <sys/wait.h>    /* Wait for Process Termination */
#include <errno.h>       /* Errors */
#include <string.h>      /* String Library */

/*
 *   CIS*3110 Assignment #1
 *
 *      Author: Deb Stacey 
 *      Date of Last Revision: December 29, 2020
 *      Summary: 
 *               
 *      References: Based on code found at http://www.amparo.net/ce155/fork-ex.html
 */

int main ( int argc, char *argv[] ) {

   int i = 0;
   int q = 0;
   int ret;
   size_t length;
   char *line;
   line = (char *)malloc(256);

   q = atoi(argv[1]); /* number of questions */

   for ( i=0; i<q; i++ ) {
      printf ("What is your question? > ");
      ret = getline(&line, &length, stdin);
      strtok(line,"\n");
      printf ("Question %d: %s\n",i+1,line); 
   }

   printf ( "Bye!\n" );
   exit (0);
}
