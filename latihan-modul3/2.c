// C program to illustrate 
// pipe system call in C 
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <wait.h>
#include <stdlib.h>

int main() 
{ 
    if (chdir("dir") < 0) {
        exit(EXIT_FAILURE);
    }

    int p[2], status1, status2;
    char* argv1[] = {"ls", NULL, NULL};
    char* argv2[] = {"head", "-3", NULL};
    char* argv3[] = {"tail", "-1", NULL};

    if (pipe(p) < 0)
        exit(1);

    if (fork() == 0) {
        dup2(p[1], 1); // redirect stdout to pipe
        execv("/usr/bin/ls", argv1);
    }

    if (fork() == 0){
        dup2(p[0], 0); // get stdin from pipe
        dup2(p[1], 1);
        execv("/usr/bin/head", argv2);
    }

    if (fork() == 0) {
        close(p[1]);
        dup2(p[0], 0);
        execv("/usr/bin/tail", argv3);
    }
    
    close(p[0]);
    close(p[1]);

    wait(NULL); 
    wait(NULL);
    wait(NULL);

    exit(1);
    return 0; 
} 
