#include "systemcalls.h"
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);

    char *command[count + 1];
    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return false;
    } else if (pid == 0) {
        // Child process
        execv(command[0], command);
        perror("Execv failed"); // If execv fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return true; // Success
        } else {
            return false; // Failure
        }
    }

    va_end(args);
}
