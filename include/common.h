#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

// Platform-specific includes and definitions
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #define CLEAR_SCREEN system("cls")
    #define MKDIR(dir) _mkdir(dir)
    #define FILE_EXISTS(file) (_access(file, 0) != -1)
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #define CLEAR_SCREEN system("clear")
    #define MKDIR(dir) mkdir(dir, 0777)
    #define FILE_EXISTS(file) (access(file, F_OK) != -1)
    // Function prototypes for Unix
    int getch(void);
    int getche(void);
#endif

// File paths
#define DATA_FILE "data/student.dat"
#define USER_FILE "data/users.dat"
#define LOG_FILE "data/system.log"
#define CONFIG_FILE "data/config.dat"

// Constants
#define MAX_NAME 100
#define MAX_DOB 20
#define MAX_ADDRESS 50
#define MAX_PHONE 20
#define MAX_EMAIL 100
#define MAX_QR_DATA 256
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define EXAM_QUESTIONS 10
#define VERSION "3.0"
#define MAX_LOG_MSG 200

// Log levels
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3

// User roles
#define ROLE_USER 0
#define ROLE_ADMIN 1
#define ROLE_EXAMINER 2

// ANSI Color codes for cross-platform
enum {
    COLOR_RESET,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE
};

// Function prototypes from utils.c
void set_color(int color);
void print_char(char ch, int n);
void print_header();
void print_footer();
void print_separator(char ch);
void safe_input(char *buffer, size_t size);
void secure_password_input(char *password, int max_length);
bool validate_email(const char *email);
bool validate_phone(const char *phone);
bool is_numeric(const char *str);
void ensure_dir_exists(const char *dir);
FILE* safe_open(const char *filename, const char *mode);
char* get_role_name(int role);
void display_datetime();

#endif // COMMON_H
