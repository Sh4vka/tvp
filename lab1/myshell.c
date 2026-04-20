#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

int split_arguments(char *input, char *args[]) {
    int arg_count = 0;
    char *token;
    
    input[strcspn(input, "\n")] = '\0';
    
    char *ptr = input;
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }
    
    if (*ptr == '\0') {
        return 0;
    }
    
    token = strtok(ptr, " \t");
    
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t");
    }
    
    args[arg_count] = NULL;
    
    return arg_count;
}

int parse_pipe(char *args[], char *left_args[], char *right_args[]) {
    int i;
    int pipe_pos = -1;
    
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            pipe_pos = i;
            break;
        }
    }
    
    if (pipe_pos == -1) {
        return 0;
    }
    
    for (i = 0; i < pipe_pos; i++) {
        left_args[i] = args[i];
    }
    left_args[pipe_pos] = NULL;
    
    for (i = pipe_pos + 1; args[i] != NULL; i++) {
        right_args[i - pipe_pos - 1] = args[i];
    }
    right_args[i - pipe_pos - 1] = NULL;
    
    return 1;
}

int is_exit_command(char *args[]) {
    if (args[0] == NULL) {
        return 0;
    }
    return strcmp(args[0], "exit") == 0;
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    char *left_args[MAX_ARGS];
    char *right_args[MAX_ARGS];
    int arg_count;
    pid_t pid1, pid2;
    int status;
    int pipefd[2];
    
    while (1) {
        printf("mysh> ");
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            break;
        }
        
        arg_count = split_arguments(input, args);
        
        if (arg_count == 0) {
            continue;
        }
        
        if (is_exit_command(args)) {
            break;
        }
        
        if (parse_pipe(args, left_args, right_args)) {
            if (pipe(pipefd) == -1) {
                perror("pipe");
                continue;
            }
            
            pid1 = fork();
            if (pid1 == -1) {
                perror("fork");
                continue;
            }
            
            if (pid1 == 0) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
                execvp(left_args[0], left_args);
                perror("execvp");
                exit(1);
            }
            
            pid2 = fork();
            if (pid2 == -1) {
                perror("fork");
                continue;
            }
            
            if (pid2 == 0) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
                execvp(right_args[0], right_args);
                perror("execvp");
                exit(1);
            }
            
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid1, &status, 0);
            waitpid(pid2, &status, 0);
        } else {
            pid_t pid = fork();
            
            if (pid == -1) {
                perror("fork");
                continue;
            }
            
            if (pid == 0) {
                execvp(args[0], args);
                perror("execvp");
                exit(1);
            } else {
                wait(&status);
            }
        }
    }
    
    return 0;
}
