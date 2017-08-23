#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_STR_LEN 128
#define MAX_ARGS 128

/* Prints the '$' symbol and reads buf */
void print_prompt(char buf[]) {
    write(1, "$ ", 2);
    read(0, buf, (MAX_STR_LEN-1));
}

/* Resets the buf array and argv array to read command */
void clear_arrays(char buf[], char* argv[]) {
    memset(buf, 0, (MAX_STR_LEN-1));
    memset(argv, 0, MAX_ARGS);
}

/* Splits the buf by spaces, new line characters and places these arguments into the argv array */
int parse_args(char buf[], char* argv[]) {

    int argc = 0;
    argv[argc] = strtok(buf, " \n");
    argc++;
    while ((argv[argc] = strtok(NULL, " \n")) != NULL) {
        argc++; 
    }
    argv[argc] = NULL;

    return argc;
}

/* The parent process creates a pipe and two children processes,
 * changes the respective file decriptors, and executes both programs.
 */
void execute_pipe(char* argv[], char* argv1[]) {
    pid_t id;
    int filedes[2];

    pipe(filedes);
    id = fork();

    if (id == 0) {
        close(1);
        dup(filedes[1]);
        close(filedes[0]);

        if (execvp(argv[0], argv) < 0) {
            printf("Invalid command: %s.\n", argv[0]);
            exit(1);
        }
    } 

    id = fork();
    if (id == 0) {
        close(0);
        dup(filedes[0]);
        close(filedes[1]);

            if (execvp(argv1[0], argv1) < 0) {
                printf("Invalid command: %s.\n", argv1[0]);
                exit(1);
            }
        } 

        close(filedes[0]);
        close(filedes[1]);
        id = wait(NULL);
        id = wait(NULL);
}

/* Takes in an array of arguments and a specified file descriptor fd. 
 * The stdout file descriptor is updated to fd, and executed.
 */
void execute_redirect(char* argv[], int fd) {
    pid_t id;

    id = fork();

    if (id == 0) {

          close(1);
          dup(fd);
          close(fd);

          if (execvp(argv[0], argv) < 0) {
              printf("Invalid command: %s.\n", argv[0]);
              exit(1);
          }
    }
    else {
          close(fd);
          id = wait(NULL);
    }
}

/* Takes arguments from an array, forks a child process and then executes */
void execute_program(char* argv[]) {
    pid_t id;

    id = fork();

    if (id == 0) {
          if (execvp(argv[0], argv) < 0) {
              printf("%s: command not found.\n", argv[0]);
              exit(1);
          }
    }
    else {
          id = wait(NULL);
    }
}

/* Takes in an array of arguments, argv, and the count of arguments, argc.
 * Based on the first argument in argv, determines which command to execute.
 */
void execute_command(int argc, char* argv[]) {
    int fd;
    int i = 0;
    int j = 0;
    char* argv1[MAX_ARGS];

    if (strcmp(argv[0], "exit") == 0) {
          printf("Exiting USFshell.\n");
          exit(0);
    } 
    else if (strcmp(argv[0], "cd") == 0) {
          chdir(argv[1]);
    } 
    else {
          for (i = 0; i < argc; i++) {
              if (strcmp(argv[i], ">") == 0) {
                    if ((fd = open(argv[i + 1], O_CREAT | O_WRONLY, 0644)) < 0) {
                        printf("No output file!\n");
                        return;
                    }
                    argv[i] = NULL;
                    execute_redirect(argv, fd);
                    return;

              }
              else if (strcmp(argv[i], "|") == 0) {
                    argv[i] = NULL;
                    i++;

                    while (i < argc) {
                        argv1[j] = argv[i];
                        j++;
                        i++;
                    }
                    argv1[j] = NULL;

                    execute_pipe(argv, argv1);
                    return;
              }
          }

          execute_program(argv);
    }
}

/* main function */
int main(void) {
    char buf[MAX_STR_LEN];
    char* argv[MAX_ARGS];
    int argc;

    while (true) {

          clear_arrays(buf, argv);

          print_prompt(buf);

          argc = parse_args(buf, argv);

          execute_command(argc, argv);
    }

    return 0;
}
