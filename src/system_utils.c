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
    printf("\n\t\tA default admin account has been created:");
    printf("\n\n\t\tUsername: admin");
    printf("\n\t\tPassword: admin123");
    
    printf("\n\n\t\tPlease change the default password after login.");
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
