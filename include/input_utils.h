#ifndef INPUT_UTILS_H
#define INPUT_UTILS_H

#include "common.h"

// Input functions
void clear_input_buffer(void);
void safe_input(char *buffer, size_t size);
int get_integer_input(const char *prompt, int min, int max);
int get_menu_choice(int min, int max);
void secure_password_input(char *password, int max_length);

// Validation functions
bool validate_email(const char *email);
bool validate_phone(const char *phone);

// Terminal functions
void print_header(void);
void print_footer(void);
void print_separator(char ch);
void set_color(int color);

// Cross-platform input functions
int getch(void);
int getche(void);

#endif // INPUT_UTILS_H
