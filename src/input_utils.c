#include "../include/input_utils.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Clear input buffer
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Safe string input
void safe_input(char *buffer, size_t size) {
    if (!buffer || size == 0) return;

    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        } else {
            clear_input_buffer();
        }
    } else {
        clear_input_buffer();
        buffer[0] = '\0';
    }
}

// Get integer within range
int get_integer_input(const char *prompt, int min, int max) {
    int value;
    char input[32];

    do {
        printf("%s", prompt);
        safe_input(input, sizeof(input));

        if (sscanf(input, "%d", &value) != 1) {
            printf("Please enter a valid number.\n");
            continue;
        }

        if (value < min || value > max) {
            printf("Please enter a number between %d and %d.\n", min, max);
            continue;
        }

        break;
    } while (1);

    return value;
}

// Get menu choice
int get_menu_choice(int min, int max) {
    char input[32];
    int choice;

    printf("\n\n\t\tEnter your choice (%d-%d): ", min, max);
    safe_input(input, sizeof(input));

    if (sscanf(input, "%d", &choice) != 1) {
        return min - 1;  // Invalid input
    }

    return choice;
}

// Secure password input (hides input)
void secure_password_input(char *password, int max_length) {
    int i = 0;
    char ch;

    while (i < max_length - 1) {
        ch = getch();

        if (ch == '\n' || ch == '\r') {
            break;
        }

        if (ch == '\b' && i > 0) {  // Backspace
            printf("\b \b");
            i--;
            continue;
        }

        if (isprint(ch)) {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0';
    printf("\n");
}

// Cross-platform terminal functions
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>

    int getch(void) {
        return _getch();
    }

    int getche(void) {
        return _getche();
    }

    void set_color(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
#else
    #include <termios.h>
    #include <unistd.h>

    int getch(void) {
        struct termios oldattr, newattr;
        int ch;

        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }

    int getche(void) {
        struct termios oldattr, newattr;
        int ch;

        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }

    void set_color(int color) {
        switch (color) {
            case COLOR_RED:
                printf("\033[31m");
                break;
            case COLOR_GREEN:
                printf("\033[32m");
                break;
            case COLOR_YELLOW:
                printf("\033[33m");
                break;
            case COLOR_BLUE:
                printf("\033[34m");
                break;
            default:
                printf("\033[0m");
                break;
        }
    }
#endif

// Print functions
void print_header(void) {
    CLEAR_SCREEN;
    printf("\n\t\t===========================================");
    printf("\n\t\t       ENTRANCE MANAGEMENT SYSTEM");
    printf("\n\t\t===========================================\n");
}

void print_footer(void) {
    printf("\n\t\t===========================================\n");
}

void print_separator(char ch) {
    printf("\n\t\t");
    for (int i = 0; i < 43; i++) printf("%c", ch);
    printf("\n");
}

// Email validation
bool validate_email(const char *email) {
    if (!email || strlen(email) < 5) return false;

    // Must contain exactly one @
    const char *at = strchr(email, '@');
    if (!at || strchr(at + 1, '@')) return false;

    // Must have characters before @
    if (at == email) return false;

    // Must have domain after @ with at least one dot
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1) return false;

    // Must have characters after last dot
    if (strlen(dot) < 2) return false;

    // Check for valid characters
    for (const char *c = email; *c; c++) {
        if (!isalnum(*c) && *c != '@' && *c != '.' && *c != '_' && *c != '-') {
            return false;
        }
    }

    return true;
}

// Phone validation
bool validate_phone(const char *phone) {
    if (!phone || strlen(phone) < 10) return false;

    int digits = 0;
    for (const char *c = phone; *c; c++) {
        if (isdigit(*c)) {
            digits++;
        } else if (*c != '+' && *c != '-' && *c != ' ' && *c != '(' && *c != ')') {
            return false;
        }
    }

    // Must have at least 10 digits
    return digits >= 10;
}
