#include "../include/exam.h"
#include "../include/common.h"
#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EXAM_DATA_FILE "data/exams.dat"
#define EXAM_SCHEDULE_FILE "data/exam_schedule.dat"

static int get_next_paper_id(void) {
    FILE* fp = fopen(EXAM_DATA_FILE, "rb");
    if (!fp) return 1;  // Start with ID 1 if file doesn't exist

    ExamPaper paper;
    int max_id = 0;

    while (fread(&paper, sizeof(ExamPaper), 1, fp) == 1) {
        if (paper.paper_id > max_id) {
            max_id = paper.paper_id;
        }
    }

    fclose(fp);
    return max_id + 1;
}

bool initialize_exam_system(void) {
    FILE* fp = fopen(EXAM_DATA_FILE, "ab");
    if (!fp) {
        log_message(LOG_ERROR, "Failed to initialize exam system: Cannot create exam data file");
        return false;
    }
    fclose(fp);

    fp = fopen(EXAM_SCHEDULE_FILE, "ab");
    if (!fp) {
        log_message(LOG_ERROR, "Failed to initialize exam system: Cannot create exam schedule file");
        return false;
    }
    fclose(fp);

    return true;
}

ExamPaper* create_new_paper(const char* title, const char* subject, int duration) {
    ExamPaper* paper = (ExamPaper*)malloc(sizeof(ExamPaper));
    if (!paper) {
        log_message(LOG_ERROR, "Memory allocation failed for new exam paper");
        return NULL;
    }

    memset(paper, 0, sizeof(ExamPaper));
    paper->paper_id = get_next_paper_id();
    strncpy(paper->title, title, MAX_TITLE_LENGTH - 1);
    strncpy(paper->subject, subject, MAX_SUBJECT_LENGTH - 1);
    paper->duration_minutes = duration;
    paper->num_questions = 0;
    paper->is_active = true;

    if (save_exam_paper(paper)) {
        log_message(LOG_INFO, "Created new exam paper: %s (ID: %d)", title, paper->paper_id);
        return paper;
    }

    free(paper);
    return NULL;
}

bool save_exam_paper(const ExamPaper* paper) {
    if (!paper) return false;

    FILE* fp = fopen(EXAM_DATA_FILE, "r+b");
    if (!fp) {
        fp = fopen(EXAM_DATA_FILE, "wb");
        if (!fp) {
            log_message(LOG_ERROR, "Failed to open exam data file for writing");
            return false;
        }
    }

    // Try to find and update existing paper
    ExamPaper existing;
    bool found = false;
    long pos = 0;

    while (fread(&existing, sizeof(ExamPaper), 1, fp) == 1) {
        if (existing.paper_id == paper->paper_id) {
            fseek(fp, pos, SEEK_SET);
            found = true;
            break;
        }
        pos = ftell(fp);
    }

    if (!found) {
        fseek(fp, 0, SEEK_END);
    }

    if (fwrite(paper, sizeof(ExamPaper), 1, fp) != 1) {
        log_message(LOG_ERROR, "Failed to write exam paper to file");
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

bool load_exam_paper(int paper_id, ExamPaper* paper) {
    if (!paper) return false;

    FILE* fp = fopen(EXAM_DATA_FILE, "rb");
    if (!fp) {
        log_message(LOG_ERROR, "Failed to open exam data file for reading");
        return false;
    }

    bool found = false;
    ExamPaper temp;

    while (fread(&temp, sizeof(ExamPaper), 1, fp) == 1) {
        if (temp.paper_id == paper_id) {
            memcpy(paper, &temp, sizeof(ExamPaper));
            found = true;
            break;
        }
    }

    fclose(fp);
    return found;
}

bool add_question_to_paper(ExamPaper* paper, const Question* question) {
    if (!paper || !question) return false;
    if (paper->num_questions >= MAX_QUESTIONS_PER_PAPER) {
        log_message(LOG_ERROR, "Cannot add more questions: paper is full");
        return false;
    }

    memcpy(&paper->questions[paper->num_questions], question, sizeof(Question));
    paper->num_questions++;
    paper->total_marks += question->marks;

    return save_exam_paper(paper);
}

bool delete_exam_paper(int paper_id) {
    FILE* fp = fopen(EXAM_DATA_FILE, "r+b");
    if (!fp) return false;

    ExamPaper paper;
    bool found = false;
    long pos = 0;

    while (fread(&paper, sizeof(ExamPaper), 1, fp) == 1) {
        if (paper.paper_id == paper_id) {
            paper.is_active = false;
            fseek(fp, pos, SEEK_SET);
            found = fwrite(&paper, sizeof(ExamPaper), 1, fp) == 1;
            break;
        }
        pos = ftell(fp);
    }

    fclose(fp);
    if (found) {
        log_message(LOG_INFO, "Deleted exam paper ID: %d", paper_id);
    }
    return found;
}

bool assign_paper_to_date(int paper_id, time_t exam_date) {
    ExamPaper paper;
    if (!load_exam_paper(paper_id, &paper)) {
        log_message(LOG_ERROR, "Failed to load exam paper for assignment");
        return false;
    }

    paper.exam_date = exam_date;
    return save_exam_paper(&paper);
}

ExamPaper* get_paper_for_date(time_t date) {
    FILE* fp = fopen(EXAM_DATA_FILE, "rb");
    if (!fp) return NULL;

    ExamPaper* paper = NULL;
    ExamPaper temp;

    while (fread(&temp, sizeof(ExamPaper), 1, fp) == 1) {
        // Compare dates ignoring time
        struct tm* temp_tm = localtime(&temp.exam_date);
        struct tm* date_tm = localtime(&date);

        if (temp_tm->tm_year == date_tm->tm_year &&
            temp_tm->tm_mon == date_tm->tm_mon &&
            temp_tm->tm_mday == date_tm->tm_mday &&
            temp.is_active) {
            paper = (ExamPaper*)malloc(sizeof(ExamPaper));
            if (paper) {
                memcpy(paper, &temp, sizeof(ExamPaper));
            }
            break;
        }
    }

    fclose(fp);
    return paper;
}

bool list_available_papers(void) {
    FILE* fp = fopen(EXAM_DATA_FILE, "rb");
    if (!fp) {
        log_message(LOG_ERROR, "No exam papers found");
        return false;
    }

    ExamPaper paper;
    bool found = false;

    printf("\nAvailable Exam Papers:\n");
    printf("%-5s %-30s %-15s %-10s %-20s\n", "ID", "Title", "Subject", "Questions", "Exam Date");
    printf("----------------------------------------------------------------\n");

    while (fread(&paper, sizeof(ExamPaper), 1, fp) == 1) {
        if (paper.is_active) {
            char date_str[20] = "Not scheduled";
            if (paper.exam_date > 0) {
                strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime(&paper.exam_date));
            }
            printf("%-5d %-30s %-15s %-10d %-20s\n",
                   paper.paper_id, paper.title, paper.subject,
                   paper.num_questions, date_str);
            found = true;
        }
    }

    fclose(fp);
    return found;
}

// Basic implementation of exam taking functionality
bool start_exam(int student_id, ExamPaper* paper) {
    if (!paper) return false;

    log_message(LOG_INFO, "Student %d started exam: %s", student_id, paper->title);
    return true;
}

bool submit_exam(int student_id, const ExamPaper* paper, int* answers) {
    if (!paper || !answers) return false;

    // TODO: Implement exam submission and scoring logic
    log_message(LOG_INFO, "Student %d submitted exam: %s", student_id, paper->title);
    return true;
}
