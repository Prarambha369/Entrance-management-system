#define MAIN_FILE
#include "../include/common.h"
#include "../include/user.h"
#include "../include/student.h"
#include "../include/exam.h"

// Global variables are now declared in common.h with extern
// and defined here with the MAIN_FILE macro
User current_user = {0};
bool is_logged_in = false;

int main() {
    // Initialize system directories
    ensure_dir_exists("data");
    ensure_dir_exists("backups");
    ensure_dir_exists("logs");
    
    // Initialize logging system
    init_log_system();
    log_message(LOG_INFO, "Application started");
    
    // Load system configuration
    SystemConfig config = load_system_config();
    
    // First run setup
    if (config.first_run) {
        show_documentation();
        config.first_run = false;
        save_system_config(config);
    }
    
    // Main application loop
    while (1) {
        if (!is_logged_in) {
            if (!login()) {
                log_message(LOG_INFO, "Login failed, exiting application");
                break;
            }
        }
        
        // Route to appropriate panel based on user role
        if (is_logged_in) {
            switch (current_user.role) {
                case ROLE_ADMIN:
                    admin_panel();
                    break;
                case ROLE_EXAMINER:
                    // examiner_panel();
                    break;
                case ROLE_USER:
                    user_panel();
                    break;
                default:
                    log_message(LOG_ERROR, "Unknown user role: %d", current_user.role);
                    is_logged_in = false;
                    break;
            }
        }
    }
    
    log_message(LOG_INFO, "Application exiting");
    return 0;
}
