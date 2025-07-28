#ifndef ENTRANCE_MANAGEMENT_SYSTEM_EXAM_H
#define ENTRANCE_MANAGEMENT_SYSTEM_EXAM_H

#include <stdbool.h>
#include <time.h>

#define MAX_TITLE_LENGTH 100
#define MAX_SUBJECT_LENGTH 50
#define MAX_QUESTION_TEXT 500
#define MAX_OPTION_LENGTH 100
#define MAX_QUESTIONS_PER_PAPER 100

typedef struct Option {
    char text[MAX_OPTION_LENGTH];
    bool is_correct;
} Option;

typedef struct Question {
    int question_id;
    char text[MAX_QUESTION_TEXT];
    Option options[4];  // Assuming 4 options per question
    int marks;
    char difficulty;    // 'E' for Easy, 'M' for Medium, 'H' for Hard
} Question;

typedef struct {
    int paper_id;
    char title[MAX_TITLE_LENGTH];
    char subject[MAX_SUBJECT_LENGTH];
    int num_questions;
    int total_marks;
    int duration_minutes;
    time_t exam_date;       // Scheduled date for the exam
    bool is_active;         // Whether this paper is currently in use
    Question questions[MAX_QUESTIONS_PER_PAPER];
} ExamPaper;

// Core functions for exam paper management
bool initialize_exam_system(void);
ExamPaper* create_new_paper(const char* title, const char* subject, int duration);
bool save_exam_paper(const ExamPaper* paper);
bool load_exam_paper(int paper_id, ExamPaper* paper);
bool add_question_to_paper(ExamPaper* paper, const Question* question);
bool delete_exam_paper(int paper_id);
bool assign_paper_to_date(int paper_id, time_t exam_date);
ExamPaper* get_paper_for_date(time_t date);
bool list_available_papers(void);

// Student exam functions
bool start_exam(int student_id, ExamPaper* paper);
bool submit_exam(int student_id, const ExamPaper* paper, int* answers);

#endif // ENTRANCE_MANAGEMENT_SYSTEM_EXAM_H
