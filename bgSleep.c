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

   int i;
   i = atoi(argv[1]);

   sleep(i);

   exit (0);
}
