#ifndef USER_H
#define USER_H

#include "common.h"

// User account structure
typedef struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char full_name[MAX_NAME];
    char email[MAX_EMAIL];
    int role;
    int student_id;
    time_t created_date;
    time_t last_login;
    bool active;
} User;

// System configuration
typedef struct SystemConfig {
    bool first_run;
    char system_name[100];
    char institution_name[100];
    char institution_address[100];
    char contact_email[100];
    char contact_phone[50];
    time_t setup_date;
} SystemConfig;

// Global variables
extern User current_user;
extern bool is_logged_in;

// Function prototypes
bool login();
void logout();
void change_password();
void create_user_account(int role);
void user_management();
bool authenticate_user(const char *username, const char *password, User *user);

// System configuration functions
SystemConfig load_system_config();
void save_system_config(SystemConfig config);

// Admin panel functions
void admin_panel();
void admin_system_settings();
void admin_reports();

// User panel functions
void user_panel();
void user_profile();
void user_exam();
void user_results();

#endif // USER_H
