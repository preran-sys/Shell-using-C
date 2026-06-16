#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define LIMIT 512

void execute_user_input(char command[LIMIT]);
char ** delimit(const char * delimiter, char *argv[LIMIT], const char * literal);
bool file_exists(const char *filename);
bool executable(char command[LIMIT]);
void free_tokens(char *arr[]);

int main(void) {
    setbuf(stdout, NULL);               // no mem buffer, instant output

    do {
        // Print shell prompt $
        printf("$ ");

        // Handle Invalid commands

        // Read user input
        char command[LIMIT]; // CAUTION: LIMIT EXCEED would lead to unintended behavior, fix this

        // failure check read user input
        if (fgets(command, sizeof(command), stdin) == NULL) {       // reads input using if block
            if (feof(stdin)) {
                printf("EOF reached\n");
                break;
            }
            if (ferror(stdin)) {
                perror("stdin error");
                break;
            }
            continue;
        }

        const size_t len = strlen(command);

        if (len > 0 && command[len - 1] == '\n') {
            // Replace trailing new line char with null space char
            command[strcspn(command, "\n")] = '\0';         // Source - https://stackoverflow.com/a/28462221 // Posted by Tim Čas // Retrieved 2026-06-14, License - CC BY-SA 3.0

        } else {
            // command too long than allowed size, safely discard the rest of the stream
            int c = 0;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("command too long (max %d chars)\n", LIMIT - 1);
            continue;
        }


        // Halt - user input "exit"
        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Input nothing
        if (strcmp(command, "") == 0) {
            continue;
        }

        execute_user_input(command);

    } while (true);

    return 0;
}

void execute_user_input(char command[LIMIT]) {
    // Implement echo

    // Verify echo to be the command argument 0
    if (strcmp(command, "echo") == 0) {
        printf("");
    }
    if (strncmp(command, "echo ", 5) == 0) {
        printf("%s\n", command + 5);
    }
    // Implement type
    // Verify type to be the command argument 0
    if (strcmp(command, "type") == 0) {
        printf("");
    }
    if (strncmp(command, "type ", 5) == 0) {
        // Validate builtin shell commands to print "<command> is a shell builtin"
        if (strcmp(command + 5, "echo") == 0) {
            printf("%s is a shell builtin\n", command + 5);
        } else if (strcmp(command + 5, "exit") == 0) {
            printf("%s is a shell builtin\n", command + 5);
        } else if (strcmp(command + 5, "type") == 0) {
            printf("%s is a shell builtin\n", command + 5);
        } else {
            // T1: Iterate through each directory in PATH
            // T2: Verify file with (command + 5) name exists
            // T3: Verify executable permission
            // If T2 and T3 true together, print "<command> is <full_path>", stop
            // If T2 true and T3 false, move to next directory in T1
            char *argv[LIMIT];
            const char * environment = getenv("PATH");          // NUL-terminated string // should not modify the string pointed to by the getenv() function
            if (environment != NULL) {
                char ** path_directories = delimit(":", argv, environment);
                while (*path_directories != NULL) {
                    char file[1024];
                    snprintf(file, sizeof(file), "%s/%s" ,*path_directories, command + 5);
                    if (file_exists(file)) {
                        if (access(file, X_OK) == 0) {
                            printf("%s is %s\n", command+5, file);
                            break;
                        }
                        path_directories++;
                    } else {
                        path_directories++;
                    }
            }

                // Clean up individual tokens
                for (int i = 0; argv[i] != NULL; i++) {
                    free(argv[i]);
                }
            }
        }
    } else {
        if (executable(command)) {
            //
        } else {
            // Print invalid command format "<command>: command not found"
            printf("%s: command not found\n", command);
        }
    }
}

// Verify provided shell command is executable and execute
bool executable(char command[LIMIT]) {
    // T1: Delimit env path
    char * argv[LIMIT];
    const char * environment = getenv("PATH");
    char ** path_directories = delimit(":", argv, environment);
    // T2: Iterate through each path directory

    char * av[LIMIT];
    delimit(" ", av, command);
    char * arg0 = av[0];
    while (*path_directories != NULL) {
        char file[1024];
        snprintf(file, sizeof(file), "%s/%s", *path_directories, arg0);
        // T3: Verify file with argv[0] name exists
        if (file_exists(file)) {
            // T4: Verify executable permission
            if (access(file, X_OK) == 0) {
                // printf("%s is %s\n", arg0, file);

                pid_t pid = fork();// pid_t is the datatype used to store process IDs.

                if (pid < 0) {
                    // fork failed
                    printf("fork failed!\n");
                }
                else if (pid == 0) {
                    // Child process
                    execv(file, av);

                    // If we reach this line, execv failed!
                    perror("execv failed");

                    // Terminate the child process immediately so it does not return to the shell loop
                    exit(1);
                }
                else {
                    // Parent process
                    // printf("Parent: waiting for child...\n");
                    wait(NULL);
                    // printf("Parent: child finished.\n");
                }

                // T5: If T2 and T3 true together, true lets run execute(command), stop
                free_tokens(argv);
                free_tokens(av);
                return true;
            }
            // T6: If T2 true and T3 false, move to next directory in T1
            path_directories++;
        } else {
            path_directories++;
        }
    }
    // Clean up individual tokens
    free_tokens(argv);
    free_tokens(av);
    return false;
}

// Helper to free a null-terminated array of strings
void free_tokens(char *arr[]) {
    for (int i = 0; arr[i] != NULL; i++) {
        free(arr[i]);
    }
}


// return true if the file specified
// by the filename exists
bool file_exists(const char *filename)
{
    return access(filename, F_OK) == 0;
}

// Delimit env path
// T1: Retrieve environment variable
// T2: Separate T1 string output whenever ":" (abstractable)
// T3: Store each substring separated from T2 in an array
// T4: Return the data for further filtering
char ** delimit(const char * delimiter, char *argv[LIMIT], const char * literal) {
    char *string = strdup(literal);
    char *tofree = string;
    char **ap = argv;
    char *token;

    while ((token = strsep(&string, delimiter)) != NULL) {
        if (*token != '\0') {
            *ap = strdup(token); // Duplicate the individual token
            if (++ap >= &argv[LIMIT]) break;
        }
    }
    *ap = NULL;

    free(tofree); // Now safe to free the temporary environment string copy

    return argv;
}

/*
 * Notes
 *
* // Source - https://stackoverflow.com/a/75913252
* // Posted by Lover of Structure, modified by community. See post 'Timeline' for change history
* // Retrieved 2026-06-14, License - CC BY-SA 4.0

*/