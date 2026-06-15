#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 512

void execute_user_input(char command[LIMIT]);
char * delimit(char command[LIMIT], char * delimiter);

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
            while ((c = getchar()) != '\n' && (c != EOF));
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
            // Print invalid command format "<command>: not found"
            printf("%s: not found\n", command + 5);
        }
    } else {
        // Print invalid command format "<command>: command not found"
        printf("%s: command not found\n", command);
    }
}


char * delimit(char command[LIMIT], char * delimiter) {

    return NULL;
}

/*
 * Notes
 *
* // Source - https://stackoverflow.com/a/75913252
* // Posted by Lover of Structure, modified by community. See post 'Timeline' for change history
* // Retrieved 2026-06-14, License - CC BY-SA 4.0

*/