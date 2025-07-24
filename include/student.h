#ifndef STUDENT_H
#define STUDENT_H

#include "common.h"

// Student structure with improved naming
typedef struct Student {
    char name[MAX_NAME];
    char dob[MAX_DOB];
    char address[MAX_ADDRESS];
    int ID;
    float mark;
    int has_taken_exam;
    char phone[MAX_PHONE];
    char guardian_name[MAX_NAME];
    char email[MAX_EMAIL];
    char guardian_phone[MAX_PHONE];
    time_t registration_date;
    char qr_code[MAX_QR_DATA];
    bool has_user_account;
} Student;

// Function prototypes
void add_student(FILE *fp);
void modify_student(FILE *fp);
void display_all_students(FILE *fp);
void search_student(FILE *fp);
void rank_students(FILE *fp);
void generate_qr_code(Student *s);
void display_qr_code(const char *qr_data);
void check_in_with_qr(FILE *fp);

#endif // STUDENT_H
