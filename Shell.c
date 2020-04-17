#define _POSIX_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

# define MAX_LINE 80 /* The maximum length command */


// Group members: Richard Fina, Justin Flores, Joshua Matson


//Parse through users string input
char ** stringParser(char * str) {

  char delim[] = " ";

  char * cptr = strtok(str, delim); // strtok returns pointer to the next token

  char ** tokens = malloc(10 * sizeof(char * ));
  
  //Initialize pointers to NULL
  for (int j = 0; j < 10; j++) {
    tokens[j] = NULL;
  }
  
  int i = 0;
  while (cptr != NULL) {
    tokens[i] = malloc(20 * sizeof(char));
    tokens[i++] = cptr;
    cptr = strtok(NULL, delim);
  }
  return tokens;
}

int main(void) {

  //Run until run != 1
  while (1) {
    printf("osh>");
    fflush(stdout);
    char userin[MAX_LINE];
    fgets(userin, MAX_LINE / 2 + 1, stdin);

    //Get rid of newline char from fgets()
    userin[strcspn(userin, "\r\n")] = 0;

    //Exit program if user input is "exit"
    if (strcmp(userin, "exit") == 0) {
      exit(0);
    }

    //Call parse function
    char ** tok = stringParser(userin);

    //Echo the tokens to output
    for (int i = 0; tok[i] == NULL; i++) {
      if(tok[i] != NULL) {
        printf("%s ", tok[i]);
      }
    }

    //Create new child process
    pid_t pid = fork();

    //Failed in creating 
    if (pid < 0) {
      perror("...failed");
      exit(EXIT_FAILURE);
    }

    //If parent process, invoke wait
    else if (pid > 0) {
      // pid > 0: Parent process
      wait(NULL);

      //If child process, exec
    } else {
      // pid == 0: child process
      int err = execvp(tok[0], tok); // execvp returns -1 if unsuccessful

      //Output error message and terminate calling process
      if (err == -1) {
        printf("Invalid command, please enter a valid command\n");
        exit(EXIT_FAILURE);
      }
    }
    //Free heap allocated pointer
    for (int i = 0; tok[i] == NULL; i++) {
      free(tok[i]);
    }
    free(tok);
  }
  return 0;
}