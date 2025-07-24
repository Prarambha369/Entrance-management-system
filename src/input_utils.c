#include "../include/common.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Clear the input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Safe string input with buffer overflow protection
void safe_input(char *buffer, size_t size) {
    if (!buffer || size < 1) return;
    
    if (fgets(buffer, (int)size, stdin)) {
        // Remove trailing newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        } else {
            // Clear buffer if input was too long
            clear_input_buffer();
        }
    } else {
        // Handle input error
        buffer[0] = '\0';
        clear_input_buffer();
    }
}

// Safe integer input with validation
int get_integer_input(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        if (prompt) printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (sscanf(buffer, "%d", &value) == 1) {
                if (value >= min && value <= max) {
                    return value;
                }
            }
        }
        
        printf("Invalid input. Please enter a number between %d and %d: ", min, max);
    }
}

// Get menu selection with validation
int get_menu_choice(int min, int max) {
    int choice;
    char input[10];
    
    while (1) {
        printf("\n\t\tEnter your choice (%d-%d): ", min, max);
        
        if (fgets(input, sizeof(input), stdin)) {
            // Clear the input buffer if input was too long
            if (!strchr(input, '\n')) {
                clear_input_buffer();
            }
            
            if (sscanf(input, "%d", &choice) == 1) {
                if (choice >= min && choice <= max) {
                    return choice;
                }
            }
        }
        
        printf("\t\tInvalid choice. Please try again.");
    }
}
