#define MAIN_FILE
#include "../include/common.h"
#include "../include/user.h"
#include "../include/student.h"
#include "../include/exam.h"
#include "../include/input_utils.h"

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
    
    // Check if this is first run by looking for user file
    if (!FILE_EXISTS(USER_FILE)) {
        show_documentation();
        config.first_run = false;
        save_system_config(config);
        log_message(LOG_INFO, "First-time setup completed");
        printf("\n\n\t\tSetup complete! Please restart the application to login.");
        printf("\n\t\tPress any key to exit...");
        getch();
        return 0;
    }
    
    // Main application loop
    while (1) {
        if (!is_logged_in) {
            if (!login()) {
                log_message(LOG_INFO, "Login failed, exiting application");
                break;
            }
        }
        
        // Single menu system for all user types
        if (is_logged_in) {
            user_panel();  // This now handles all user types with proper access control
        }
    }
    
    log_message(LOG_INFO, "Application exiting");
    return 0;
}
