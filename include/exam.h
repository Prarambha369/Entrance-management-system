#ifndef EXAM_H
#define EXAM_H

#include "common.h"

// Question structure for exams
typedef struct Question {
    char question_text[500];
    char options[4][100];
    char correct_answer;
    int difficulty;
    char category[50];
} Question;

// Function prototypes
void conduct_exam(FILE *fp);
void load_questions(Question questions[]);
void shuffle_questions(int order[], int count);

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
