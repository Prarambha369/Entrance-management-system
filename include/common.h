#ifndef COMMON_H
#define COMMON_H
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// Platform-specific includes
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <conio.h>
    #include <direct.h>
    #define CLEAR_SCREEN system("cls")
    #define MKDIR(dir) _mkdir(dir)
    #define FILE_EXISTS(path) (_access(path, 0) != -1)
#else
    #include <unistd.h>
    #include <termios.h>
    #include <sys/stat.h>
    #include <dirent.h>
    #define CLEAR_SCREEN system("clear")
    #define MKDIR(dir) mkdir(dir, 0777)
    #define FILE_EXISTS(path) (access(path, F_OK) != -1)
#endif

// Constants
#define MAX_NAME 100
#define MAX_EMAIL 100
#define MAX_PHONE 20
#define MAX_ADDRESS 200
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_QR_DATA 100
#define MAX_QUESTION 500
#define MAX_OPTION 100
#define MAX_OPTIONS 5
#define MAX_QUESTIONS 100
#define MAX_USERS 1000
#define MAX_STUDENTS 10000
#define VERSION "1.0.0"

// File paths
#define CONFIG_FILE "data/config.dat"
#define USER_FILE "data/users.dat"
#define STUDENT_FILE "data/students.dat"
#define EXAM_FILE "data/exam.dat"
#define LOG_FILE "data/system.log"

// Log levels
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3

// User roles
#define ROLE_ADMIN 0
#define ROLE_EXAMINER 1
#define ROLE_USER 2

// Colors
#define COLOR_RESET 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_YELLOW 3
#define COLOR_BLUE 4

// Forward declarations for structs
typedef struct User User;
typedef struct SystemConfig SystemConfig;
typedef struct Student Student;
// Forward declaration for Question is now in exam.h

// System configuration structure
struct SystemConfig {
    bool first_run;
    char system_name[100];
    char institution_name[100];
    char institution_address[200];
    char contact_email[100];
    char contact_phone[20];
    time_t setup_date;
};

// User structure
struct User {
    int ID;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    int role;
    bool active;
    time_t last_login;
    time_t created_at;
};

// Global variables
#ifdef MAIN_FILE
User current_user = {0};
bool is_logged_in = false;
#else
extern User current_user;
extern bool is_logged_in;
#endif

// Helper functions
void print_char(char ch, int n);
void print_header();
void print_footer();
void print_separator(char ch);
void display_datetime();
void display_about();
void display_help();
void ensure_dir_exists(const char *dir);
char* get_role_name(int role);
void set_color(int color);
void clear_input_buffer();
void safe_input(char *buffer, size_t size);
int get_integer_input(const char *prompt, int min, int max);
int get_menu_choice(int min, int max);
void secure_password_input(char *password, int max_length);
int getch(void);
int getche(void);

// Validation functions
bool validate_email(const char *email);
bool validate_phone(const char *phone);
bool is_numeric(const char *str);

// System functions
void init_log_system();
void log_message(int level, const char *message, ...);
void show_documentation();
void admin_panel();
void user_panel();
FILE* safe_open(const char *filename, const char *mode);
bool authenticate_user(const char *username, const char *password, User *user);
SystemConfig load_system_config();
void save_system_config(SystemConfig config);

#endif // COMMON_H
