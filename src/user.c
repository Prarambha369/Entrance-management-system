#include "../include/user.h"
#include "../include/common.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// Global variables are declared extern in common.h and defined in main.c

bool login() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int attempts = 0;
    const int max_attempts = 3;
    
    while (attempts < max_attempts) {
        print_header();
        printf("\n\n\t\tLOGIN");
        printf("\n\t\t-----");
        
        if (attempts > 0) {
            set_color(COLOR_RED);
            printf("\n\t\tInvalid credentials. Attempt %d of %d", 
                   attempts, max_attempts);
            set_color(COLOR_RESET);
        }
        
        printf("\n\n\t\tUsername: ");
        safe_input(username, MAX_USERNAME);
        
        if (strcmp(username, "exit") == 0 || strcmp(username, "quit") == 0) {
            return false;
        }
        
        printf("\t\tPassword: ");
        secure_password_input(password, MAX_PASSWORD);
        
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
            log_message(LOG_INFO, "User logged in: %s", username);
            return true;
        }
        
        attempts++;
    }
    
    log_message(LOG_WARNING, "Max login attempts exceeded");
    return false;
}

void logout() {
    log_message(LOG_INFO, "User logged out: %s", current_user.username);
    is_logged_in = false;
    memset(&current_user, 0, sizeof(User));
}

void change_password() {
    char current_pass[MAX_PASSWORD];
    char new_pass[MAX_PASSWORD];
    char confirm_pass[MAX_PASSWORD];
    
    print_header();
    printf("\n\n\t\tCHANGE PASSWORD");
    printf("\n\t\t---------------");
    
    printf("\n\n\t\tCurrent Password: ");
    secure_password_input(current_pass, MAX_PASSWORD);
    
    if (strcmp(current_pass, current_user.password) != 0) {
        set_color(COLOR_RED);
        printf("\n\t\tIncorrect current password!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }
    
    printf("\n\t\tNew Password (min 4 chars): ");
    secure_password_input(new_pass, MAX_PASSWORD);
    
    printf("\n\t\tConfirm New Password: ");
    secure_password_input(confirm_pass, MAX_PASSWORD);
    
    if (strcmp(new_pass, confirm_pass) != 0) {
        set_color(COLOR_RED);
        printf("\n\t\tPasswords do not match!");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    if (strlen(new_pass) < 4) {
        set_color(COLOR_RED);
        printf("\n\t\tPassword too short! Must be at least 4 characters.");
        set_color(COLOR_RESET);
        printf("\n\t\tPress any key to return...");
        getch();
        return;
    }

    // Update password in file
    FILE *user_fp = safe_open(USER_FILE, "r+b");
    if (user_fp) {
        User temp;
        while (fread(&temp, sizeof(User), 1, user_fp) == 1) {
            if (strcmp(temp.username, current_user.username) == 0) {
                fseek(user_fp, -sizeof(User), SEEK_CUR);
                strcpy(temp.password, new_pass);
                fwrite(&temp, sizeof(User), 1, user_fp);
                strcpy(current_user.password, new_pass);
                break;
            }
        }
        fclose(user_fp);
        
        log_message(LOG_INFO, "Password changed for user: %s", current_user.username);
        
        set_color(COLOR_GREEN);
        printf("\n\t\tPassword changed successfully!");
        set_color(COLOR_RESET);
    } else {
        set_color(COLOR_RED);
        printf("\n\t\tError updating password!");
        set_color(COLOR_RESET);
    }
    
    printf("\n\t\tPress any key to continue...");
    getch();
}

void manage_users(void) {
    print_header();
    printf("\n\n\t\tUSER MANAGEMENT");
    printf("\n\t\t--------------");

    printf("\n\n\t\t1. Add New User");
    printf("\n\t\t2. List Users");
    printf("\n\t\t3. Edit User");
    printf("\n\t\t4. Delete User");
    printf("\n\t\t0. Back");

    int choice = get_menu_choice(0, 4);

    switch (choice) {
        case 1:
            // TODO: Implement add_new_user()
            printf("\n\t\tAdding new user...");
            break;
        case 2:
            // TODO: Implement list_users()
            printf("\n\t\tListing users...");
            break;
        case 3:
            // TODO: Implement edit_user()
            printf("\n\t\tEditing user...");
            break;
        case 4:
            // TODO: Implement delete_user()
            printf("\n\t\tDeleting user...");
            break;
    }
}

void view_logs(void) {
    print_header();
    printf("\n\n\t\tSYSTEM LOGS");
    printf("\n\t\t-----------");

    FILE *fp = safe_open(LOG_FILE, "r");
    if (!fp) {
        printf("\n\t\tNo logs found.");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("\n\t\t%s", line);
    }

    fclose(fp);
}
