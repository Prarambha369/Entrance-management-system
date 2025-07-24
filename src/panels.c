#include "../include/common.h"
#include "../include/user.h"
#include "../include/student.h"
#include "../include/exam.h"

void admin_panel() {
    while (is_logged_in && current_user.role == ROLE_ADMIN) {
        print_header();
        printf("\n\n\t\tADMIN PANEL");
        printf("\n\t\t-----------");
        
        printf("\n\n\t\t1. Manage Users");
        printf("\n\t\t2. Manage Students");
        printf("\n\t\t3. System Configuration");
        printf("\n\t\t4. Reports");
        printf("\n\t\t5. Backup/Restore");
        printf("\n\t\t6. View Logs");
        printf("\n\t\t0. Logout");
        
        int choice = get_menu_choice(0, 6);
        
        switch (choice) {
            case 0:
                return;
            case 1:
                // Manage Users
                break;
            case 2:
                // Manage Students
                break;
            case 3:
                // System Configuration
                break;
            case 4:
                // Reports
                break;
            case 5:
                // Backup/Restore
                break;
            case 6:
                // View Logs
                break;
        }
    }
}

void user_panel() {
    while (is_logged_in) {
        print_header();
        printf("\n\n\t\tUSER PANEL");
        printf("\n\t\t----------");
        
        printf("\n\n\t\t1. Take Exam");
        printf("\n\t\t2. View Results");
        printf("\n\t\t3. My Profile");
        printf("\n\t\t0. Logout");
        
        int choice = get_menu_choice(0, 3);
        
        switch (choice) {
            case 0:
                return;
            case 1:
                // Take Exam
                break;
            case 2:
                // View Results
                break;
            case 3:
                // My Profile
                break;
        }
    }
}
