#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 1024

int main(void) {
    setbuf(stdout, NULL);               // no mem buffer, instant output

    do {
        // Print shell prompt $
        printf("$ ");

        // Handle Invalid commands

        // Read user input
        char command[LIMIT];
        fgets(command, sizeof(command), stdin);

        // Replace \n with \0 fgets output
        command[strcspn(command, "\n")] = '\0';         // Source - https://stackoverflow.com/a/28462221 // Posted by Tim Čas // Retrieved 2026-06-14, License - CC BY-SA 3.0

        // Halt - user input "exit"
        if (strcmp(command, "exit") == 0) {
            break;
        } else {
            // Print invalid command format "<command>: command not found"
            printf("%s: command not found\n", command);
        }

    } while (true);

    return 0;
}




/*
 * Notes
 *
 */