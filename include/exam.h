#ifndef EXAM_H
#define EXAM_H

#include "common.h"

// Forward declarations
typedef struct Question Question;
typedef struct Exam Exam;
typedef struct ExamResult ExamResult;

// Question structure
struct Question {
    int id;
    char question_text[500];
    char options[4][200];
    int correct_option;
    int marks;
    char subject[50];
    char difficulty[20];
    int used_count;
    time_t created_at;
    time_t last_used;
};

// Exam structure
struct Exam {
    int id;
    char title[100];
    char subject[50];
    int duration; // in minutes
    int total_marks;
    int pass_marks;
    time_t start_time;
    time_t end_time;
    bool is_active;
    int question_count;
    Question questions[100]; // Maximum 100 questions per exam
};

// Exam result structure
struct ExamResult {
    int id;
    int user_id;
    int exam_id;
    int total_marks;
    int obtained_marks;
    float percentage;
    char grade[3];
    bool passed;
    time_t attempt_time;
    time_t completion_time;
    int time_taken; // in seconds
    int correct_answers;
    int wrong_answers;
    int not_attempted;
};

// Function prototypes
// Exam management
bool create_exam(Exam *exam);
bool update_exam(Exam *exam);
bool delete_exam(int exam_id);
Exam* get_exam(int exam_id);
Exam* get_active_exams(int *count);
bool activate_exam(int exam_id);
bool deactivate_exam(int exam_id);

// Question management
bool add_question(Question *question);
bool update_question(Question *question);
bool delete_question(int question_id);
Question* get_question(int question_id);
Question* get_questions_by_subject(const char *subject, int *count);
Question* get_random_questions(const char *subject, int difficulty, int count);

// Exam taking
bool start_exam(int user_id, int exam_id);
bool submit_answer(int attempt_id, int question_id, int selected_option);
bool submit_exam(int attempt_id);
ExamResult* calculate_result(int attempt_id);

// Results and reports
ExamResult* get_exam_results(int exam_id, int *count);
ExamResult* get_user_results(int user_id, int *count);
ExamResult* get_result(int result_id);
bool generate_exam_report(int exam_id, const char *filename);
bool generate_user_report(int user_id, const char *filename);
int get_questions_by_category(const char *category);
int get_questions_by_difficulty(int difficulty);
float get_average_question_difficulty(void);
int get_total_exams(void);
int get_exams_by_status(bool is_active);
int get_total_exam_results(void);
float get_average_exam_score(int exam_id);
float get_exam_pass_rate(int exam_id);

// Import/Export
bool import_questions_from_csv(const char *filename);
bool export_questions_to_csv(const char *filename);
bool export_exam_results_to_csv(int exam_id, const char *filename);

// Helper functions
time_t parse_exam_time(const char *time_str);
char* format_exam_time(time_t timestamp);
int calculate_exam_duration(time_t start_time, time_t end_time);
bool is_exam_available(int exam_id);
int count_questions_in_exam(int exam_id);

// Backup and recovery
void backup_exam_data(void);
bool restore_exam_data(const char *backup_file);

// Logging
void log_exam_event(int student_id, int exam_id, const char *event_type, const char *details);
void view_exam_logs(int exam_id);
void view_student_exam_logs(int student_id);

// Backup and report functions
void backup_menu();
void create_binary_backup();
void export_csv_backup();
void restore_backup();
void generate_report();

// Logging functions
void init_log_system();
void log_message(int level, const char *message);
void view_logs();

#endif // EXAM_H
