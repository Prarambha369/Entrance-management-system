#include "../include/common.h"
#include "../include/user.h"
#include "../include/student.h"
#include "../include/exam.h"

// Global variables
User current_user;
bool is_logged_in = false;

int main() {
    // Initialize system directories and logging
    ensure_dir_exists("data");
    ensure_dir_exists("backups");
    ensure_dir_exists("reports");
    
    // Initialize logging system
    init_log_system();
    log_message(LOG_INFO, "System starting...");

    // Show documentation on first run
    SystemConfig config = load_system_config();
    if (config.first_run) {
        show_documentation();
        config.first_run = false;
        save_system_config(config);
    }

    // Main program loop
    while (1) {
        if (!is_logged_in) {
            if (!login()) {
                log_message(LOG_INFO, "User canceled login. Exiting program.");
                break;
            }
        }
        
        // Route to appropriate panel based on user role
        if (current_user.role == ROLE_ADMIN) {
            admin_panel();
        } else {
            user_panel();
        }
    }

    log_message(LOG_INFO, "System shutdown");
    return 0;
}
