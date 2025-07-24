#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>

/*==============================
 * CROSS-PLATFORM COMPATIBILITY
 *==============================*/
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #define CLEAR_SCREEN system("cls")
    #define DATA_FILE "data/student.dat"
    #define USER_FILE "data/users.dat"
    #define LOG_FILE "data/system.log"
    #define CONFIG_FILE "data/config.dat"
    #define MKDIR(dir) _mkdir(dir)
    #define FILE_EXISTS(file) (_access(file, 0) != -1)

    // Define colors for Windows
    #define COLOR_RESET 7
    #define COLOR_RED 12
    #define COLOR_GREEN 10
    #define COLOR_BLUE 9
    #define COLOR_YELLOW 14

    void set_color(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
#else
    #include <stdio.h>
    #include <termios.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #define CLEAR_SCREEN system("clear")
    #define DATA_FILE "data/student.dat"
    #define USER_FILE "data/users.dat"
    #define LOG_FILE "data/system.log"
    #define CONFIG_FILE "data/config.dat"
    #define MKDIR(dir) mkdir(dir, 0777)
    #define FILE_EXISTS(file) (access(file, F_OK) != -1)

    // Define ANSI color codes
    #define COLOR_RESET 0
    #define COLOR_RED 1
    #define COLOR_GREEN 2
    #define COLOR_BLUE 4
    #define COLOR_YELLOW 3

    void set_color(int color) {
        const char *colors[] = {
            "\033[0m",     // Reset
            "\033[1;31m",  // Red
            "\033[1;32m",  // Green
            "\033[1;33m",  // Yellow
            "\033[1;34m"   // Blue
        };
        printf("%s", colors[color % 5]);
    }

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
        newattr.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif

/*==============================
 * CONSTANTS AND DEFINITIONS
 *==============================*/
#define MAX_NAME 100
#define MAX_DOB 20
#define MAX_ADDRESS 50
#define MAX_PHONE 20
#define MAX_EMAIL 100
#define MAX_QR_DATA 256
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define EXAM_TIME_MINUTES 20
#define EXAM_QUESTIONS 10
#define VERSION "3.0"
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARNING 2
#define LOG_ERROR 3

// User roles
#define ROLE_USER 0
#define ROLE_ADMIN 1
#define ROLE_EXAMINER 2

// System configuration
typedef struct {
    bool first_run;
    char system_name[100];
    char institution_name[100];
    char institution_address[100];
    char contact_email[100];
    char contact_phone[50];
    time_t setup_date;
} SystemConfig;

// User account structure
typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char full_name[MAX_NAME];
    char email[MAX_EMAIL];
    int role; // 0=student, 1=admin, 2=examiner
    int student_id; // If role=0, links to a student record
    time_t created_date;
    time_t last_login;
    bool active;
} User;

// Current logged in user
User current_user;
bool is_logged_in = false;

// Student structure with improved naming
typedef struct Student {
    char name[MAX_NAME];
    char dob[MAX_DOB];
    char address[MAX_ADDRESS];
    int ID;
    float mark;
    int has_taken_exam; // Changed from n to a more descriptive name
    char phone[MAX_PHONE];
    char guardian_name[MAX_NAME];
    char email[MAX_EMAIL];
    char guardian_phone[MAX_PHONE];
    time_t registration_date; // Track when student registered
    char qr_code[MAX_QR_DATA]; // Store QR code data for student
    bool has_user_account; // Whether the student has a user account
} Student;

// Question structure for exams
typedef struct Question {
    char question_text[500];
    char options[4][100];
    char correct_answer; // 'A', 'B', 'C', or 'D'
    int difficulty;      // 1-5
    char category[50];   // e.g., "Math", "Physics", etc.
} Question;

/*==============================
 * FUNCTION PROTOTYPES
 *==============================*/
// System initialization
void initialize_system();
void create_default_admin();
bool check_first_run();
void save_system_config(SystemConfig config);
SystemConfig load_system_config();
void show_documentation();

// User Authentication
bool login();
void logout();
void create_user_account(int role);
bool authenticate_user(const char *username, const char *password, User *user);
void change_password();
void user_management();

// Admin Panel
void admin_panel();
void admin_student_management();
void admin_user_management();
void admin_system_settings();
void admin_reports();

// User Panel
void user_panel();
void user_profile();
void user_exam();
void user_results();

// Core functionality
void add_student(FILE *fp);
void conduct_exam(FILE *fp);
void modify_student(FILE *fp);
void display_all_students(FILE *fp);
void search_student(FILE *fp);
void rank_students(FILE *fp);

// Backup and Report Functions
void backup_menu();
void create_binary_backup();
void export_csv_backup();
void restore_backup();
void generate_report();

// QR Code functionality
void generate_qr_code(Student *s);
void display_qr_code(const char *qr_data);
void check_in_with_qr(FILE *fp);

// Logging System
void init_log_system();
void log_message(int level, const char *message);
void view_logs();

// Helper functions
void print_char(char ch, int n);
void print_header();
void print_footer();
void print_separator(char ch);
void safe_input(char *buffer, int size);
FILE* safe_open(const char *filename, const char *mode);
bool validate_email(const char *email);
bool validate_phone(const char *phone);
bool is_numeric(const char *str);
void display_datetime();
void display_about();
void display_help();
void ensure_dir_exists(const char *dir);
void secure_password_input(char *password, int max_length);
char* get_role_name(int role);

// Exam system functions
void load_questions(Question questions[]);
void shuffle_questions(int order[], int count);

/*==============================
 * MAIN FUNCTION
 *==============================*/
int main() {
    // Initialize system
    initialize_system();

    // If first run, show documentation
    if (check_first_run()) {
        show_documentation();
    }

    // Main program loop
    while (1) {
        // Try to login
        if (!is_logged_in) {
            if (!login()) {
                // Exit if user cancels login
                log_message(LOG_INFO, "User canceled login. Exiting program.");
                return 0;
            }
        }

        // Display appropriate panel based on user role
        if (current_user.role == ROLE_ADMIN) {
            admin_panel();
        } else {
            user_panel();
        }
    }

    return 0;
}

/*==============================
 * SYSTEM INITIALIZATION
 *==============================*/

void initialize_system() {
    // Create necessary directories
    ensure_dir_exists("data");
    ensure_dir_exists("backups");
    ensure_dir_exists("reports");

    // Initialize logging
    init_log_system();
    log_message(LOG_INFO, "System initializing");

    // Check if this is first run
    if (check_first_run()) {
        log_message(LOG_INFO, "First run detected. Setting up system.");

        // Create default admin account if this is first run
        create_default_admin();

        // Create system configuration
        SystemConfig config;
        config.first_run = false;
        strcpy(config.system_name, "Entrance Management System");
        strcpy(config.institution_name, "Demo Institution");
        strcpy(config.institution_address, "123 Education St.");
        strcpy(config.contact_email, "admin@example.com");
        strcpy(config.contact_phone, "123-456-7890");
        config.setup_date = time(NULL);

        save_system_config(config);
    }
}

bool check_first_run() {
    // Check if configuration file exists
    if (!FILE_EXISTS(CONFIG_FILE)) {
        return true;
    }

    // Check if user file exists
    if (!FILE_EXISTS(USER_FILE)) {
        return true;
    }

    // Load config and check first_run flag
    SystemConfig config = load_system_config();
    return config.first_run;
}

void create_default_admin() {
    FILE *user_fp = safe_open(USER_FILE, "wb");
    if (!user_fp) {
        log_message(LOG_ERROR, "Failed to create user file for default admin");
        return;
    }

    User admin;
    strcpy(admin.username, "admin");
    strcpy(admin.password, "admin123"); // In production, hash this password
    strcpy(admin.full_name, "System Administrator");
    strcpy(admin.email, "admin@example.com");
    admin.role = ROLE_ADMIN;
    admin.student_id = 0;
    admin.created_date = time(NULL);
    admin.last_login = 0;
    admin.active = true;

    fwrite(&admin, sizeof(User), 1, user_fp);
    fclose(user_fp);

    log_message(LOG_INFO, "Default admin account created");
}

void save_system_config(SystemConfig config) {
    FILE *config_fp = safe_open(CONFIG_FILE, "wb");
    if (!config_fp) {
        log_message(LOG_ERROR, "Failed to save system configuration");
        return;
    }

    fwrite(&config, sizeof(SystemConfig), 1, config_fp);
    fclose(config_fp);

    log_message(LOG_INFO, "System configuration saved");
}

SystemConfig load_system_config() {
    SystemConfig config;
    memset(&config, 0, sizeof(SystemConfig));

    FILE *config_fp = safe_open(CONFIG_FILE, "rb");
    if (!config_fp) {
        log_message(LOG_ERROR, "Failed to load system configuration");
        config.first_run = true;
        return config;
    }

    fread(&config, sizeof(SystemConfig), 1, config_fp);
    fclose(config_fp);

    return config;
}

void show_documentation() {
    print_header();

    set_color(COLOR_GREEN);
    printf("\n\n\t\tWELCOME TO ENTRANCE MANAGEMENT SYSTEM v%s", VERSION);
    set_color(COLOR_RESET);
    printf("\n\t\t----------------------------------------\n");

    printf("\n\t\tThis documentation will help you get started with the system.\n");

    printf("\n\n\t");
    set_color(COLOR_BLUE);
    printf("SYSTEM OVERVIEW");
    set_color(COLOR_RESET);
    printf("\n\t---------------\n");
    printf("\n\tThe Entrance Management System (EMS) is designed to help educational institutions");
    printf("\n\tmanage their entrance examination process from registration to results.");

    printf("\n\n\t");
    set_color(COLOR_BLUE);
    printf("KEY FEATURES");
    set_color(COLOR_RESET);
    printf("\n\t------------\n");
    printf("\n\t1. User Management - Different access levels for students, admins and examiners");
    printf("\n\t2. Student Registration - Register students for entrance exams");
    printf("\n\t3. Examination System - Conduct online entrance tests");
    printf("\n\t4. QR Code Integration - Check-in students using QR codes");
    printf("\n\t5. Reporting - Generate various reports and rankings");
    printf("\n\t6. Backup System - Backup data in binary and CSV formats");

    printf("\n\n\t");
    set_color(COLOR_BLUE);
    printf("GETTING STARTED");
    set_color(COLOR_RESET);
    printf("\n\t---------------\n");
    printf("\n\t1. Default admin account has been created for you:");
    printf("\n\t   Username: admin");
    printf("\n\t   Password: admin123");
    printf("\n\n\t2. Please change the default password immediately after first login");
    printf("\n\n\t3. Create additional user accounts as needed");
    printf("\n\n\t4. Set up your institution details in System Settings");

    printf("\n\n\t");
    set_color(COLOR_BLUE);
    printf("USER TYPES");
    set_color(COLOR_RESET);
    printf("\n\t----------\n");
    printf("\n\t1. Admin - Full access to all features");
    printf("\n\t2. Examiner - Can manage exams and view results");
    printf("\n\t3. Student - Can take exams and view their own results");

    printf("\n\n\t");
    set_color(COLOR_YELLOW);
    printf("IMPORTANT: For security, change the default admin password immediately.");
    set_color(COLOR_RESET);

    printf("\n\n\n\tPress any key to continue to login screen...");
    getch();
}

/*==============================
 * USER AUTHENTICATION
 *==============================*/

bool login() {
    print_header();

    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int attempts = 0;
    const int max_attempts = 3;

    while (attempts < max_attempts) {
        print_header();

        set_color(COLOR_BLUE);
        printf("\n\n\t\tENTRANCE MANAGEMENT SYSTEM LOGIN");
        set_color(COLOR_RESET);
        printf("\n\t\t--------------------------------\n");

        if (attempts > 0) {
            set_color(COLOR_RED);
            printf("\n\t\tInvalid username or password. Attempt %d of %d.",
                   attempts + 1, max_attempts);
            set_color(COLOR_RESET);
        }

        printf("\n\n\t\tUsername: ");
        fflush(stdin);
        safe_input(username, MAX_USERNAME);

        if (strcmp(username, "exit") == 0 || strcmp(username, "quit") == 0) {
            return false; // User wants to exit
        }

        printf("\t\tPassword: ");
        secure_password_input(password, MAX_PASSWORD);

        // Check credentials
        if (authenticate_user(username, password, &current_user)) {
            // Update last login time
            FILE *user_fp = safe_open(USER_FILE, "r+b");
            if (user_fp) {
                User temp;
                while (fread(&temp, sizeof(User), 1, user_fp) == 1) {
                    if (strcmp(temp.username, username) == 0) {
                        fseek(user_fp, -sizeof(User), SEEK_CUR);
                        temp.last_login = time(NULL);
                        fwrite(&temp, sizeof(User), 1, user_fp);
                        break;
                    }
                }
                fclose(user_fp);
            }

            is_logged_in = true;

            char log_msg[100];
            sprintf(log_msg, "User logged in: %s (Role: %s)",
                    username, get_role_name(current_user.role));
            log_message(LOG_INFO, log_msg);

            return true;
        }

        attempts++;
    }

    log_message(LOG_WARNING, "Maximum login attempts exceeded");
    set_color(COLOR_RED);
    printf("\n\n\t\tMaximum login attempts exceeded.");
    printf("\n\t\tPlease try again later.");
    set_color(COLOR_RESET);
    printf("\n\n\t\tPress any key to exit...");
    getch();

    return false;
}

bool authenticate_user(const char *username, const char *password, User *user) {
    FILE *user_fp = safe_open(USER_FILE, "rb");
    if (!user_fp) {
        return false;
    }

    User temp;
    bool found = false;

    while (fread(&temp, sizeof(User), 1, user_fp) == 1) {
        if (strcmp(temp.username, username) == 0 &&
            strcmp(temp.password, password) == 0 &&
            temp.active) {

            *user = temp;
            found = true;
            break;
        }
    }

    fclose(user_fp);
    return found;
}

void logout() {
    char log_msg[100];
    sprintf(log_msg, "User logged out: %s", current_user.username);
    log_message(LOG_INFO, log_msg);

    is_logged_in = false;
    memset(&current_user, 0, sizeof(User));

    print_header();
    printf("\n\n\t\tYou have been logged out successfully.");
    printf("\n\n\t\tPress any key to continue...");
    getch();
}

void secure_password_input(char *password, int max_length) {
    int i = 0;
    char ch;

    while (1) {
        ch = getch();

        if (ch == '\r' || ch == '\n') {
            password[i] = '\0';
            printf("\n");
            break;
        } else if (ch == '\b' && i > 0) { // Handle backspace
            i--;
            printf("\b \b");
        } else if (ch != '\b' && i < max_length - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
}

void change_password() {
    print_header();

    char current_pass[MAX_PASSWORD];
    char new_pass[MAX_PASSWORD];
    char confirm_pass[MAX_PASSWORD];

    set_color(COLOR_BLUE);
    printf("\n\n\t\tCHANGE PASSWORD");
    set_color(COLOR_RESET);
    printf("\n\t\t--------------\n");

    printf("\n\t\tCurrent Password: ");
    secure_password_input(current_pass, MAX_PASSWORD);

    // Verify current password
    if (strcmp(current_pass, current_user.password) != 0) {
        set_color(COLOR_RED);
        printf("\n\t\tIncorrect current password!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    printf("\n\t\tNew Password: ");
    secure_password_input(new_pass, MAX_PASSWORD);

    printf("\n\t\tConfirm New Password: ");
    secure_password_input(confirm_pass, MAX_PASSWORD);

    // Check if passwords match
    if (strcmp(new_pass, confirm_pass) != 0) {
        set_color(COLOR_RED);
        printf("\n\t\tPasswords do not match!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Check password strength (simple check)
    if (strlen(new_pass) < 6) {
        set_color(COLOR_RED);
        printf("\n\t\tPassword must be at least 6 characters!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Update password in file
    FILE *user_fp = safe_open(USER_FILE, "r+b");
    if (!user_fp) {
        set_color(COLOR_RED);
        printf("\n\t\tError opening user file!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    User temp;
    while (fread(&temp, sizeof(User), 1, user_fp) == 1) {
        if (strcmp(temp.username, current_user.username) == 0) {
            fseek(user_fp, -sizeof(User), SEEK_CUR);
            strcpy(temp.password, new_pass);
            fwrite(&temp, sizeof(User), 1, user_fp);

            // Update current user
            strcpy(current_user.password, new_pass);
            break;
        }
    }

    fclose(user_fp);

    log_message(LOG_INFO, "User changed password");

    set_color(COLOR_GREEN);
    printf("\n\n\t\tPassword changed successfully!");
    set_color(COLOR_RESET);
    printf("\n\t\tPress any key to return...");
    getch();
}

void create_user_account(int role) {
    print_header();

    User new_user;
    memset(&new_user, 0, sizeof(User));

    set_color(COLOR_BLUE);
    printf("\n\n\t\tCREATE NEW %s ACCOUNT", get_role_name(role));
    set_color(COLOR_RESET);
    printf("\n\t\t-------------------\n");

    // Get user details
    printf("\n\t\tUsername: ");
    fflush(stdin);
    safe_input(new_user.username, MAX_USERNAME);

    // Check if username exists
    FILE *check_fp = safe_open(USER_FILE, "rb");
    if (check_fp) {
        User temp;
        while (fread(&temp, sizeof(User), 1, check_fp) == 1) {
            if (strcmp(temp.username, new_user.username) == 0) {
                set_color(COLOR_RED);
                printf("\n\t\tUsername already exists!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to return...");
                fclose(check_fp);
                getch();
                return;
            }
        }
        fclose(check_fp);
    }

    printf("\n\t\tPassword: ");
    secure_password_input(new_user.password, MAX_PASSWORD);

    printf("\n\t\tFull Name: ");
    fflush(stdin);
    safe_input(new_user.full_name, MAX_NAME);

    do {
        printf("\n\t\tEmail: ");
        fflush(stdin);
        safe_input(new_user.email, MAX_EMAIL);

        if (!validate_email(new_user.email)) {
            set_color(COLOR_RED);
            printf("\n\t\tInvalid email format! Try again.");
            set_color(COLOR_RESET);
        }
    } while (!validate_email(new_user.email));

    // If creating a student account, ask for student ID
    if (role == ROLE_USER) {
        printf("\n\t\tStudent ID: ");
        scanf("%d", &new_user.student_id);

        // Verify student ID exists in student records
        FILE *student_fp = safe_open(DATA_FILE, "rb");
        bool student_found = false;

        if (student_fp) {
            Student s;
            while (fread(&s, sizeof(Student), 1, student_fp) == 1) {
                if (s.ID == new_user.student_id) {
                    student_found = true;

                    // Update student record to indicate they have an account
                    FILE *update_fp = safe_open(DATA_FILE, "r+b");
                    if (update_fp) {
                        Student update_s;
                        while (fread(&update_s, sizeof(Student), 1, update_fp) == 1) {
                            if (update_s.ID == new_user.student_id) {
                                fseek(update_fp, -sizeof(Student), SEEK_CUR);
                                update_s.has_user_account = true;
                                fwrite(&update_s, sizeof(Student), 1, update_fp);
                                break;
                            }
                        }
                        fclose(update_fp);
                    }
                    break;
                }
            }
            fclose(student_fp);
        }

        if (!student_found) {
            set_color(COLOR_RED);
            printf("\n\t\tNo student found with this ID!");
            set_color(COLOR_RESET);
            printf("\n\t\tPress any key to return...");
            getch();
            return;
        }
    } else {
        new_user.student_id = 0; // Not a student account
    }

    new_user.role = role;
    new_user.created_date = time(NULL);
    new_user.last_login = 0;
    new_user.active = true;

    // Save to file
    FILE *user_fp = safe_open(USER_FILE, "ab");
    if (!user_fp) {
        set_color(COLOR_RED);
        printf("\n\t\tError creating user account!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    fwrite(&new_user, sizeof(User), 1, user_fp);
    fclose(user_fp);

    char log_msg[100];
    sprintf(log_msg, "New %s account created: %s",
            get_role_name(role), new_user.username);
    log_message(LOG_INFO, log_msg);

    set_color(COLOR_GREEN);
    printf("\n\n\t\tUser account created successfully!");
    set_color(COLOR_RESET);
    printf("\n\t\tPress any key to return...");
    getch();
}

void user_management() {
    int option;

    do {
        print_header();

        set_color(COLOR_BLUE);
        printf("\n\n\t\tUSER MANAGEMENT");
        set_color(COLOR_RESET);
        printf("\n\t\t--------------\n");

        printf("\n\t\t1. List All Users");
        printf("\n\t\t2. Create Admin Account");
        printf("\n\t\t3. Create Examiner Account");
        printf("\n\t\t4. Create Student Account");
        printf("\n\t\t5. Disable/Enable User");
        printf("\n\t\t0. Return to Admin Panel");

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1: {
                print_header();
                printf("\n\t\tUSER LIST\n");
                printf("\n\t%-20s %-30s %-15s %-20s", "Username", "Full Name", "Role", "Last Login");
                printf("\n\t");
                print_char('-', 85);
                printf("\n");

                FILE *user_fp = safe_open(USER_FILE, "rb");
                if (user_fp) {
                    User u;
                    while (fread(&u, sizeof(User), 1, user_fp) == 1) {
                        char last_login[30] = "Never";

                        if (u.last_login > 0) {
                            struct tm *tm_info = localtime(&u.last_login);
                            strftime(last_login, 30, "%Y-%m-%d %H:%M:%S", tm_info);
                        }

                        if (!u.active) {
                            set_color(COLOR_RED); // Highlight disabled users
                        }

                        printf("\t%-20s %-30s %-15s %-20s",
                               u.username, u.full_name, get_role_name(u.role), last_login);

                        if (!u.active) {
                            printf(" [DISABLED]");
                            set_color(COLOR_RESET);
                        }

                        printf("\n");
                    }
                    fclose(user_fp);
                }

                printf("\n\n\t\tPress any key to continue...");
                getch();
                break;
            }

            case 2:
                create_user_account(ROLE_ADMIN);
                break;

            case 3:
                create_user_account(ROLE_EXAMINER);
                break;

            case 4:
                create_user_account(ROLE_USER);
                break;

            case 5: {
                print_header();
                printf("\n\t\tDISABLE/ENABLE USER\n");

                char username[MAX_USERNAME];
                printf("\n\t\tEnter username: ");
                fflush(stdin);
                safe_input(username, MAX_USERNAME);

                // Don't allow disabling your own account
                if (strcmp(username, current_user.username) == 0) {
                    set_color(COLOR_RED);
                    printf("\n\t\tYou cannot disable your own account!");
                    set_color(COLOR_RESET);
                    printf("\n\t\tPress any key to return...");
                    getch();
                    break;
                }

                FILE *user_fp = safe_open(USER_FILE, "r+b");
                bool found = false;

                if (user_fp) {
                    User u;
                    while (fread(&u, sizeof(User), 1, user_fp) == 1) {
                        if (strcmp(u.username, username) == 0) {
                            fseek(user_fp, -sizeof(User), SEEK_CUR);
                            u.active = !u.active;
                            fwrite(&u, sizeof(User), 1, user_fp);

                            found = true;

                            char log_msg[100];
                            sprintf(log_msg, "User %s %s by %s",
                                    username, u.active ? "enabled" : "disabled",
                                    current_user.username);
                            log_message(LOG_INFO, log_msg);

                            set_color(COLOR_GREEN);
                            printf("\n\t\tUser %s %s successfully!",
                                   username, u.active ? "enabled" : "disabled");
                            set_color(COLOR_RESET);
                            break;
                        }
                    }
                    fclose(user_fp);
                }

                if (!found) {
                    set_color(COLOR_RED);
                    printf("\n\t\tUser not found!");
                    set_color(COLOR_RESET);
                }

                printf("\n\t\tPress any key to continue...");
                getch();
                break;
            }

            case 0:
                return;

            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to try again...");
                getch();
        }
    } while (option != 0);
}

/*==============================
 * ADMIN PANEL
 *==============================*/

void admin_panel() {
    int option;

    do {
        print_header();

        set_color(COLOR_GREEN);
        printf("\n\n\t\tADMIN PANEL - %s", current_user.full_name);
        set_color(COLOR_RESET);
        printf("\n\t\t");
        print_char('-', 50);

        printf("\n\n\t\t1. Student Management");
        printf("\n\t\t2. User Management");
        printf("\n\t\t3. System Settings");
        printf("\n\t\t4. Reports & Statistics");
        printf("\n\t\t5. Backup & Restore");
        printf("\n\t\t6. View System Logs");
        printf("\n\t\t7. Change Password");
        printf("\n\t\t8. About System");
        printf("\n\t\t9. Logout");
        printf("\n\t\t0. Exit System");

        printf("\n\n\t\tCurrent Date & Time: ");
        display_datetime();

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                admin_student_management();
                break;

            case 2:
                user_management();
                break;

            case 3:
                admin_system_settings();
                break;

            case 4:
                admin_reports();
                break;

            case 5:
                backup_menu();
                break;

            case 6:
                view_logs();
                break;

            case 7:
                change_password();
                break;

            case 8:
                display_about();
                break;

            case 9:
                logout();
                return;

            case 0:
                log_message(LOG_INFO, "System shutdown initiated by admin");
                printf("\n\n\t\tShutting down system...");
                exit(0);

            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to try again...");
                getch();
        }
    } while (option != 9 && option != 0);
}

void admin_student_management() {
    FILE *fp = safe_open(DATA_FILE, "rb+");
    if (!fp) {
        fp = safe_open(DATA_FILE, "wb+");
        if (!fp) {
            log_message(LOG_ERROR, "Cannot create data file");
            set_color(COLOR_RED);
            printf("\n\n\t\tError: Cannot create data file!");
            set_color(COLOR_RESET);
            printf("\n\t\tPress any key to return...");
            getch();
            return;
        }
    }

    int option;

    do {
        print_header();

        set_color(COLOR_BLUE);
        printf("\n\n\t\tSTUDENT MANAGEMENT");
        set_color(COLOR_RESET);
        printf("\n\t\t------------------\n");

        printf("\n\t\t1. Register New Student");
        printf("\n\t\t2. Modify Student Record");
        printf("\n\t\t3. View All Students");
        printf("\n\t\t4. Search for Student");
        printf("\n\t\t5. Manage Entrance Exam");
        printf("\n\t\t6. View Student Rankings");
        printf("\n\t\t7. QR Code Check-in");
        printf("\n\t\t0. Return to Admin Panel");

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                add_student(fp);
                break;

            case 2:
                modify_student(fp);
                break;

            case 3:
                display_all_students(fp);
                break;

            case 4:
                search_student(fp);
                break;

            case 5:
                conduct_exam(fp);
                break;

            case 6:
                rank_students(fp);
                break;

            case 7:
                check_in_with_qr(fp);
                break;

            case 0:
                fclose(fp);
                return;

            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to try again...");
                getch();
        }
    } while (option != 0);
}

void admin_system_settings() {
    int option;
    SystemConfig config = load_system_config();

    do {
        print_header();

        set_color(COLOR_BLUE);
        printf("\n\n\t\tSYSTEM SETTINGS");
        set_color(COLOR_RESET);
        printf("\n\t\t---------------\n");

        printf("\n\t\tCurrent Settings:");
        printf("\n\t\t----------------");
        printf("\n\t\tSystem Name: %s", config.system_name);
        printf("\n\t\tInstitution: %s", config.institution_name);
        printf("\n\t\tAddress: %s", config.institution_address);
        printf("\n\t\tContact Email: %s", config.contact_email);
        printf("\n\t\tContact Phone: %s", config.contact_phone);

        printf("\n\n\t\tOptions:");
        printf("\n\t\t--------");
        printf("\n\t\t1. Update System Name");
        printf("\n\t\t2. Update Institution Name");
        printf("\n\t\t3. Update Institution Address");
        printf("\n\t\t4. Update Contact Email");
        printf("\n\t\t5. Update Contact Phone");
        printf("\n\t\t6. View Documentation");
        printf("\n\t\t0. Return to Admin Panel");

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        char buffer[100];

        switch (option) {
            case 1:
                printf("\n\t\tEnter new System Name: ");
                fflush(stdin);
                safe_input(config.system_name, 100);
                save_system_config(config);
                break;

            case 2:
                printf("\n\t\tEnter new Institution Name: ");
                fflush(stdin);
                safe_input(config.institution_name, 100);
                save_system_config(config);
                break;

            case 3:
                printf("\n\t\tEnter new Institution Address: ");
                fflush(stdin);
                safe_input(config.institution_address, 100);
                save_system_config(config);
                break;

            case 4:
                printf("\n\t\tEnter new Contact Email: ");
                fflush(stdin);
                safe_input(buffer, 100);
                if (validate_email(buffer)) {
                    strcpy(config.contact_email, buffer);
                    save_system_config(config);
                } else {
                    set_color(COLOR_RED);
                    printf("\n\t\tInvalid email format!");
                    set_color(COLOR_RESET);
                    printf("\n\t\tPress any key to continue...");
                    getch();
                }
                break;

            case 5:
                printf("\n\t\tEnter new Contact Phone: ");
                fflush(stdin);
                safe_input(config.contact_phone, 50);
                save_system_config(config);
                break;

            case 6:
                show_documentation();
                break;

            case 0:
                return;

            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to try again...");
                getch();
        }
    } while (option != 0);
}

void admin_reports() {
    int option;

    do {
        print_header();

        set_color(COLOR_BLUE);
        printf("\n\n\t\tREPORTS & STATISTICS");
        set_color(COLOR_RESET);
        printf("\n\t\t--------------------\n");

        printf("\n\t\t1. Generate Exam Report");
        printf("\n\t\t2. View Student Rankings");
        printf("\n\t\t3. User Activity Report");
        printf("\n\t\t4. System Usage Statistics");
        printf("\n\t\t0. Return to Admin Panel");

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        FILE *fp = safe_open(DATA_FILE, "rb");

        switch (option) {
            case 1:
                if (fp) {
                    generate_report();
                    fclose(fp);
                } else {
                    set_color(COLOR_RED);
                    printf("\n\t\tError opening data file!");
                    set_color(COLOR_RESET);
                    printf("\n\t\tPress any key to continue...");
                    getch();
                }
                break;

            case 2:
                if (fp) {
                    rank_students(fp);
                    fclose(fp);
                } else {
                    set_color(COLOR_RED);
                    printf("\n\t\tError opening data file!");
                    set_color(COLOR_RESET);
                    printf("\n\t\tPress any key to continue...");
                    getch();
                }
                break;

            case 3:
                print_header();
                printf("\n\t\tUSER ACTIVITY REPORT\n");

                printf("\n\t%-20s %-20s %-20s %-20s", "Username", "Role", "Last Login", "Account Status");
                printf("\n\t");
                print_char('-', 80);
                printf("\n");

                FILE *user_fp = safe_open(USER_FILE, "rb");
                if (user_fp) {
                    User u;
                    while (fread(&u, sizeof(User), 1, user_fp) == 1) {
                        char last_login[30] = "Never";

                        if (u.last_login > 0) {
                            struct tm *tm_info = localtime(&u.last_login);
                            strftime(last_login, 30, "%Y-%m-%d %H:%M:%S", tm_info);
                        }

                        printf("\t%-20s %-20s %-20s %-20s",
                               u.username, get_role_name(u.role),
                               last_login, u.active ? "Active" : "Disabled");
                        printf("\n");
                    }
                    fclose(user_fp);
                }

                printf("\n\n\t\tPress any key to continue...");
                getch();
                break;

            case 4: {
                print_header();
                printf("\n\t\tSYSTEM USAGE STATISTICS\n\n");

                SystemConfig config = load_system_config();
                time_t now = time(NULL);
                double days_running = difftime(now, config.setup_date) / (60 * 60 * 24);

                // Count users
                int total_users = 0, admins = 0, examiners = 0, students = 0;
                FILE *user_fp = safe_open(USER_FILE, "rb");
                if (user_fp) {
                    User u;
                    while (fread(&u, sizeof(User), 1, user_fp) == 1) {
                        total_users++;
                        if (u.role == ROLE_ADMIN) admins++;
                        else if (u.role == ROLE_EXAMINER) examiners++;
                        else if (u.role == ROLE_USER) students++;
                    }
                    fclose(user_fp);
                }

                // Count students
                int total_students = 0, taken_exam = 0, passed_exam = 0;
                FILE *student_fp = safe_open(DATA_FILE, "rb");
                if (student_fp) {
                    Student s;
                    while (fread(&s, sizeof(Student), 1, student_fp) == 1) {
                        total_students++;
                        if (s.has_taken_exam) {
                            taken_exam++;
                            if (s.mark >= 3.0) passed_exam++;
                        }
                    }
                    fclose(student_fp);
                }

                printf("\t\tSystem Installation Date: ");
                char setup_date[30];
                struct tm *tm_info = localtime(&config.setup_date);
                strftime(setup_date, 30, "%Y-%m-%d %H:%M:%S", tm_info);
                printf("%s\n", setup_date);

                printf("\t\tSystem Running For: %.1f days\n\n", days_running);

                printf("\t\tUser Statistics:\n");
                printf("\t\t----------------\n");
                printf("\t\tTotal User Accounts: %d\n", total_users);
                printf("\t\t- Admin Accounts: %d\n", admins);
                printf("\t\t- Examiner Accounts: %d\n", examiners);
                printf("\t\t- Student Accounts: %d\n\n", students);

                printf("\t\tStudent Statistics:\n");
                printf("\t\t-------------------\n");
                printf("\t\tTotal Registered Students: %d\n", total_students);
                printf("\t\tStudents Who Took Exam: %d (%.1f%%)\n",
                       taken_exam, total_students > 0 ? (float)taken_exam / total_students * 100 : 0);
                printf("\t\tStudents Who Passed Exam: %d (%.1f%%)\n",
                       passed_exam, taken_exam > 0 ? (float)passed_exam / taken_exam * 100 : 0);

                printf("\n\n\t\tPress any key to continue...");
                getch();
                break;
            }

            case 0:
                if (fp) fclose(fp);
                return;

            default:
                if (fp) fclose(fp);
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to try again...");
                getch();
        }
    } while (option != 0);
}

/*==============================
 * USER PANEL
 *==============================*/

void user_panel() {
    int option;

    do {
        print_header();

        set_color(COLOR_GREEN);
        printf("\n\n\t\tSTUDENT PANEL - %s", current_user.full_name);
        set_color(COLOR_RESET);
        printf("\n\t\t");
        print_char('-', 50);

        printf("\n\n\t\t1. View My Profile");
        printf("\n\t\t2. Take Entrance Exam");
        printf("\n\t\t3. View My Results");
        printf("\n\t\t4. Change Password");
        printf("\n\t\t5. About System");
        printf("\n\t\t6. Help");
        printf("\n\t\t9. Logout");
        printf("\n\t\t0. Exit System");

        printf("\n\n\t\tCurrent Date & Time: ");
        display_datetime();

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                user_profile();
                break;

            case 2:
                user_exam();
                break;

            case 3:
                user_results();
                break;

            case 4:
                change_password();
                break;

            case 5:
                display_about();
                break;

            case 6:
                display_help();
                break;

            case 9:
                logout();
                return;

            case 0:
                log_message(LOG_INFO, "System shutdown initiated by user");
                printf("\n\n\t\tShutting down system...");
                exit(0);

            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to try again...");
                getch();
        }
    } while (option != 9 && option != 0);
}

void user_profile() {
    print_header();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tMY PROFILE");
    set_color(COLOR_RESET);
    printf("\n\t\t----------\n");

    // Find the student record
    FILE *fp = safe_open(DATA_FILE, "rb");
    if (!fp) {
        set_color(COLOR_RED);
        printf("\n\t\tError: Cannot open data file!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    Student s;
    bool found = false;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.ID == current_user.student_id) {
            found = true;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        set_color(COLOR_RED);
        printf("\n\t\tError: Student record not found!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Display student information
    printf("\n\t\tUser Information:");
    printf("\n\t\t----------------");
    printf("\n\t\tUsername: %s", current_user.username);
    printf("\n\t\tFull Name: %s", current_user.full_name);
    printf("\n\t\tEmail: %s", current_user.email);
    printf("\n\t\tRole: %s", get_role_name(current_user.role));

    // Format last login time
    char last_login[30] = "Never";
    if (current_user.last_login > 0) {
        struct tm *tm_info = localtime(&current_user.last_login);
        strftime(last_login, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    }
    printf("\n\t\tLast Login: %s", last_login);

    printf("\n\n\t\tStudent Information:");
    printf("\n\t\t--------------------");
    printf("\n\t\tID: %d", s.ID);
    printf("\n\t\tName: %s", s.name);
    printf("\n\t\tDate of Birth: %s", s.dob);
    printf("\n\t\tAddress: %s", s.address);
    printf("\n\t\tPhone: %s", s.phone);
    printf("\n\t\tEmail: %s", s.email);
    printf("\n\t\tGuardian: %s", s.guardian_name);
    printf("\n\t\tGuardian Phone: %s", s.guardian_phone);

    // Display exam status
    printf("\n\n\t\tExam Status:");
    printf("\n\t\t------------");

    if (s.has_taken_exam) {
        printf("\n\t\tYou have taken the entrance exam.");
        printf("\n\t\tYour Score: %.2f out of 10.0", s.mark);
        printf("\n\t\tResult: %s", s.mark >= 3.0 ? "PASS" : "FAIL");
    } else {
        printf("\n\t\tYou have not yet taken the entrance exam.");
        printf("\n\t\tSelect 'Take Entrance Exam' from the menu to start.");
    }

    printf("\n\n\t\tPress any key to return to menu...");
    getch();
}

void user_exam() {
    print_header();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tENTRANCE EXAM");
    set_color(COLOR_RESET);
    printf("\n\t\t-------------\n");

    // Find the student record
    FILE *fp = safe_open(DATA_FILE, "rb+");
    if (!fp) {
        set_color(COLOR_RED);
        printf("\n\t\tError: Cannot open data file!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    Student s;
    bool found = false;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.ID == current_user.student_id) {
            found = true;
            break;
        }
    }

    if (!found) {
        fclose(fp);
        set_color(COLOR_RED);
        printf("\n\t\tError: Student record not found!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Check if student has already taken the exam
    if (s.has_taken_exam) {
        fclose(fp);
        set_color(COLOR_RED);
        printf("\n\t\tYou have already taken the entrance exam!");
        printf("\n\t\tYour score was: %.2f", s.mark);
        set_color(COLOR_RESET);
        printf("\n\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Ask for confirmation
    printf("\n\t\tYou are about to start the entrance examination.");
    printf("\n\t\tThis will take approximately 20 minutes.");
    printf("\n\t\tYou can only take the exam once.");
    printf("\n\n\t\tAre you ready to proceed? (Y/N): ");

    fflush(stdin);
    char confirm = getche();

    if (toupper(confirm) != 'Y') {
        fclose(fp);
        printf("\n\n\t\tExam canceled. Returning to menu...");
        getch();
        return;
    }

    // Conduct the exam (similar to the original conduct_exam but simpler)
    float score = 0.0;

    // Load and prepare exam questions
    Question questions[EXAM_QUESTIONS];
    load_questions(questions);

    // Create a random order for questions
    int question_order[EXAM_QUESTIONS];
    shuffle_questions(question_order, EXAM_QUESTIONS);

    // Set up timer
    time_t start_time = time(NULL);
    time_t end_time = start_time + (EXAM_TIME_MINUTES * 60);
    time_t current_time;

    // Take the exam
    for (int i = 0; i < EXAM_QUESTIONS; i++) {
        CLEAR_SCREEN;

        // Display timer
        current_time = time(NULL);
        if (current_time >= end_time) {
            set_color(COLOR_RED);
            printf("\n\n\t\tTIME'S UP!");
            set_color(COLOR_RESET);
            break;
        }

        int mins_left = (end_time - current_time) / 60;
        int secs_left = (end_time - current_time) % 60;

        set_color(COLOR_YELLOW);
        printf("\n\tTime Remaining: %02d:%02d", mins_left, secs_left);
        printf("\n\tQuestion %d of %d", i+1, EXAM_QUESTIONS);
        set_color(COLOR_RESET);

        // Get the question based on the randomized order
        int q_index = question_order[i];

        printf("\n\n\t%s\n", questions[q_index].question_text);

        // Display options
        for (int j = 0; j < 4; j++) {
            printf("\n\t(%c) %s", 'A' + j, questions[q_index].options[j]);
        }

        // Get answer
        printf("\n\n\tYour answer (A/B/C/D or S to skip): ");
        fflush(stdin);
        char answer = getch();
        answer = toupper(answer);

        // Process answer
        if (answer == 'S') {
            printf("\n\tQuestion skipped.");
            getch();
        } else if (answer >= 'A' && answer <= 'D') {
            if (answer == questions[q_index].correct_answer) {
                score += 1.0;
                set_color(COLOR_GREEN);
                printf("\n\tCorrect!");
                set_color(COLOR_RESET);
            } else {
                score -= 0.1;
                set_color(COLOR_RED);
                printf("\n\tIncorrect!");
                set_color(COLOR_RESET);
            }
            getch();
        } else {
            set_color(COLOR_RED);
            printf("\n\tInvalid option! Press any key to try again.");
            set_color(COLOR_RESET);
            getch();
            i--; // Repeat this question
        }
    }

    // Update the student record
    rewind(fp);
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.ID == current_user.student_id) {
            fseek(fp, -sizeof(Student), SEEK_CUR);
            s.mark = score;
            s.has_taken_exam = 1;
            fwrite(&s, sizeof(Student), 1, fp);
            break;
        }
    }

    fclose(fp);

    // Show results
    CLEAR_SCREEN;
    print_header();
    set_color(COLOR_GREEN);
    printf("\n\n\t\tEXAM COMPLETED!");
    printf("\n\t\tYour score: %.2f out of 10.0", score);

    if (score >= 3.0) {
        printf("\n\n\t\tCongratulations! You have PASSED the entrance exam.");
    } else {
        set_color(COLOR_RED);
        printf("\n\n\t\tSorry, you did not pass the entrance exam.");
        printf("\n\t\tA minimum score of 3.0 is required to pass.");
        set_color(COLOR_RESET);
    }

    printf("\n\n\t\tPress any key to return to menu...");
    getch();
}

void user_results() {
    print_header();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tMY EXAM RESULTS");
    set_color(COLOR_RESET);
    printf("\n\t\t---------------\n");

    // Find the student record
    FILE *fp = safe_open(DATA_FILE, "rb");
    if (!fp) {
        set_color(COLOR_RED);
        printf("\n\t\tError: Cannot open data file!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    Student s;
    bool found = false;

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.ID == current_user.student_id) {
            found = true;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        set_color(COLOR_RED);
        printf("\n\t\tError: Student record not found!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Check if student has taken the exam
    if (!s.has_taken_exam) {
        set_color(COLOR_YELLOW);
        printf("\n\t\tYou have not yet taken the entrance exam.");
        printf("\n\t\tSelect 'Take Entrance Exam' from the menu to start.");
        set_color(COLOR_RESET);
        printf("\n\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Display exam results
    printf("\n\t\tName: %s", s.name);
    printf("\n\t\tID: %d", s.ID);
    printf("\n\n\t\tExam Score: %.2f out of 10.0", s.mark);
    printf("\n\t\tResult: ");

    if (s.mark >= 3.0) {
        set_color(COLOR_GREEN);
        printf("PASS");
    } else {
        set_color(COLOR_RED);
        printf("FAIL");
    }
    set_color(COLOR_RESET);
    printf("\n\t\tMinimum passing score: 3.0");

    // Get student ranking
    FILE *rank_fp = safe_open(DATA_FILE, "rb");
    if (rank_fp) {
        // Count students who have taken the exam
        int total = 0, passed = 0;
        Student temp;
        int rank = 1;

        while (fread(&temp, sizeof(Student), 1, rank_fp) == 1) {
            if (temp.has_taken_exam) {
                total++;
                if (temp.mark >= 3.0) passed++;
                if (temp.mark > s.mark) rank++;
            }
        }

        fclose(rank_fp);

        if (s.mark >= 3.0) {
            printf("\n\n\t\tYour Rank: %d out of %d passed students", rank, passed);
        }
        printf("\n\t\tTotal students who took the exam: %d", total);
    }

    printf("\n\n\t\tExam taken on: ");
    if (s.registration_date > 0) {
        char date_str[30];
        struct tm *tm_info = localtime(&s.registration_date);
        strftime(date_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s", date_str);
    } else {
        printf("Unknown");
    }

    printf("\n\n\t\tPress any key to return to menu...");
    getch();
}

/*==============================
 * HELPER FUNCTIONS
 *==============================*/

void init_log_system() {
    // Create data directory if it doesn't exist
    ensure_dir_exists("data");

    FILE *log_fp = fopen(LOG_FILE, "a");
    if(log_fp) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[30];
        strftime(time_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(log_fp, "\n\n[%s] ---------- SYSTEM STARTED ----------\n", time_str);
        fclose(log_fp);
    }
}

void log_message(int level, const char *message) {
    FILE *log_fp = fopen(LOG_FILE, "a");
    if(log_fp) {
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[30];
        strftime(time_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);

        const char *level_str;
        switch(level) {
            case LOG_DEBUG:   level_str = "DEBUG"; break;
            case LOG_INFO:    level_str = "INFO"; break;
            case LOG_WARNING: level_str = "WARNING"; break;
            case LOG_ERROR:   level_str = "ERROR"; break;
            default:          level_str = "UNKNOWN";
        }

        fprintf(log_fp, "[%s] %s: %s\n", time_str, level_str, message);
        fclose(log_fp);
    }
}

void view_logs() {
    print_header();

    FILE *log_fp = fopen(LOG_FILE, "r");
    if(!log_fp) {
        set_color(COLOR_RED);
        printf("\n\n\t\tNo log file found or couldn't open it.");
        set_color(COLOR_RESET);
        printf("\n\n\t\tPress any key to continue...");
        getch();
        return;
    }

    set_color(COLOR_BLUE);
    printf("\n\n\t\tSYSTEM LOGS");
    set_color(COLOR_RESET);
    printf("\n\t\t-----------\n\n");

    char buffer[256];
    int line_count = 0;
    int page = 1;
    int lines_per_page = 20;

    // Count total lines first
    int total_lines = 0;
    while(fgets(buffer, sizeof(buffer), log_fp)) {
        total_lines++;
    }

    rewind(log_fp);

    while(1) {
        CLEAR_SCREEN;
        print_header();

        set_color(COLOR_BLUE);
        printf("\n\t\tSYSTEM LOGS (Page %d)", page);
        set_color(COLOR_RESET);
        printf("\n\t\t");
        print_char('-', 40);
        printf("\n\n");

        line_count = 0;
        int i;
        for (i = 0; i < (page-1) * lines_per_page; i++) {
            if(!fgets(buffer, sizeof(buffer), log_fp)) break;
        }

        line_count = 0;
        while(line_count < lines_per_page && fgets(buffer, sizeof(buffer), log_fp)) {
            // Highlight errors in red
            if(strstr(buffer, "ERROR:") || strstr(buffer, "ERROR]")) {
                set_color(COLOR_RED);
                printf("\t%s", buffer);
                set_color(COLOR_RESET);
            }
            // Highlight warnings in yellow
            else if(strstr(buffer, "WARNING:") || strstr(buffer, "WARNING]")) {
                set_color(COLOR_YELLOW);
                printf("\t%s", buffer);
                set_color(COLOR_RESET);
            }
            else {
                printf("\t%s", buffer);
            }

            line_count++;
        }

        int total_pages = (total_lines + lines_per_page - 1) / lines_per_page;

        printf("\n\n\t\tPage %d of %d", page, total_pages);
        printf("\n\n\t\t[N]ext Page | [P]revious Page | [E]xit\n\t\t");

        char choice = getch();

        if(choice == 'n' || choice == 'N') {
            if(page < total_pages) {
                page++;
                rewind(log_fp);
            }
        }
        else if(choice == 'p' || choice == 'P') {
            if(page > 1) {
                page--;
                rewind(log_fp);
            }
        }
        else if(choice == 'e' || choice == 'E') {
            break;
        }
    }

    fclose(log_fp);
}

void ensure_dir_exists(const char *dir) {
    #if defined(_WIN32) || defined(_WIN64)
    // Windows implementation
    if (!FILE_EXISTS(dir)) {
        MKDIR(dir);
        char log_msg[100];
        sprintf(log_msg, "Created directory: %s", dir);
        log_message(LOG_INFO, log_msg);
    }
    #else
    // Unix/Linux implementation
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        MKDIR(dir);
        char log_msg[100];
        sprintf(log_msg, "Created directory: %s", dir);
        log_message(LOG_INFO, log_msg);
    }
    #endif
}

void print_char(char ch, int n) {
    while(n--) {
        putchar(ch);
    }
}

void print_header() {
    CLEAR_SCREEN;
    printf("\n\n\t");
    print_char('=', 16);

    set_color(COLOR_BLUE);
    printf("[ENTRANCE] [MANAGEMENT] [SYSTEM]");
    set_color(COLOR_RESET);

    print_char('=', 16);
    printf("\n");

    // Display date and time in header
    char datetime[30];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(datetime, 30, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("\n\t\t\t\t\t\tLogin: %s", current_user.username);
    printf("\n\t\t\t\t\t\tDate: %s", datetime);
}

void print_footer() {
    printf("\n\n\t");
    print_char('-', 64);
    printf("\n\t© 2025 Prarambha Bashyal. All Rights Reserved.");
    printf("\n\t");
    print_char('-', 64);
}

void print_separator(char ch) {
    printf("\n\t");
    print_char(ch, 64);
}

void safe_input(char *buffer, int size) {
    fgets(buffer, size, stdin);

    // Remove trailing newline if present
    size_t len = strlen(buffer);
    if(len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }

    // Clear input buffer if overflow occurred
    if(len == size-1 && buffer[len-1] != '\n') {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }
}

FILE* safe_open(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if(fp == NULL) {
        char error_msg[256];
        sprintf(error_msg, "Error opening file %s: %s", filename, strerror(errno));
        log_message(LOG_ERROR, error_msg);
    }
    return fp;
}

bool validate_email(const char *email) {
    // Basic email validation: check for @ symbol and at least one character on each side
    const char *at = strchr(email, '@');
    if(at == NULL || at == email || *(at+1) == '\0') {
        return false;
    }

    // Check for a period after the @ symbol
    const char *dot = strchr(at, '.');
    return (dot != NULL && dot > at && *(dot+1) != '\0');
}

bool validate_phone(const char *phone) {
    // Check that phone contains only digits, spaces, dashes
    for(int i = 0; phone[i] != '\0'; i++) {
        if(!isdigit(phone[i]) && phone[i] != '-' && phone[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool is_numeric(const char *str) {
    while(*str) {
        if(!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

void display_datetime() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buffer[30];

    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s", buffer);
}

void display_about() {
    print_header();

    SystemConfig config = load_system_config();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tABOUT ENTRANCE MANAGEMENT SYSTEM");
    set_color(COLOR_RESET);
    printf("\n\t\t--------------------------------\n");

    printf("\n\t\tSystem Name: %s", config.system_name);
    printf("\n\t\tVersion: %s", VERSION);
    printf("\n\n\t\tInstitution: %s", config.institution_name);
    printf("\n\t\tAddress: %s", config.institution_address);

    printf("\n\n\t\tContact Information:");
    printf("\n\t\t--------------------");
    printf("\n\t\tEmail: %s", config.contact_email);
    printf("\n\t\tPhone: %s", config.contact_phone);

    printf("\n\n\t\tSystem Information:");
    printf("\n\t\t------------------");
    printf("\n\t\tSystem Installation Date: ");
    if (config.setup_date > 0) {
        char date_str[30];
        struct tm *tm_info = localtime(&config.setup_date);
        strftime(date_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s", date_str);
    } else {
        printf("Unknown");
    }

    printf("\n\n\t\tDeveloped by: Prarambha Bashyal");
    printf("\n\t\tLast Updated: July 24, 2025");

    print_footer();

    printf("\n\n\t\tPress any key to return...");
    getch();
}

void display_help() {
    print_header();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tHELP & INSTRUCTIONS");
    set_color(COLOR_RESET);
    printf("\n\t\t-------------------\n");

    if (current_user.role == ROLE_ADMIN) {
        printf("\n\t\tADMIN PANEL GUIDE:");
        printf("\n\t\t------------------");
        printf("\n\t\t1. Student Management - Register and manage students");
        printf("\n\t\t2. User Management - Create and manage user accounts");
        printf("\n\t\t3. System Settings - Configure system parameters");
        printf("\n\t\t4. Reports - Generate various reports");
        printf("\n\t\t5. Backup & Restore - Protect and recover data");
        printf("\n\t\t6. View Logs - Check system activity");
    } else {
        printf("\n\t\tSTUDENT PANEL GUIDE:");
        printf("\n\t\t--------------------");
        printf("\n\t\t1. My Profile - View your personal information");
        printf("\n\t\t2. Take Entrance Exam - Start your exam (one-time only)");
        printf("\n\t\t3. View Results - See your exam score and ranking");
        printf("\n\t\t4. Change Password - Update your login password");
    }

    printf("\n\n\t\tGENERAL TIPS:");
    printf("\n\t\t-------------");
    printf("\n\t\t- Keep your password secure and don't share it");
    printf("\n\t\t- Log out when you are done using the system");
    printf("\n\t\t- Contact administrator if you encounter any issues");

    printf("\n\n\t\tEXAM GUIDELINES:");
    printf("\n\t\t----------------");
    printf("\n\t\t- The entrance exam consists of 10 questions");
    printf("\n\t\t- Time limit: 20 minutes");
    printf("\n\t\t- Each question is worth 1 mark");
    printf("\n\t\t- Negative marking: -0.1 per wrong answer");
    printf("\n\t\t- Minimum passing score: 3.0 out of 10.0");
    printf("\n\t\t- You can skip questions by pressing 'S'");
    printf("\n\t\t- You can only take the exam once");

    printf("\n\n\t\tPress any key to return...");
    getch();
}

char* get_role_name(int role) {
    switch (role) {
        case ROLE_ADMIN:
            return "Administrator";
        case ROLE_EXAMINER:
            return "Examiner";
        case ROLE_USER:
            return "Student";
        default:
            return "Unknown";
    }
}

/*==============================
 * BACKUP & REPORT FUNCTIONS
 *==============================*/

void backup_menu() {
    int option;

    do {
        print_header();
        printf("\n\t\t");
        set_color(COLOR_BLUE);
        printf("BACKUP & REPORT MENU");
        set_color(COLOR_RESET);

        printf("\n\n\t\t1. Create Binary Backup");
        printf("\n\t\t2. Export Data to CSV");
        printf("\n\t\t3. Restore Backup");
        printf("\n\t\t4. Generate Reports");
        printf("\n\t\t0. Return to Main Menu");

        printf("\n\n\t\tEnter option: ");
        scanf("%d", &option);

        switch(option) {
            case 1: create_binary_backup(); break;
            case 2: export_csv_backup(); break;
            case 3: restore_backup(); break;
            case 4: generate_report(); break;
            case 0: return;
            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option! Press any key to try again...");
                set_color(COLOR_RESET);
                getch();
        }
    } while(option != 0);
}

void create_binary_backup() {
    print_header();

    FILE *src, *dst;
    char backup_filename[100];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Create backups directory if it doesn't exist
    ensure_dir_exists("backups");

    // Create backup filename with timestamp
    strftime(backup_filename, 100, "backups/student_%Y%m%d_%H%M%S.dat", tm_info);

    src = safe_open(DATA_FILE, "rb");
    if(src == NULL) {
        log_message(LOG_ERROR, "Could not open source file for backup");
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not open source file!");
        set_color(COLOR_RESET);
        getch();
        return;
    }

    dst = safe_open(backup_filename, "wb");
    if(dst == NULL) {
        log_message(LOG_ERROR, "Could not create backup file");
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not create backup file!");
        set_color(COLOR_RESET);
        fclose(src);
        getch();
        return;
    }

    // Copy file contents
    char buffer[1024];
    size_t bytes_read;

    printf("\n\n\t\tCreating binary backup...");

    while((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dst);
    }

    fclose(src);
    fclose(dst);

    char log_msg[100];
    sprintf(log_msg, "Binary backup created: %s", backup_filename);
    log_message(LOG_INFO, log_msg);

    set_color(COLOR_GREEN);
    printf("\n\n\t\tBackup created successfully: %s", backup_filename);
    set_color(COLOR_RESET);

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

void export_csv_backup() {
    print_header();

    FILE *data_file, *csv_file;
    char csv_filename[100];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    Student s;

    // Create backups directory if it doesn't exist
    ensure_dir_exists("backups");

    // Create CSV filename with timestamp
    strftime(csv_filename, 100, "backups/student_data_%Y%m%d_%H%M%S.csv", tm_info);

    data_file = safe_open(DATA_FILE, "rb");
    if(data_file == NULL) {
        log_message(LOG_ERROR, "Could not open data file for CSV export");
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not open data file!");
        set_color(COLOR_RESET);
        getch();
        return;
    }

    csv_file = safe_open(csv_filename, "w");
    if(csv_file == NULL) {
        log_message(LOG_ERROR, "Could not create CSV file");
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not create CSV file!");
        set_color(COLOR_RESET);
        fclose(data_file);
        getch();
        return;
    }

    printf("\n\n\t\tExporting data to CSV...");

    // Write CSV header
    fprintf(csv_file, "ID,Name,DOB,Address,Phone,Email,GuardianName,GuardianPhone,HasTakenExam,Mark,RegistrationDate\n");

    // Write each student record as a CSV row
    while(fread(&s, sizeof(s), 1, data_file) == 1) {
        char reg_date[30] = "";

        // Format registration date if available
        if(s.registration_date > 0) {
            struct tm *reg_tm = localtime(&s.registration_date);
            strftime(reg_date, 30, "%Y-%m-%d %H:%M:%S", reg_tm);
        }

        // Escape commas in text fields and wrap in quotes
        fprintf(csv_file, "%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%.2f,\"%s\"\n",
                s.ID, s.name, s.dob, s.address, s.phone, s.email,
                s.guardian_name, s.guardian_phone, s.has_taken_exam, s.mark, reg_date);
    }

    fclose(data_file);
    fclose(csv_file);

    char log_msg[100];
    sprintf(log_msg, "CSV backup created: %s", csv_filename);
    log_message(LOG_INFO, log_msg);

    set_color(COLOR_GREEN);
    printf("\n\n\t\tCSV file created successfully: %s", csv_filename);
    printf("\n\t\tYou can open this file in Excel or any spreadsheet program.");
    set_color(COLOR_RESET);

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

void restore_backup() {
    print_header();

    set_color(COLOR_YELLOW);
    printf("\n\n\t\tWARNING: Restoring a backup will overwrite current data!");
    printf("\n\t\tMake sure you have a backup of your current data first.");
    set_color(COLOR_RESET);

    printf("\n\n\t\tDo you want to continue? (Y/N): ");
    fflush(stdin);
    char confirm = getche();

    if (toupper(confirm) != 'Y') {
        printf("\n\n\t\tRestore operation canceled.");
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    printf("\n\n\t\tRestoration functionality will be implemented in next version.");
    printf("\n\n\t\tTo restore a backup manually:");
    printf("\n\t\t1. Exit the program");
    printf("\n\t\t2. Copy your backup file to: %s", DATA_FILE);
    printf("\n\t\t3. Restart the program");

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

void generate_report() {
    print_header();
    FILE *data_file, *report_file;
    char report_filename[100];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    Student s;
    int total = 0, took_exam = 0, passed = 0;
    float total_marks = 0, pass_rate = 0, avg_score = 0;

    // Create reports directory if it doesn't exist
    ensure_dir_exists("reports");

    // Create report filename with timestamp
    strftime(report_filename, 100, "reports/exam_report_%Y%m%d_%H%M%S.txt", tm_info);

    data_file = safe_open(DATA_FILE, "rb");
    if(data_file == NULL) {
        log_message(LOG_ERROR, "Could not open data file for report generation");
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not open data file!");
        set_color(COLOR_RESET);
        getch();
        return;
    }

    // First pass - collect statistics
    while(fread(&s, sizeof(s), 1, data_file) == 1) {
        total++;
        if(s.has_taken_exam) {
            took_exam++;
            total_marks += s.mark;
            if(s.mark >= 3.0) {
                passed++;
            }
        }
    }

    if(took_exam > 0) {
        avg_score = total_marks / took_exam;
        pass_rate = (float)passed / took_exam * 100;
    }

    // Create the report file
    report_file = safe_open(report_filename, "w");
    if(report_file == NULL) {
        log_message(LOG_ERROR, "Could not create report file");
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not create report file!");
        set_color(COLOR_RESET);
        fclose(data_file);
        getch();
        return;
    }

    // Write the report header
    fprintf(report_file, "===============================================\n");
    fprintf(report_file, "       ENTRANCE EXAMINATION SYSTEM REPORT      \n");
    fprintf(report_file, "===============================================\n\n");

    char date_str[30];
    strftime(date_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(report_file, "Report Generated: %s\n\n", date_str);

    // Write summary statistics
    fprintf(report_file, "SUMMARY STATISTICS:\n");
    fprintf(report_file, "-------------------\n");
    fprintf(report_file, "Total Registered Students: %d\n", total);
    fprintf(report_file, "Students Who Took Exam: %d (%.1f%%)\n", took_exam, total > 0 ? (float)took_exam / total * 100 : 0);
    fprintf(report_file, "Students Who Passed (>=3.0): %d\n", passed);
    fprintf(report_file, "Pass Rate: %.1f%%\n", pass_rate);
    fprintf(report_file, "Average Score: %.2f\n\n", avg_score);

    // Write detailed student results
    fprintf(report_file, "DETAILED RESULTS:\n");
    fprintf(report_file, "----------------\n");
    fprintf(report_file, "%-5s %-30s %-10s %-10s\n", "ID", "Name", "Mark", "Status");
    fprintf(report_file, "--------------------------------------------------------\n");

    rewind(data_file);
    while(fread(&s, sizeof(s), 1, data_file) == 1) {
        if(s.has_taken_exam) {
            fprintf(report_file, "%-5d %-30s %-10.2f %-10s\n",
                s.ID, s.name, s.mark, s.mark >= 3.0 ? "PASS" : "FAIL");
        }
    }

    fclose(data_file);
    fclose(report_file);

    char log_msg[100];
    sprintf(log_msg, "Exam report created: %s", report_filename);
    log_message(LOG_INFO, log_msg);

    // Display report on screen too
    print_header();
    set_color(COLOR_GREEN);
    printf("\n\n\t\tREPORT GENERATED SUCCESSFULLY");
    set_color(COLOR_RESET);
    printf("\n\t\tReport saved to: %s\n\n", report_filename);

    printf("\n\t\tSUMMARY STATISTICS:");
    printf("\n\t\t-------------------");
    printf("\n\t\tTotal Registered Students: %d", total);
    printf("\n\t\tStudents Who Took Exam: %d (%.1f%%)", took_exam, total > 0 ? (float)took_exam / total * 100 : 0);
    printf("\n\t\tStudents Who Passed (>=3.0): %d", passed);
    printf("\n\t\tPass Rate: %.1f%%", pass_rate);
    printf("\n\t\tAverage Score: %.2f\n", avg_score);

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

/*==============================
 * QR CODE FUNCTIONALITY
 *==============================*/

void generate_qr_code(Student *s) {
    // In a real implementation, this would use a QR code library
    // For this example, we'll simulate by creating a unique string

    char qr_data[MAX_QR_DATA];

    // Create a unique string combining student ID and other info
    sprintf(qr_data, "EMS:%d:%s:%ld", s->ID, s->name, s->registration_date);

    // Store in the student record
    strncpy(s->qr_code, qr_data, MAX_QR_DATA);

    char log_msg[100];
    sprintf(log_msg, "QR code generated for student ID %d", s->ID);
    log_message(LOG_INFO, log_msg);
}

void display_qr_code(const char *qr_data) {
    print_header();

    // This is a very simple ASCII representation of a QR code
    // In a real implementation, you would use a proper library

    printf("\n\n\t\tQR CODE (Simulated):\n\n");

    int size = 10; // Size of our "QR code"

    printf("\t\t\t");
    for (int i = 0; i < size+2; i++) printf("█");
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("\t\t\t█");
        for (int j = 0; j < size; j++) {
            // Create a pattern based on the qr_data and position
            // This isn't a real QR code, just a visual representation
            int index = (i + j) % strlen(qr_data);
            if ((qr_data[index] + i + j) % 3 == 0)
                printf("  ");
            else
                printf("██");
        }
        printf("█\n");
    }

    printf("\t\t\t");
    for (int i = 0; i < size+2; i++) printf("█");
    printf("\n");

    printf("\n\t\tData: %s\n", qr_data);

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

void check_in_with_qr(FILE *fp) {
    print_header();

    set_color(COLOR_YELLOW);
    printf("\n\n\t\tQR CODE CHECK-IN");
    set_color(COLOR_RESET);

    printf("\n\n\t\t1. Scan Student QR Code");
    printf("\n\t\t2. Manual ID Entry");
    printf("\n\t\t3. Generate QR Code for Student");
    printf("\n\t\t0. Back to Main Menu");

    printf("\n\n\t\tSelect option: ");
    int option;
    scanf("%d", &option);

    Student s;
    int id;
    bool found = false;

    switch(option) {
        case 1:
            // Simulate QR code scanning
            printf("\n\n\t\tScanning QR code... (Simulated)");
            printf("\n\t\tPlease wait...");

            // Add a small delay to simulate scanning
            for (int i = 0; i < 5; i++) {
                printf(".");
                fflush(stdout);
                #if defined(_WIN32) || defined(_WIN64)
                    Sleep(500);
                #else
                    usleep(500000);
                #endif
            }

            printf("\n\n\t\tEnter the ID from the scanned code: ");
            scanf("%d", &id);

            // Now proceed like manual entry
            rewind(fp);
            while(fread(&s, sizeof(s), 1, fp) == 1) {
                if(s.ID == id) {
                    found = true;
                    break;
                }
            }
            break;

        case 2:
            printf("\n\n\t\tEnter student ID: ");
            scanf("%d", &id);

            rewind(fp);
            while(fread(&s, sizeof(s), 1, fp) == 1) {
                if(s.ID == id) {
                    found = true;
                    break;
                }
            }
            break;

        case 3:
            printf("\n\n\t\tEnter student ID to generate QR code: ");
            scanf("%d", &id);

            rewind(fp);
            found = false;
            while(fread(&s, sizeof(s), 1, fp) == 1) {
                if(s.ID == id) {
                    found = true;
                    generate_qr_code(&s);

                    // Save the updated record with QR code
                    fseek(fp, -sizeof(s), SEEK_CUR);
                    fwrite(&s, sizeof(s), 1, fp);

                    display_qr_code(s.qr_code);
                    return;
                }
            }

            if(!found) {
                set_color(COLOR_RED);
                printf("\n\t\tStudent not found!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to continue...");
                getch();
            }
            return;

        case 0:
            return;

        default:
            set_color(COLOR_RED);
            printf("\n\t\tInvalid option!");
            set_color(COLOR_RESET);
            printf("\n\t\tPress any key to continue...");
            getch();
            return;
    }

    if(found) {
        // Record the check-in
        char log_msg[100];
        sprintf(log_msg, "Student checked in: ID=%d, Name=%s", s.ID, s.name);
        log_message(LOG_INFO, log_msg);

        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        char time_str[30];
        strftime(time_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);

        set_color(COLOR_GREEN);
        printf("\n\n\t\tStudent Check-in Successful!");
        set_color(COLOR_RESET);
        printf("\n\t\tName: %s", s.name);
        printf("\n\t\tID: %d", s.ID);
        printf("\n\t\tDate/Time: %s", time_str);

        if (s.has_taken_exam) {
            printf("\n\t\tExam Status: Completed (Score: %.2f)", s.mark);
        } else {
            printf("\n\t\tExam Status: Not taken");
        }
    } else {
        set_color(COLOR_RED);
        printf("\n\n\t\tStudent not found!");
        set_color(COLOR_RESET);
    }

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

/*==============================
 * EXAM SYSTEM FUNCTIONS
 *==============================*/

void load_questions(Question questions[]) {
    // In a real implementation, these would be loaded from a file
    // Hardcoding for demonstration purposes

    strcpy(questions[0].question_text, "The word 'engineer' has its primary stress on its ______ syllable:");
    strcpy(questions[0].options[0], "first");
    strcpy(questions[0].options[1], "second");
    strcpy(questions[0].options[2], "third");
    strcpy(questions[0].options[3], "fourth");
    questions[0].correct_answer = 'C';
    questions[0].difficulty = 2;
    strcpy(questions[0].category, "English");

    strcpy(questions[1].question_text, "Two and two _____ four.");
    strcpy(questions[1].options[0], "makes");
    strcpy(questions[1].options[1], "make");
    strcpy(questions[1].options[2], "have made");
    strcpy(questions[1].options[3], "will be made");
    questions[1].correct_answer = 'A';
    questions[1].difficulty = 1;
    strcpy(questions[1].category, "English");

    strcpy(questions[2].question_text, "pH of 0.2 N H2SO4 is:");
    strcpy(questions[2].options[0], "0.69");
    strcpy(questions[2].options[1], "1.2");
    strcpy(questions[2].options[2], "0.76");
    strcpy(questions[2].options[3], "0.56");
    questions[2].correct_answer = 'A';
    questions[2].difficulty = 3;
    strcpy(questions[2].category, "Chemistry");

    strcpy(questions[3].question_text, "Catalytic oxidation of ammonia in air forms:");
    strcpy(questions[3].options[0], "NO2");
    strcpy(questions[3].options[1], "N2O5");
    strcpy(questions[3].options[2], "N2O");
    strcpy(questions[3].options[3], "NO");
    questions[3].correct_answer = 'D';
    questions[3].difficulty = 3;
    strcpy(questions[3].category, "Chemistry");

    strcpy(questions[4].question_text, "The magnetism of the magnet is due to:");
    strcpy(questions[4].options[0], "the spin motion of electron");
    strcpy(questions[4].options[1], "cosmic ray");
    strcpy(questions[4].options[2], "the earth");
    strcpy(questions[4].options[3], "pressure of big magnet inside the earth");
    questions[4].correct_answer = 'A';
    questions[4].difficulty = 2;
    strcpy(questions[4].category, "Physics");

    strcpy(questions[5].question_text, "The maximum percentage of ingredients in cement is that of:");
    strcpy(questions[5].options[0], "magnesia");
    strcpy(questions[5].options[1], "lime");
    strcpy(questions[5].options[2], "alumina");
    strcpy(questions[5].options[3], "silica");
    questions[5].correct_answer = 'B';
    questions[5].difficulty = 4;
    strcpy(questions[5].category, "Chemistry");

    strcpy(questions[6].question_text, "The second derivative of f(x) = 1/x at point (1,1) is equal to:");
    strcpy(questions[6].options[0], "1");
    strcpy(questions[6].options[1], "-1");
    strcpy(questions[6].options[2], "2");
    strcpy(questions[6].options[3], "-2");
    questions[6].correct_answer = 'C';
    questions[6].difficulty = 4;
    strcpy(questions[6].category, "Mathematics");

    strcpy(questions[7].question_text, "Which of the following is a logic gate?");
    strcpy(questions[7].options[0], "PUT");
    strcpy(questions[7].options[1], "THEN");
    strcpy(questions[7].options[2], "NOR");
    strcpy(questions[7].options[3], "WHEN");
    questions[7].correct_answer = 'C';
    questions[7].difficulty = 2;
    strcpy(questions[7].category, "Computer Science");

    strcpy(questions[8].question_text, "If the line 2x+3y+4+k(-x+y+5)=0 is horizontal then the value of k is:");
    strcpy(questions[8].options[0], "0");
    strcpy(questions[8].options[1], "1");
    strcpy(questions[8].options[2], "3");
    strcpy(questions[8].options[3], "2");
    questions[8].correct_answer = 'D';
    questions[8].difficulty = 5;
    strcpy(questions[8].category, "Mathematics");

    strcpy(questions[9].question_text, "IUPAC name of CH3-CH2-CH2-CH(OCH3)-CO Br is:");
    strcpy(questions[9].options[0], "2-methoxylpentanoyl bromide");
    strcpy(questions[9].options[1], "3-methoxylpentanoyl bromide");
    strcpy(questions[9].options[2], "3-methoxyhexanoyl bromide");
    strcpy(questions[9].options[3], "2-methoxyhexanoyl bromide");
    questions[9].correct_answer = 'A';
    questions[9].difficulty = 5;
    strcpy(questions[9].category, "Chemistry");
}

void shuffle_questions(int order[], int count) {
    // Initialize order array
    for(int i = 0; i < count; i++) {
        order[i] = i;
    }

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Shuffle using Fisher-Yates algorithm
    for(int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }
}

/*==============================
 * STUDENT MANAGEMENT FUNCTIONS
 *==============================*/

// Helper function for case-insensitive string comparison
int strcasecmp(const char *s1, const char *s2) {
    while(*s1 && *s2) {
        if (tolower(*s1) != tolower(*s2)) {
            break;
        }
        s1++;
        s2++;
    }
    return tolower(*(unsigned char *)s1) - tolower(*(unsigned char *)s2);
}

// Helper function for fuzzy name matching
int fuzzy_match(const char *str, const char *pattern) {
    if (!*pattern) return 1;
    if (!*str) return 0;
    
    if (tolower(*pattern) == tolower(*str)) {
        return fuzzy_match(str + 1, pattern + 1);
    }
    
    return fuzzy_match(str + 1, pattern);
}

// Structure to hold exam questions and answers
typedef struct {
    char question[256];
    char options[4][100];
    int correct_option; // 0-3 for A-D
} ExamQuestion;

// Array of sample questions
ExamQuestion exam_questions[] = {
    {
        "What is the capital of Nepal?",
        {"A. Kathmandu", "B. Pokhara", "C. Biratnagar", "D. Lalitpur"},
        0
    },
    {
        "Which is the highest peak in the world?",
        {"A. K2", "B. Kangchenjunga", "C. Mount Everest", "D. Lhotse"},
        2
    },
    {
        "What is the main language of Nepal?",
        {"A. Hindi", "B. English", "C. Nepali", "D. Maithili"},
        2
    },
    {
        "Which river is known as the holiest river in Nepal?",
        {"A. Bagmati", "B. Gandaki", "C. Koshi", "D. Karnali"},
        0
    },
    {
        "What is the national flower of Nepal?",
        {"A. Sunflower", "B. Marigold", "C. Rhododendron", "D. Rose"},
        2
    }
};

#define TOTAL_QUESTIONS (sizeof(exam_questions) / sizeof(ExamQuestion))

// Comparison function for qsort to sort students by mark (descending)
int compare_students(const void *a, const void *b) {
    const Student *studentA = (const Student *)a;
    const Student *studentB = (const Student *)b;
    
    // Sort by mark in descending order
    if (studentB->mark > studentA->mark) return 1;
    if (studentB->mark < studentA->mark) return -1;
    
    // If marks are equal, sort by name
    return strcasecmp(studentA->name, studentB->name);
}

void rank_students(FILE *fp) {
    print_header();
    printf("\n\n\t\tSTUDENT RANKINGS");
    printf("\n\t\t-----------------\n");
    
    // Count number of students who have taken the exam
    rewind(fp);
    Student s;
    int count = 0;
    
    while(fread(&s, sizeof(s), 1, fp) == 1) {
        if(s.has_taken_exam) {
            count++;
        }
    }
    
    if(count == 0) {
        printf("\n\tNo students have taken the exam yet!");
        printf("\n\n\tPress any key to return...");
        getch();
        return;
    }
    
    // Allocate array for students who have taken the exam
    Student *students = (Student *)malloc(count * sizeof(Student));
    if(!students) {
        printf("\n\tMemory allocation error!");
        log_message(LOG_ERROR, "Failed to allocate memory for student rankings");
        printf("\n\n\tPress any key to return...");
        getch();
        return;
    }
    
    // Read students who have taken the exam into the array
    rewind(fp);
    int index = 0;
    while(fread(&s, sizeof(s), 1, fp) == 1) {
        if(s.has_taken_exam) {
            students[index++] = s;
        }
    }
    
    // Sort students by mark (descending)
    qsort(students, count, sizeof(Student), compare_students);
    
    // Display rankings with pagination
    int page = 1;
    int records_per_page = 10;
    int total_pages = (count + records_per_page - 1) / records_per_page;
    char choice;
    
    do {
        print_header();
        printf("\n\n\t\tSTUDENT RANKINGS");
        printf("\n\t\t-----------------\n");
        
        // Display table header
        printf("\n\t%-6s %-8s %-30s %-8s %-10s %-15s", 
               "Rank", "ID", "Name", "Score", "Status", "Date Taken");
        printf("\n\t");
        print_char('-', 80);
        
        // Calculate start and end indices for current page
        int start = (page - 1) * records_per_page;
        int end = start + records_per_page;
        if(end > count) end = count;
        
        // Display records for current page
        for(int i = start; i < end; i++) {
            char date_str[12] = "N/A";
            
            // Format exam date if available
            if(students[i].registration_date > 0) {
                struct tm *tm_info = localtime(&students[i].registration_date);
                strftime(date_str, 12, "%Y-%m-%d", tm_info);
            }
            
            // Set color based on pass/fail
            if(students[i].mark >= 3.0) {
                set_color(COLOR_GREEN);
            } else {
                set_color(COLOR_RED);
            }
            
            // Truncate long names
            char display_name[30];
            strncpy(display_name, students[i].name, 27);
            if(strlen(students[i].name) > 27) {
                strcpy(display_name + 27, "...");
            }
            
            // Print student ranking
            printf("\n\t%-6d %-8d %-30s %-8.2f %-10s %-15s", 
                   i + 1, // Rank (1-based)
                   students[i].ID,
                   display_name,
                   students[i].mark,
                   (students[i].mark >= 3.0) ? "PASSED" : "FAILED",
                   date_str);
        }
        
        set_color(COLOR_RESET);
        
        // Display page info and navigation
        printf("\n\n\tPage %d of %d (Showing %d-%d of %d students)", 
               page, total_pages, start + 1, end, count);
        printf("\n\t[N]ext [P]revious [G]o to page [S]ave to file [Q]uit: ");
        
        // Get user choice
        fflush(stdin);
        choice = tolower(getche());
        
        // Process navigation
        switch(choice) {
            case 'n':
                if(page < total_pages) page++;
                break;
                
            case 'p':
                if(page > 1) page--;
                break;
                
            case 'g':
                printf("\n\tEnter page number (1-%d): ", total_pages);
                int new_page;
                if(scanf("%d", &new_page) == 1 && new_page >= 1 && new_page <= total_pages) {
                    page = new_page;
                } else {
                    set_color(COLOR_RED);
                    printf("\n\tInvalid page number!");
                    set_color(COLOR_RESET);
                    getch();
                }
                fflush(stdin);
                break;
                
            case 's': {
                // Save rankings to file
                char filename[100];
                time_t now = time(NULL);
                struct tm *tm_info = localtime(&now);
                strftime(filename, sizeof(filename), "rankings_%Y%m%d_%H%M%S.txt", tm_info);
                
                FILE *outfile = fopen(filename, "w");
                if(!outfile) {
                    set_color(COLOR_RED);
                    printf("\n\tError: Could not create output file!");
                    set_color(COLOR_RESET);
                    getch();
                    break;
                }
                
                // Write header
                fprintf(outfile, "STUDENT RANKINGS\n");
                fprintf(outfile, "Generated on: %s\n\n", ctime(&now));
                fprintf(outfile, "%-6s %-8s %-30s %-8s %-10s %-15s\n", 
                        "Rank", "ID", "Name", "Score", "Status", "Date Taken");
                fprintf(outfile, "%s\n", "----------------------------------------------------------------");
                
                // Write all records
                for(int i = 0; i < count; i++) {
                    char date_str[12] = "N/A";
                    if(students[i].registration_date > 0) {
                        struct tm *tm_info = localtime(&students[i].registration_date);
                        strftime(date_str, 12, "%Y-%m-%d", tm_info);
                    }
                    
                    fprintf(outfile, "%-6d %-8d %-30s %-8.2f %-10s %-15s\n", 
                           i + 1, // Rank (1-based)
                           students[i].ID,
                           students[i].name,
                           students[i].mark,
                           (students[i].mark >= 3.0) ? "PASSED" : "FAILED",
                           date_str);
                }
                
                fclose(outfile);
                
                // Log the export
                char log_msg[200];
                sprintf(log_msg, "Exported student rankings to %s", filename);
                log_message(LOG_INFO, log_msg);
                
                set_color(COLOR_GREEN);
                printf("\n\n\tRankings saved to %s", filename);
                set_color(COLOR_RESET);
                printf("\n\tPress any key to continue...");
                getch();
                break;
            }
                
            case 'q':
                free(students);
                return;
                
            default:
                break;
        }
        
    } while(1);
}

void conduct_exam(FILE *fp) {
    print_header();
    printf("\n\n\t\tCONDUCT ENTRANCE EXAM");
    printf("\n\t\t----------------------\n");
    
    int id, found = 0;
    Student s;
    long pos = 0;
    char log_msg[200];
    
    printf("\n\tEnter Student ID to take exam: ");
    if (scanf("%d", &id) != 1) {
        printf("\n\tInvalid ID format!");
        fflush(stdin);
        getch();
        return;
    }
    
    rewind(fp);
    
    // Find the student record
    while(fread(&s, sizeof(s), 1, fp) == 1) {
        if(s.ID == id) {
            found = 1;
            pos = ftell(fp) - sizeof(s);
            break;
        }
    }
    
    if(!found) {
        printf("\n\tStudent with ID %d not found!", id);
        printf("\n\n\tPress any key to return...");
        getch();
        return;
    }
    
    // Check if student has already taken the exam
    if(s.has_taken_exam) {
        printf("\n\tThis student has already taken the exam!");
        printf("\n\tCurrent score: %.2f/5.00", s.mark);
        printf("\n\tDo you want to retake the exam? (y/n): ");
        
        char confirm;
        fflush(stdin);
        confirm = tolower(getche());
        
        if(confirm != 'y') {
            printf("\n\n\tExam cancelled. Returning to menu...");
            getch();
            return;
        }
    }
    
    // Display exam instructions
    print_header();
    printf("\n\n\t\tEXAM INSTRUCTIONS");
    printf("\n\t\t------------------");
    printf("\n\n\t1. This exam consists of %d multiple-choice questions.", TOTAL_QUESTIONS);
    printf("\n\t2. Each question has 4 options (A, B, C, D).");
    printf("\n\t3. Select the correct answer by entering the corresponding letter.");
    printf("\n\t4. Each correct answer earns you 1 point.");
    printf("\n\t5. There is no negative marking for wrong answers.");
    printf("\n\t6. You must score at least 3.0 points to pass the exam.");
    printf("\n\n\tPress any key to start the exam...");
    getch();
    
    // Variables to track exam progress
    int score = 0;
    char answer;
    
    // Present each question
    for(int i = 0; i < TOTAL_QUESTIONS; i++) {
        print_header();
        printf("\n\tQuestion %d of %d\n\n", i+1, TOTAL_QUESTIONS);
        printf("\t%s\n\n", exam_questions[i].question);
        
        // Display options
        for(int j = 0; j < 4; j++) {
            printf("\t%s\n", exam_questions[i].options[j]);
        }
        
        // Get and validate answer
        do {
            printf("\n\tYour answer (A/B/C/D): ");
            fflush(stdin);
            answer = toupper(getchar());
            
            // Clear input buffer
            while(getchar() != '\n');
            
            if(answer < 'A' || answer > 'D') {
                printf("\tInvalid input! Please enter A, B, C, or D.");
            } else {
                break;
            }
        } while(1);
        
        // Check if answer is correct
        if((answer - 'A') == exam_questions[i].correct_option) {
            score++;
            printf("\n\t\x1b[32mCorrect!\x1b[0m");
        } else {
            printf("\n\t\x1b[31mIncorrect!\x1b[0m The correct answer was %c.", 
                   'A' + exam_questions[i].correct_option);
        }
        
        // Pause before next question
        if(i < TOTAL_QUESTIONS - 1) {
            printf("\n\n\tPress any key for the next question...");
            getch();
        }
    }
    
    // Calculate final score (out of 5.00)
    float final_score = (float)score / TOTAL_QUESTIONS * 5.0;
    
    // Display results
    print_header();
    printf("\n\n\t\tEXAM RESULTS");
    printf("\n\t\t------------\n");
    
    printf("\n\tStudent: %s (ID: %d)", s.name, s.ID);
    printf("\n\tCorrect Answers: %d/%d", score, TOTAL_QUESTIONS);
    printf("\n\tFinal Score: %.2f/5.00", final_score);
    
    if(final_score >= 3.0) {
        set_color(COLOR_GREEN);
        printf("\n\n\tCONGRATULATIONS! You have passed the exam!");
    } else {
        set_color(COLOR_RED);
        printf("\n\n\tSORRY! You did not pass the exam.");
    }
    set_color(COLOR_RESET);
    
    // Update student record
    s.mark = final_score;
    s.has_taken_exam = 1;
    
    // Save the changes back to the file
    fseek(fp, pos, SEEK_SET);
    if(fwrite(&s, sizeof(s), 1, fp) != 1) {
        printf("\n\n\tError saving exam results!");
        log_message(LOG_ERROR, "Failed to save exam results to file");
    } else {
        fflush(fp);
        
        // Log the exam result
        sprintf(log_msg, "Student ID %d (%s) took the exam. Score: %.2f/5.00 - %s", 
                s.ID, s.name, final_score, 
                (final_score >= 3.0) ? "PASSED" : "FAILED");
        log_message(LOG_INFO, log_msg);
        
        printf("\n\n\tExam results have been recorded successfully!");
    }
    
    printf("\n\n\tPress any key to return to the menu...");
    getch();
}

void modify_student(FILE *fp) {
    print_header();
    printf("\n\n\t\tMODIFY STUDENT RECORD");
    printf("\n\t\t----------------------\n");
    
    int id, found = 0, choice;
    Student s;
    long pos = 0;
    char log_msg[200];
    
    printf("\n\tEnter Student ID to modify: ");
    if (scanf("%d", &id) != 1) {
        printf("\n\tInvalid ID format!");
        fflush(stdin);
        getch();
        return;
    }
    
    rewind(fp);
    
    // Find the student record
    while(fread(&s, sizeof(s), 1, fp) == 1) {
        if(s.ID == id) {
            found = 1;
            pos = ftell(fp) - sizeof(s);
            break;
        }
    }
    
    if(!found) {
        printf("\n\tStudent with ID %d not found!", id);
        printf("\n\n\tPress any key to return...");
        getch();
        return;
    }
    
    // Display current student info
    printf("\n\tCurrent Student Information:");
    printf("\n\t--------------------------");
    printf("\n\t1. Name: %s", s.name);
    printf("\n\t2. Date of Birth: %s", s.dob);
    printf("\n\t3. Address: %s", s.address);
    printf("\n\t4. Phone: %s", s.phone);
    printf("\n\t5. Email: %s", s.email);
    printf("\n\t6. Guardian Name: %s", s.guardian_name);
    printf("\n\t7. Guardian Phone: %s", s.guardian_phone);
    
    if(s.has_taken_exam) {
        printf("\n\t8. Exam Score: %.2f", s.mark);
    }
    
    printf("\n\n\t0. Return to previous menu");
    printf("\n\n\tSelect field to modify (1-8): ");
    
    if(scanf("%d", &choice) != 1) {
        printf("\n\tInvalid choice!");
        fflush(stdin);
        getch();
        return;
    }
    
    if(choice < 0 || choice > 8) {
        printf("\n\tInvalid choice!");
        getch();
        return;
    }
    
    if(choice == 0) {
        return;
    }
    
    // Store old value for logging
    char old_value[200] = "";
    
    // Modify the selected field
    switch(choice) {
        case 1:
            strcpy(old_value, s.name);
            printf("\n\tEnter new name: ");
            fflush(stdin);
            safe_input(s.name, MAX_NAME);
            break;
            
        case 2:
            strcpy(old_value, s.dob);
            printf("\n\tEnter new date of birth (YYYY-MM-DD): ");
            fflush(stdin);
            safe_input(s.dob, MAX_DOB);
            break;
            
        case 3:
            strcpy(old_value, s.address);
            printf("\n\tEnter new address: ");
            fflush(stdin);
            safe_input(s.address, MAX_ADDRESS);
            break;
            
        case 4: {
            strcpy(old_value, s.phone);
            char new_phone[MAX_PHONE];
            do {
                printf("\n\tEnter new phone number: ");
                fflush(stdin);
                safe_input(new_phone, MAX_PHONE);
                
                if(!validate_phone(new_phone)) {
                    set_color(COLOR_RED);
                    printf("\n\tInvalid phone number. Please enter only digits.");
                    set_color(COLOR_RESET);
                } else {
                    strcpy(s.phone, new_phone);
                    break;
                }
            } while(1);
            break;
        }
            
        case 5: {
            strcpy(old_value, s.email);
            char new_email[MAX_EMAIL];
            do {
                printf("\n\tEnter new email: ");
                fflush(stdin);
                safe_input(new_email, MAX_EMAIL);
                
                if(!validate_email(new_email)) {
                    set_color(COLOR_RED);
                    printf("\n\tInvalid email format. Please try again.");
                    set_color(COLOR_RESET);
                } else {
                    strcpy(s.email, new_email);
                    break;
                }
            } while(1);
            break;
        }
            
        case 6:
            strcpy(old_value, s.guardian_name);
            printf("\n\tEnter new guardian name: ");
            fflush(stdin);
            safe_input(s.guardian_name, MAX_NAME);
            break;
            
        case 7: {
            strcpy(old_value, s.guardian_phone);
            char new_phone[MAX_PHONE];
            do {
                printf("\n\tEnter new guardian phone number: ");
                fflush(stdin);
                safe_input(new_phone, MAX_PHONE);
                
                if(!validate_phone(new_phone)) {
                    set_color(COLOR_RED);
                    printf("\n\tInvalid phone number. Please enter only digits.");
                    set_color(COLOR_RESET);
                } else {
                    strcpy(s.guardian_phone, new_phone);
                    break;
                }
            } while(1);
            break;
        }
            
        case 8:
            if(s.has_taken_exam) {
                char old_mark[10];
                sprintf(old_mark, "%.2f", s.mark);
                strcpy(old_value, old_mark);
                
                do {
                    printf("\n\tEnter new exam score (0.00-5.00): ");
                    float new_mark;
                    if(scanf("%f", &new_mark) != 1) {
                        printf("\n\tInvalid input! Please enter a number.");
                        fflush(stdin);
                        continue;
                    }
                    
                    if(new_mark < 0 || new_mark > 5) {
                        printf("\n\tScore must be between 0.00 and 5.00!");
                    } else {
                        s.mark = new_mark;
                        s.has_taken_exam = 1;
                        break;
                    }
                } while(1);
            } else {
                printf("\n\tThis student hasn't taken the exam yet!");
                printf("\n\tUse the 'Manage Entrance Exam' option to record exam results.");
                printf("\n\n\tPress any key to return...");
                getch();
                return;
            }
            break;
            
        default:
            printf("\n\tInvalid choice!");
            getch();
            return;
    }
    
    // Save the changes back to the file
    fseek(fp, pos, SEEK_SET);
    if(fwrite(&s, sizeof(s), 1, fp) != 1) {
        printf("\n\tError saving changes!");
        log_message(LOG_ERROR, "Failed to update student record in file");
    } else {
        fflush(fp);
        
        // Log the modification
        const char *field_names[] = {
            "", "Name", "Date of Birth", "Address", "Phone", 
            "Email", "Guardian Name", "Guardian Phone", "Exam Score"
        };
        
        // For non-string fields, format the old value
        if(choice == 8) {
            char formatted_old[20];
            sprintf(formatted_old, "%.2f", atof(old_value));
            strcpy(old_value, formatted_old);
        }
        
        sprintf(log_msg, "Modified student ID %d - %s changed from '%s' to '%s'", 
                s.ID, field_names[choice], old_value, 
                (choice == 8) ? "" : (choice == 1) ? s.name : 
                (choice == 2) ? s.dob : (choice == 3) ? s.address : 
                (choice == 4) ? s.phone : (choice == 5) ? s.email : 
                (choice == 6) ? s.guardian_name : s.guardian_phone);
        
        log_message(LOG_INFO, log_msg);
        
        set_color(COLOR_GREEN);
        printf("\n\n\tRecord updated successfully!");
        set_color(COLOR_RESET);
    }
    
    printf("\n\n\tPress any key to return...");
    getch();
}

void search_student(FILE *fp) {
    char search_term[100];
    int search_id, found = 0, choice;
    Student s;
    
    do {
        print_header();
        printf("\n\n\t\tSEARCH STUDENT");
        printf("\n\t\t--------------\n");
        printf("\n\t1. Search by ID");
        printf("\n\t2. Search by Name");
        printf("\n\t3. Back to Menu");
        printf("\n\n\tEnter your choice: ");
        
        scanf("%d", &choice);
        fflush(stdin);
        
        if (choice == 3) return;
        
        if (choice != 1 && choice != 2) {
            printf("\n\tInvalid choice! Please try again.");
            getch();
            continue;
        }
        
        printf("\n\tEnter %s: ", (choice == 1) ? "ID" : "Name");
        
        if (choice == 1) {
            if (scanf("%d", &search_id) != 1) {
                printf("\n\tInvalid ID format!");
                fflush(stdin);
                getch();
                continue;
            }
            fflush(stdin);
        } else {
            safe_input(search_term, sizeof(search_term));
            // Convert search term to lowercase for case-insensitive search
            for(int i = 0; search_term[i]; i++) {
                search_term[i] = tolower(search_term[i]);
            }
        }
        
        rewind(fp);
        found = 0;
        
        printf("\n\t");
        print_char('=', 80);
        printf("\n\t%-8s %-30s %-15s %-25s %-10s %-6s %-12s", 
               "ID", "Name", "Phone", "Email", "Exam", "Score", "Reg. Date");
        printf("\n\t");
        print_char('=', 80);
        
        while(fread(&s, sizeof(s), 1, fp) == 1) {
            int match = 0;
            
            if (choice == 1) {
                if (s.ID == search_id) {
                    match = 1;
                }
            } else {
                // Check if search term is in name (case insensitive)
                char lower_name[100];
                strncpy(lower_name, s.name, sizeof(lower_name));
                for(int i = 0; lower_name[i]; i++) {
                    lower_name[i] = tolower(lower_name[i]);
                }
                
                if (strstr(lower_name, search_term) != NULL || 
                    fuzzy_match(lower_name, search_term)) {
                    match = 1;
                }
            }
            
            if (match) {
                found = 1;
                char exam_status[20];
                char reg_date[12];
                
                if(s.has_taken_exam) {
                    if(s.mark >= 3.0) {
                        set_color(COLOR_GREEN);
                        strcpy(exam_status, "PASSED");
                    } else {
                        set_color(COLOR_RED);
                        strcpy(exam_status, "FAILED");
                    }
                } else {
                    set_color(COLOR_YELLOW);
                    strcpy(exam_status, "NOT TAKEN");
                }
                
                // Format registration date
                if(s.registration_date > 0) {
                    struct tm *tm_info = localtime(&s.registration_date);
                    strftime(reg_date, 12, "%Y-%m-%d", tm_info);
                } else {
                    strcpy(reg_date, "N/A");
                }
                
                // Truncate long fields for display
                char display_name[30];
                strncpy(display_name, s.name, 27);
                if(strlen(s.name) > 27) {
                    strcpy(display_name + 27, "...");
                }
                
                char display_email[30];
                strncpy(display_email, s.email, 25);
                if(strlen(s.email) > 25) {
                    strcpy(display_email + 25, "...");
                }
                
                printf("\n\t%-8d %-30s %-15s %-25s %-10s %-6.2f %-12s", 
                       s.ID, display_name, s.phone, display_email, 
                       exam_status, s.mark, reg_date);
                
                // Show more details if only one match
                if (choice == 1 || (choice == 2 && found == 1)) {
                    printf("\n\n\tAdditional Details:");
                    printf("\n\t-----------------");
                    printf("\n\tDate of Birth: %s", s.dob);
                    printf("\n\tAddress: %s", s.address);
                    printf("\n\tGuardian: %s (%s)", s.guardian_name, s.guardian_phone);
                    
                    if (s.has_taken_exam) {
                        printf("\n\n\tExam Details:");
                        printf("\n\t------------");
                        printf("\n\tScore: %.2f/5.00", s.mark);
                        printf("\n\tResult: %s", (s.mark >= 3.0) ? "PASSED" : "FAILED");
                        
                        if (s.mark >= 4.5) {
                            printf(" - Excellent!");
                        } else if (s.mark >= 4.0) {
                            printf(" - Very Good!");
                        } else if (s.mark >= 3.0) {
                            printf(" - Good!");
                        }
                    }
                }
                
                set_color(COLOR_RESET);
                printf("\n\t");
                print_char('-', 80);
                
                // If searching by ID, we can break after finding the match
                if (choice == 1) break;
            }
        }
        
        if (!found) {
            printf("\n\tNo matching records found!");
        }
        
        printf("\n\n\tPress any key to continue...");
        getch();
        
    } while (1);
}

void display_all_students(FILE *fp) {
    print_header();
    
    set_color(COLOR_BLUE);
    printf("\n\n\t\tSTUDENT RECORDS");
    set_color(COLOR_RESET);
    printf("\n\t\t----------------\n");

    Student s;
    int count = 0, page = 1, records_per_page = 10;
    long current_pos;
    char choice;

    do {
        rewind(fp);
        count = 0;
        int record_count = 0;
        
        // Count total records first
        while(fread(&s, sizeof(s), 1, fp) == 1) {
            record_count++;
        }
        
        if(record_count == 0) {
            printf("\n\t\tNo student records found!");
            printf("\n\n\t\tPress any key to return...");
            getch();
            return;
        }
        
        rewind(fp);
        
        // Calculate total pages
        int total_pages = (record_count + records_per_page - 1) / records_per_page;
        
        // Validate page number
        if(page < 1) page = 1;
        if(page > total_pages) page = total_pages;
        
        // Skip records from previous pages
        int skip = (page - 1) * records_per_page;
        while(skip-- > 0 && fread(&s, sizeof(s), 1, fp) == 1);
        
        // Display header
        printf("\n\t%-8s %-30s %-15s %-30s %-15s %-6s %-10s", 
               "ID", "Name", "Phone", "Email", "Exam Status", "Score", "Reg. Date");
        printf("\n\t");
        print_char('-', 100);
        
        // Display records for current page
        while(count < records_per_page && fread(&s, sizeof(s), 1, fp) == 1) {
            char exam_status[20];
            char reg_date[12];
            
            if(s.has_taken_exam) {
                if(s.mark >= 3.0) {
                    set_color(COLOR_GREEN);
                    strcpy(exam_status, "PASSED");
                } else {
                    set_color(COLOR_RED);
                    strcpy(exam_status, "FAILED");
                }
            } else {
                set_color(COLOR_YELLOW);
                strcpy(exam_status, "NOT TAKEN");
            }
            
            // Format registration date
            if(s.registration_date > 0) {
                struct tm *tm_info = localtime(&s.registration_date);
                strftime(reg_date, 12, "%Y-%m-%d", tm_info);
            } else {
                strcpy(reg_date, "N/A");
            }
            
            // Truncate long fields for display
            char display_name[30];
            strncpy(display_name, s.name, 27);
            if(strlen(s.name) > 27) {
                strcpy(display_name + 27, "...");
            }
            
            char display_email[30];
            strncpy(display_email, s.email, 27);
            if(strlen(s.email) > 27) {
                strcpy(display_email + 27, "...");
            }
            
            printf("\n\t%-8d %-30s %-15s %-30s %-15s %-6.2f %-10s", 
                   s.ID, display_name, s.phone, display_email, 
                   exam_status, s.mark, reg_date);
            
            set_color(COLOR_RESET);
            count++;
        }
        
        // Display page info
        printf("\n\n\tPage %d of %d (Total: %d records)", page, total_pages, record_count);
        printf("\n\t[N]ext [P]revious [G]o to page [Q]uit: ");
        
        // Get user choice
        fflush(stdin);
        choice = tolower(getche());
        
        // Process navigation
        switch(choice) {
            case 'n':
                if(page < total_pages) page++;
                break;
            case 'p':
                if(page > 1) page--;
                break;
            case 'g':
                printf("\n\tEnter page number (1-%d): ", total_pages);
                int new_page;
                if(scanf("%d", &new_page) == 1 && new_page >= 1 && new_page <= total_pages) {
                    page = new_page;
                } else {
                    set_color(COLOR_RED);
                    printf("\n\tInvalid page number!");
                    set_color(COLOR_RESET);
                    getch();
                }
                fflush(stdin);
                break;
            case 'q':
                return;
            default:
                break;
        }
        
        print_header();
        set_color(COLOR_BLUE);
        printf("\n\n\t\tSTUDENT RECORDS");
        set_color(COLOR_RESET);
        printf("\n\t\t----------------\n");
        
    } while(choice != 'q');
}

void add_student(FILE *fp) {
    print_header();
    char another = 'y';
    Student s;
    char log_msg[200];

    // Guidelines for filling the form
    printf("\n\n\t");
    print_char('~', 85);
    set_color(COLOR_YELLOW);
    puts("\n\t\t*POINTS TO REMEMBER WHILE FILLING FORM");
    set_color(COLOR_RESET);
    puts("\n\t\ti.   One should pay proper attention while filling the form.");
    puts("\n\t\tii.  Phone number should contain only numeric digits.");
    puts("\n\t\tiii. You must remember your ID number for future reference.");
    puts("\n\t\tiv.  Records can be modified later if needed (additional fee applies).");
    puts("\n\t\tv.   ID number should be your last exam symbol number or citizenship");
    puts("\n\t\t     number without any special characters.");
    printf("\t\t");
    print_char('~', 85);
    printf("\n\n\n");
    getch();

    print_header();
    while(another == 'y' || another == 'Y') {
        // Clear any existing data in the struct
        memset(&s, 0, sizeof(Student));

        printf("\n\n\t\tEnter Full Name of Student:\t");
        fflush(stdin);
        safe_input(s.name, MAX_NAME);

        printf("\n\n\t\tEnter date of birth (YYYY-MM-DD):\t");
        fflush(stdin);
        safe_input(s.dob, MAX_DOB);

        printf("\n\n\t\tEnter Address:\t");
        fflush(stdin);
        safe_input(s.address, MAX_ADDRESS);

        do {
            printf("\n\n\t\tIdentification document no: \t");
            scanf("%d", &s.ID);

            // Check if ID already exists
            int exists = 0;
            Student temp;
            long current_pos = ftell(fp);

            rewind(fp);
            while(fread(&temp, sizeof(temp), 1, fp) == 1) {
                if(temp.ID == s.ID) {
                    exists = 1;
                    break;
                }
            }

            // Go back to our position in the file
            fseek(fp, current_pos, SEEK_SET);

            if(exists) {
                set_color(COLOR_RED);
                printf("\n\t\tThis ID already exists. Please enter a different ID.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        s.mark = 0;
        s.has_taken_exam = 0;
        s.has_user_account = 0;

        do {
            printf("\n\n\t\tEnter student phone number:\t");
            fflush(stdin);
            safe_input(s.phone, MAX_PHONE);

            if(!validate_phone(s.phone)) {
                set_color(COLOR_RED);
                printf("\n\t\tInvalid phone number. Please enter only digits.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        do {
            printf("\n\n\t\tEnter email ID of Student:\t");
            fflush(stdin);
            safe_input(s.email, MAX_EMAIL);

            if(!validate_email(s.email)) {
                set_color(COLOR_RED);
                printf("\n\t\tInvalid email format. Please try again.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        printf("\n\n\t\tEnter guardian name of Student:\t");
        fflush(stdin);
        safe_input(s.guardian_name, MAX_NAME);

        printf("\n\n\t\tEnter student's guardian phone number:\t");
        fflush(stdin);
        safe_input(s.guardian_phone, MAX_PHONE);

        // Record registration timestamp
        s.registration_date = time(NULL);
        
        // Generate QR code for the student
        generate_qr_code(&s);

        // Write to file
        fwrite(&s, sizeof(s), 1, fp);
        fflush(fp);

        // Log the registration
        sprintf(log_msg, "New student registered - ID: %d, Name: %s", s.ID, s.name);
        log_message(LOG_INFO, log_msg);

        // Ask if user wants to add another student
        set_color(COLOR_GREEN);
        printf("\n\n\t\tRegistration successful!");
        set_color(COLOR_RESET);
        printf("\n\n\t\tWant to enter another student info (Y/N)?\t");
        fflush(stdin);
        another = getche();

        if (another == 'y' || another == 'Y') {
            print_header();
        }
    }
}