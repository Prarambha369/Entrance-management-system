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
            if (s.ID == search_id) {
                display_student_details(&s);
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
                display_student_details(&s);
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
void display_student_details(Student *s) {
    if (!s) return;
    
    printf("\n\n\t\tStudent ID: %d", s->ID);
    printf("\n\t\tName: %s", s->name);
    printf("\n\t\tDate of Birth: %s", s->dob);
    printf("\n\t\tEmail: %s", s->email);
    printf("\n\t\tPhone: %s", s->phone);
    printf("\n\t\tGuardian: %s (%s)", s->guardian_name, s->guardian_phone);
    printf("\n\t\tAddress: %s", s->address);
    
    if (s->has_taken_exam) {
        printf("\n\t\tExam Status: Taken (Score: %.2f)", s->mark);
    } else {
        printf("\n\t\tExam Status: Not taken yet");
    }
}

// Add a new student
void add_student(FILE *fp) {
    if (!fp) return;
    
    Student s;
    memset(&s, 0, sizeof(Student));
    
    print_header();
    printf("\n\n\t\tADD NEW STUDENT");
    printf("\n\t\t---------------");
    
    // Get student details
    printf("\n\n\t\tEnter Student Details:");
    printf("\n\t\t---------------------");
    
    // Get ID
    printf("\n\t\tID: ");
    if (scanf("%d", &s.ID) != 1) {
        clear_input_buffer();
        printf("\n\t\tInvalid ID format!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }
    clear_input_buffer();
    
    // Check if ID already exists
    rewind(fp);
    Student temp;
    while (fread(&temp, sizeof(Student), 1, fp) == 1) {
        if (temp.ID == s.ID) {
            printf("\n\t\tStudent with ID %d already exists!", s.ID);
            printf("\n\t\tPress any key to continue...");
            getch();
            return;
        }
    }
    
    // Get other details
    printf("\t\tName: ");
    safe_input(s.name, sizeof(s.name));
    
    printf("\t\tDate of Birth (YYYY-MM-DD): ");
    safe_input(s.dob, sizeof(s.dob));
    
    do {
        printf("\t\tEmail: ");
        safe_input(s.email, sizeof(s.email));
    } while (!validate_email(s.email));
    
    do {
        printf("\t\tPhone: ");
        safe_input(s.phone, sizeof(s.phone));
    } while (!validate_phone(s.phone));
    
    printf("\t\tGuardian Name: ");
    safe_input(s.guardian_name, sizeof(s.guardian_name));
    
    do {
        printf("\t\tGuardian Phone: ");
        safe_input(s.guardian_phone, sizeof(s.guardian_phone));
    } while (!validate_phone(s.guardian_phone));
    
    printf("\t\tAddress: ");
    safe_input(s.address, sizeof(s.address));
    
    // Set default values
    s.has_taken_exam = 0;
    s.mark = 0.0f;
    s.registration_date = time(NULL);
    s.has_user_account = false;
    
    // Generate QR code
    generate_qr_code(&s);
    
    // Write to file
    fseek(fp, 0, SEEK_END);
    if (fwrite(&s, sizeof(Student), 1, fp) == 1) {
        printf("\n\n\t\tStudent added successfully!");
    } else {
        printf("\n\n\t\tError adding student!");
    }
    
    printf("\n\t\tPress any key to continue...");
    getch();
}

// Generate QR code for student
void generate_qr_code(Student *s) {
    if (!s) return;
    
    // Simple implementation - in a real app, use a QR code library
    snprintf(s->qr_code, MAX_QR_DATA, "STUDENT_%d_%s", s->ID, s->name);
}

// Display QR code (placeholder implementation)
void display_qr_code(const char *qr_data) {
    if (!qr_data) return;
    
    printf("\n\t\tQR Code Data: %s", qr_data);
    printf("\n\t\t[QR CODE IMAGE WOULD BE DISPLAYED HERE]");
}

// QR Code check-in
void check_in_with_qr(FILE *fp) {
    if (!fp) return;
    
    print_header();
    printf("\n\n\t\tQR CODE CHECK-IN");
    printf("\n\t\t----------------");
    
    char qr_data[MAX_QR_DATA];
    printf("\n\n\t\tScan QR Code or Enter Code: ");
    safe_input(qr_data, sizeof(qr_data));
    
    if (strlen(qr_data) == 0) {
        printf("\n\t\tNo QR code data provided!");
        printf("\n\t\tPress any key to continue...");
        getch();
        return;
    }
    
    rewind(fp);
    Student s;
    bool found = false;
    
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (strcmp(s.qr_code, qr_data) == 0) {
            printf("\n\n\t\tSTUDENT CHECKED IN SUCCESSFULLY!");
            printf("\n\t\t-------------------------------");
            display_student_details(&s);
            found = true;
            break;
        }
    }
    
    if (!found) {
        printf("\n\t\tNo student found with the provided QR code!");
    }
    
    printf("\n\n\t\tPress any key to continue...");
    getch();
}
