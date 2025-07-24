#include "../include/common.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void ensure_dir_exists(const char *dir) {
    if (!FILE_EXISTS(dir)) {
        MKDIR(dir);
    }
}

SystemConfig load_system_config() {
    SystemConfig config = {0};
    FILE *fp = safe_open(CONFIG_FILE, "rb");
    if (fp) {
        fread(&config, sizeof(SystemConfig), 1, fp);
        fclose(fp);
    } else {
        // Default configuration
        config.first_run = true;
        strcpy(config.system_name, "Entrance Management System");
        strcpy(config.institution_name, "Your Institution");
        strcpy(config.institution_address, "123 Education St.");
        strcpy(config.contact_email, "admin@example.com");
        strcpy(config.contact_phone, "+1 234 567 8900");
        config.setup_date = time(NULL);
    }
    return config;
}

void save_system_config(SystemConfig config) {
    FILE *fp = safe_open(CONFIG_FILE, "wb");
    if (fp) {
        fwrite(&config, sizeof(SystemConfig), 1, fp);
        fclose(fp);
    }
}

void show_documentation() {
    print_header();
    printf("\n\n\t\tWELCOME TO ENTRANCE MANAGEMENT SYSTEM");
    printf("\n\t\t------------------------------------");
    
    printf("\n\n\t\tThis is your first time running the system.");
    printf("\n\t\tLet's set up your administrator account.");

    User admin = {0};
    admin.role = ROLE_ADMIN;
    admin.active = true;
    admin.created_at = time(NULL);
    admin.last_login = admin.created_at;

    printf("\n\n\t\tADMINISTRATOR ACCOUNT SETUP");
    printf("\n\t\t--------------------------");

    do {
        printf("\n\n\t\tFull Name: ");
        safe_input(admin.name, sizeof(admin.name));
    } while (strlen(admin.name) < 3);

    do {
        printf("\t\tUsername (min 4 chars): ");
        safe_input(admin.username, sizeof(admin.username));
    } while (strlen(admin.username) < 4);

    do {
        printf("\t\tEmail: ");
        safe_input(admin.email, sizeof(admin.email));
    } while (!validate_email(admin.email));

    do {
        printf("\t\tPhone: ");
        safe_input(admin.phone, sizeof(admin.phone));
    } while (!validate_phone(admin.phone));

    bool password_set = false;
    do {
        char confirm_pass[MAX_PASSWORD];
        printf("\n\t\tEnter Password (min 4 chars): ");
        secure_password_input(admin.password, sizeof(admin.password));

        printf("\t\tConfirm Password: ");
        secure_password_input(confirm_pass, sizeof(confirm_pass));

        if (strcmp(admin.password, confirm_pass) != 0) {
            set_color(COLOR_RED);
            printf("\n\t\tPasswords do not match! Try again.");
            set_color(COLOR_RESET);
            continue;
        }

        if (strlen(admin.password) < 4) {
            set_color(COLOR_RED);
            printf("\n\t\tPassword too short! Must be at least 4 characters.");
            set_color(COLOR_RESET);
            continue;
        }

        password_set = true;
    } while (!password_set);

    // Save admin account
    FILE *fp = safe_open(USER_FILE, "wb");
    if (fp) {
        admin.ID = 1;  // First user gets ID 1
        if (fwrite(&admin, sizeof(User), 1, fp) == 1) {
            set_color(COLOR_GREEN);
            printf("\n\n\t\tAdministrator account created successfully!");
            set_color(COLOR_RESET);
        } else {
            set_color(COLOR_RED);
            printf("\n\n\t\tError creating administrator account!");
            set_color(COLOR_RESET);
        }
        fclose(fp);
    }

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

bool authenticate_user(const char *username, const char *password, User *user) {
    FILE *fp = safe_open(USER_FILE, "rb");
    if (!fp) return false;
    
    User temp;
    bool found = false;
    
    while (fread(&temp, sizeof(User), 1, fp) == 1) {
        if (strcmp(temp.username, username) == 0 && 
            strcmp(temp.password, password) == 0 &&
            temp.active) {
            *user = temp;
            found = true;
            break;
        }
    }
    
    fclose(fp);
    return found;
}

char* get_role_name(int role) {
    switch (role) {
        case ROLE_ADMIN: return "Administrator";
        case ROLE_EXAMINER: return "Examiner";
        case ROLE_USER: return "Student";
        default: return "Unknown";
    }
}
