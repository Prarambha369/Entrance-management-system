#include "../include/common.h"
#include "../include/user.h"
#include "../include/student.h"
#include "../include/exam.h"

void show_main_menu(void) {
    print_header();
    printf("\n\t\tMAIN MENU");
    printf("\n\t\t---------\n");

    printf("\n\t\tlogin          | Logout and login as different user");
    printf("\n\t\tadd-user       | Add a new user account");
    printf("\n\t\tadd-student    | Register a new student");
    printf("\n\t\tlist-students  | View all students");
    printf("\n\t\tsearch-student | Search for a student");
    printf("\n\t\tstart-exam     | Begin your entrance exam (students only)");
    printf("\n\t\tview-results   | View your exam results");
    printf("\n\t\trankings       | See student rankings");
    printf("\n\t\tbackup-csv     | Export data to CSV");
    printf("\n\t\tbackup-binary  | Create a binary backup");
    printf("\n\t\tgenerate-report| Generate exam report");
    printf("\n\t\tview-logs      | View system logs");
    printf("\n\t\tsystem-settings| Change system configuration");
    printf("\n\t\thelp           | Show help");
    printf("\n\t\tabout          | About this system");
    printf("\n\t\tlogout         | Logout");
    printf("\n\t\texit           | Exit the program");

    printf("\n\n\t\tEnter command: ");
}

void handle_menu_command(const char *cmd) {
    if (strcmp(cmd, "login") == 0) {
        logout();
        if (!login()) exit(0);
    }
    else if (strcmp(cmd, "add-user") == 0) {
        if (current_user.role != ROLE_ADMIN) {
            printf("\n\t\tAccess denied. Admin privileges required.");
            return;
        }
        manage_users();
    }
    else if (strcmp(cmd, "add-student") == 0) {
        if (current_user.role != ROLE_ADMIN && current_user.role != ROLE_EXAMINER) {
            printf("\n\t\tAccess denied. Staff privileges required.");
            return;
        }
        Student new_student = {0};
        add_student(&new_student);
    }
    else if (strcmp(cmd, "list-students") == 0) {
        // TODO: Implement list_students()
        printf("\n\t\tListing students...");
    }
    else if (strcmp(cmd, "search-student") == 0) {
        // TODO: Implement search functionality
        printf("\n\t\tSearching for student...");
    }
    else if (strcmp(cmd, "start-exam") == 0) {
        if (current_user.role != ROLE_USER) {
            printf("\n\t\tAccess denied. Student access only.");
            return;
        }
        // TODO: Implement start_exam()
        printf("\n\t\tStarting exam...");
    }
    else if (strcmp(cmd, "view-results") == 0) {
        // TODO: Implement view_results()
        printf("\n\t\tViewing results...");
    }
    else if (strcmp(cmd, "rankings") == 0) {
        // TODO: Implement show_rankings()
        printf("\n\t\tShowing rankings...");
    }
    else if (strcmp(cmd, "backup-csv") == 0) {
        if (current_user.role != ROLE_ADMIN) {
            printf("\n\t\tAccess denied. Admin privileges required.");
            return;
        }
        // TODO: Implement export_to_csv()
        printf("\n\t\tExporting to CSV...");
    }
    else if (strcmp(cmd, "backup-binary") == 0) {
        if (current_user.role != ROLE_ADMIN) {
            printf("\n\t\tAccess denied. Admin privileges required.");
            return;
        }
        // TODO: Implement create_binary_backup()
        printf("\n\t\tCreating binary backup...");
    }
    else if (strcmp(cmd, "generate-report") == 0) {
        if (current_user.role != ROLE_ADMIN && current_user.role != ROLE_EXAMINER) {
            printf("\n\t\tAccess denied. Staff privileges required.");
            return;
        }
        // TODO: Implement generate_report()
        printf("\n\t\tGenerating report...");
    }
    else if (strcmp(cmd, "view-logs") == 0) {
        if (current_user.role != ROLE_ADMIN) {
            printf("\n\t\tAccess denied. Admin privileges required.");
            return;
        }
        view_logs();
    }
    else if (strcmp(cmd, "system-settings") == 0) {
        if (current_user.role != ROLE_ADMIN) {
            printf("\n\t\tAccess denied. Admin privileges required.");
            return;
        }
        // TODO: Implement system_settings()
        printf("\n\t\tAccessing system settings...");
    }
    else if (strcmp(cmd, "help") == 0) {
        show_documentation();
    }
    else if (strcmp(cmd, "about") == 0) {
        print_header();
        printf("\n\t\tEntrance Management System");
        printf("\n\t\tVersion %s", VERSION);
        printf("\n\t\tDeveloped by: Your Name");
        printf("\n\t\tLicense: MIT");
    }
    else if (strcmp(cmd, "logout") == 0) {
        logout();
    }
    else if (strcmp(cmd, "exit") == 0) {
        printf("\n\t\tExiting system...");
        exit(0);
    }
    else {
        printf("\n\t\tUnknown command. Type 'help' for available commands.");
    }

    printf("\n\t\tPress any key to continue...");
    getch();
}

void admin_panel(void) {
    char cmd[32];
    while (is_logged_in) {
        show_main_menu();
        safe_input(cmd, sizeof(cmd));
        handle_menu_command(cmd);
    }
}

void user_panel(void) {
    char cmd[32];
    while (is_logged_in) {
        show_main_menu();
        safe_input(cmd, sizeof(cmd));
        handle_menu_command(cmd);
    }
}
