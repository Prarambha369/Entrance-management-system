# Entrance Management System

A comprehensive entrance examination management system for educational institutions.

## Features

- User authentication (Admin, Examiner, Student roles)
- Student registration and management
- Entrance exam administration
- QR code check-in system
- Reporting and statistics
- Backup and restore functionality

## Requirements

- C Compiler (GCC recommended)
- Make
- Linux/Unix or Windows with MinGW

## Installation

1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd Entrance-management-system
   ```

2. Build the project:
   ```bash
   make
   ```

3. Run the application:
   ```bash
   ./ems
   ```

## Default Credentials

- **Admin**:
  - Username: admin
  - Password: admin123

## Project Structure

```
Entrance-management-system/
├── include/           # Header files
│   ├── common.h       # Common includes and definitions
│   ├── student.h      # Student management
│   ├── user.h         # User authentication
│   └── exam.h         # Exam functionality
├── src/               # Source files
│   ├── main.c         # Entry point
│   ├── student.c      # Student operations
│   ├── user.c         # User management
│   ├── exam.c         # Exam logic
│   └── utils.c        # Utility functions
├── data/              # Data storage
├── backups/           # Backup files
├── reports/           # Generated reports
└── Makefile           # Build configuration
Enter ID: 123
Record added successfully!
```

## Available Commands

The following commands are available in the main menu:

Command          | Description                           | Required Role
----------------|---------------------------------------|---------------
login           | Logout and login as different user    | Any
add-user        | Add a new user account               | Admin
add-student     | Register a new student               | Admin/Examiner
list-students   | View all students                    | Any
search-student  | Search for a student                 | Any
start-exam      | Begin your entrance exam             | Student
view-results    | View your exam results               | Any
rankings        | See student rankings                 | Any
backup-csv      | Export data to CSV                   | Admin
backup-binary   | Create a binary backup               | Admin
generate-report | Generate exam report                 | Admin/Examiner
view-logs       | View system logs                     | Admin
system-settings | Change system configuration          | Admin
help            | Show help                            | Any
about           | About this system                    | Any
logout          | Logout                               | Any
exit           | Exit the program                      | Any

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request. For major changes, open an issue first to discuss your ideas.

## License
See [LICENSE](LICENSE) for details.

## Author
Maintained by mrbashyal. For questions, contact via GitHub.
