#include "../include/student.h"
#include "../include/common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Search for a student by ID or name
void search_student(FILE *fp) {
    if (!fp) return;
    
    print_header();
    printf("\n\n\t\tSEARCH STUDENT");
    printf("\n\t\t--------------");
    
    printf("\n\n\t\t1. Search by ID");
    printf("\n\t\t2. Search by Name");
    printf("\n\t\t0. Back");
    
    int choice = get_menu_choice(0, 2);
    if (choice == 0) return;
    
    if (choice == 1) {
        // Search by ID
        int search_id;
        printf("\n\t\tEnter Student ID to search: ");
        if (scanf("%d", &search_id) != 1) {
            clear_input_buffer();
            printf("\n\t\tInvalid ID format!");
            printf("\n\t\tPress any key to continue...");
            getch();
            return;
        }
        clear_input_buffer();
        
        rewind(fp);
        Student s;
        bool found = false;
        
        printf("\n\t\tSearch Results:");
        printf("\n\t\t--------------");
        
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (s.id == search_id) {
                show_student_details(&s);
                found = true;
                break;
            }
        }
        
        if (!found) {
            printf("\n\t\tNo student found with ID: %d", search_id);
        }
    } 
    else if (choice == 2) {
        // Search by Name
        char search_name[MAX_NAME];
        printf("\n\t\tEnter Student Name to search: ");
        safe_input(search_name, sizeof(search_name));
        
        rewind(fp);
        Student s;
        bool found = false;
        
        printf("\n\t\tSearch Results:");
        printf("\n\t\t--------------");
        
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (strcasecmp(s.name, search_name) == 0) {
                show_student_details(&s);
                found = true;
            }
        }
        
        if (!found) {
            printf("\n\t\tNo student found with name: %s", search_name);
        }
    }
    
    printf("\n\n\t\tPress any key to continue...");
    getch();
}

// Display student details
void show_student_details(const Student *s) {
    if (!s) return;
    
    printf("\n\n\t\tStudent ID: %d", s->id);
    printf("\n\t\tName: %s", s->name);
    printf("\n\t\tDate of Birth: %s", s->dob);
    printf("\n\t\tEmail: %s", s->email);
    printf("\n\t\tPhone: %s", s->phone);
    printf("\n\t\tParent: %s (%s)", s->parent_name, s->parent_phone);
    printf("\n\t\tAddress: %s", s->address);
    printf("\n\t\tEducation: %s", s->education);
    printf("\n\t\tSchool: %s", s->school);
    printf("\n\t\tGrade: %s, Section: %s", s->grade, s->section);
    printf("\n\t\tStatus: %s", s->is_active ? "Active" : "Inactive");
    printf("\n\t\tRegistered: %s", ctime(&s->created_at));
}

// Add a new student
bool add_student(Student *student) {
    if (!student) return false;

    FILE *fp = fopen(STUDENT_FILE, "ab+");
    if (!fp) {
        log_message(LOG_ERROR, "Failed to open student file for writing");
        return false;
    }

    // Check if ID already exists
    Student temp;
    rewind(fp);
    while (fread(&temp, sizeof(Student), 1, fp) == 1) {
        if (temp.id == student->id) {
            fclose(fp);
            log_message(LOG_WARNING, "Student with ID %d already exists", student->id);
            return false;
        }
    }
    
    // Set metadata
    student->is_active = true;
    student->created_at = time(NULL);
    student->updated_at = student->created_at;
    student->created_by = current_user.ID;  // Fixed field name to match User struct

    // Write to file
    fseek(fp, 0, SEEK_END);
    bool success = (fwrite(student, sizeof(Student), 1, fp) == 1);
    fclose(fp);

    if (success) {
        log_message(LOG_INFO, "Added new student: %s (ID: %d)", student->name, student->id);
    } else {
        log_message(LOG_ERROR, "Failed to add student: %s", student->name);
    }
    
    return success;
}

// Generate QR code for student ID card
bool generate_qr_code(const char *data, const char *filename) {
    if (!data || !filename) return false;

    // In a real implementation, this would use a QR code library
    // For now, just create a text file with the data
    FILE *fp = fopen(filename, "w");
    if (!fp) return false;

    fprintf(fp, "QR CODE DATA: %s\n", data);
    fclose(fp);

    return true;
}

// Check in a student using their QR code
bool check_in_with_qr(const char *qr_data) {
    if (!qr_data || strlen(qr_data) == 0) {
        log_message(LOG_WARNING, "Empty QR code data provided");
        return false;
    }

    FILE *fp = fopen(STUDENT_FILE, "rb");
    if (!fp) {
        log_message(LOG_ERROR, "Failed to open student file for QR check-in");
        return false;
    }
    
    Student s;
    bool found = false;
    char student_qr[MAX_QR_DATA];

    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        // Use safer snprintf with size limit and check return value
        int len = snprintf(student_qr, MAX_QR_DATA, "STUDENT_%d_%.50s", s.id, s.name);
        if (len < 0 || len >= MAX_QR_DATA) {
            log_message(LOG_ERROR, "QR code generation failed: buffer too small");
            continue;
        }

        if (strcmp(student_qr, qr_data) == 0) {
            found = true;
            show_student_details(&s);
            log_message(LOG_INFO, "Student checked in: %s (ID: %d)", s.name, s.id);
            break;
        }
    }
    
    fclose(fp);
    return found;
}
