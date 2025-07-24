#ifndef STUDENT_H
#define STUDENT_H

#include "common.h"

// Forward declaration
typedef struct Student Student;
typedef struct ExamResult ExamResult;

// Student structure
struct Student {
    int id;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    char address[200];
    char gender[10];
    char dob[20]; // Date of birth in YYYY-MM-DD format
    char parent_name[MAX_NAME];
    char parent_phone[MAX_PHONE];
    char parent_email[MAX_EMAIL];
    char parent_relation[50];
    char education[100];
    char school[100];
    char grade[10];
    char section[10];
    bool is_active;
    time_t created_at;
    time_t updated_at;
    int created_by; // User ID who created this student
};

// Function prototypes
// Student management
bool add_student(Student *student);
bool update_student(Student *student);
bool delete_student(int student_id);
Student* get_student(int student_id);
Student* get_student_by_username(const char *username);
Student* get_students(int *count);
Student* search_students(const char *query, int *count);

// Student exam related
bool register_student_for_exam(int student_id, int exam_id);
bool unregister_student_from_exam(int student_id, int exam_id);
bool has_student_taken_exam(int student_id, int exam_id);
ExamResult* get_student_exam_results(int student_id, int *count);
ExamResult* get_student_exam_result(int student_id, int exam_id);

// File operations
bool save_student(const Student *student);
bool load_student(int student_id, Student *student);
bool save_students(const Student *students, int count);
bool load_students(Student **students, int *count);

// Search and filters
Student* filter_students_by_grade(const char *grade, int *count);
Student* filter_students_by_section(const char *section, int *count);
Student* filter_students_by_status(bool is_active, int *count);

// Statistics
int get_total_students(void);
int get_active_students_count(void);
int get_inactive_students_count(void);

// User interface
void show_student_menu(void);
void show_student_details(const Student *student);
void show_student_list(const Student *students, int count);
void show_student_exam_results(int student_id);

// QR Code functions
bool generate_qr_code(const char *data, const char *filename);
bool verify_qr_code(const char *filename, const char *expected_data);
void generate_student_id_card(const Student *student);

// Import/Export
bool import_students_from_csv(const char *filename);
bool export_students_to_csv(const char *filename);
bool export_student_list_as_pdf(const char *filename);
void generate_student_report(int student_id);
void generate_class_report(void);

// User account management
bool create_student_account(int student_id);
bool deactivate_student_account(int student_id);
bool reset_student_password(int student_id);

// Helper functions
time_t parse_date(const char *date_str);
char* format_date(time_t timestamp);
int calculate_age(const char *dob);

#endif // STUDENT_H
