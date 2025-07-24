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
// Global variables
extern User current_user;
extern bool is_logged_in;

// Function prototypes for user management
bool login(void);
void logout(void);
void change_password(void);
void manage_users(void);

// User-related functions
bool user_exists(const char *username);
bool register_user(const char *username, const char *password, const char *name, 
                  const char *email, const char *phone, int role);
bool update_user(User *user);
bool delete_user(int user_id);
User* find_user_by_id(int user_id);
User* find_user_by_username(const char *username);
void list_users(int role_filter);

// Password functions
bool change_user_password(int user_id, const char *new_password);
bool reset_password(int user_id);
bool verify_password(const char *username, const char *password);

// Account status
bool activate_user(int user_id);
bool deactivate_user(int user_id);
bool is_account_locked(const char *username);

// Session management
void update_last_login(const char *username);
void lock_account(const char *username);
void unlock_account(const char *username);

// User input functions
void input_user_details(User *user, bool is_new);
void display_user_details(const User *user);

// File operations
bool save_user(const User *user);
bool load_users(User *users, int *count);
bool save_users(const User *users, int count);

// Helper functions
bool is_username_available(const char *username);
bool is_email_used(const char *email, int exclude_user_id);
bool is_phone_used(const char *phone, int exclude_user_id);

// Role management
bool has_permission(int required_role, int user_role);

// User interface
void show_user_menu(void);
void show_admin_user_menu(void);
void show_examiner_user_menu(void);
void show_student_user_menu(void);

// Panel functions
void admin_panel(void);
void admin_system_settings(void);
void admin_reports(void);
void user_panel(void);
void user_profile(void);
void user_exam(void);
void user_results(void);

void user_results();

#endif // USER_H
