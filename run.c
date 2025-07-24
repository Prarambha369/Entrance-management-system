#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>

/*==============================
 * CROSS-PLATFORM COMPATIBILITY
 *==============================*/
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    #include <windows.h>
    #define CLEAR_SCREEN system("cls")
    #define DATA_FILE "student.dat"

    // Define colors for Windows
    #define COLOR_RESET 7
    #define COLOR_RED 12
    #define COLOR_GREEN 10
    #define COLOR_BLUE 9
    #define COLOR_YELLOW 14

    void set_color(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
#else
    #include <stdio.h>
    #include <termios.h>
    #include <unistd.h>
    #define CLEAR_SCREEN system("clear")
    #define DATA_FILE "student.dat"

    // Define ANSI color codes
    #define COLOR_RESET 0
    #define COLOR_RED 1
    #define COLOR_GREEN 2
    #define COLOR_BLUE 4
    #define COLOR_YELLOW 3

    void set_color(int color) {
        const char *colors[] = {
            "\033[0m",     // Reset
            "\033[1;31m",  // Red
            "\033[1;32m",  // Green
            "\033[1;33m",  // Yellow
            "\033[1;34m"   // Blue
        };
        printf("%s", colors[color % 5]);
    }

    int getch(void) {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }

    int getche(void) {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~ICANON;
        newattr.c_lflag |= ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif

/*==============================
 * CONSTANTS AND DEFINITIONS
 *==============================*/
#define MAX_NAME 100
#define MAX_DOB 20
#define MAX_ADDRESS 50
#define MAX_PHONE 20
#define MAX_EMAIL 100
#define EXAM_TIME_MINUTES 20
#define EXAM_QUESTIONS 10
#define VERSION "2.0"

// Student structure with improved naming
typedef struct Student {
    char name[MAX_NAME];
    char dob[MAX_DOB];
    char address[MAX_ADDRESS];
    int ID;
    float mark;
    int has_taken_exam; // Changed from n to a more descriptive name
    char phone[MAX_PHONE];
    char guardian_name[MAX_NAME];
    char email[MAX_EMAIL];
    char guardian_phone[MAX_PHONE];
    time_t registration_date; // New field to track when student registered
} Student;

// Question structure for exams
typedef struct Question {
    char question_text[500];
    char options[4][100];
    char correct_answer; // 'A', 'B', 'C', or 'D'
    int difficulty;      // 1-5
    char category[50];   // e.g., "Math", "Physics", etc.
} Question;

/*==============================
 * FUNCTION PROTOTYPES
 *==============================*/
// Core functionality
void add_student(FILE *fp);
void conduct_exam(FILE *fp);
void show_records();
void modify_student(FILE *fp);
void display_all_students(FILE *fp);
void search_student(FILE *fp);
void rank_students(FILE *fp);

// Helper functions
void print_char(char ch, int n);
void print_header();
void print_footer();
void print_separator(char ch);
void safe_input(char *buffer, int size);
FILE* safe_open(const char *filename, const char *mode);
bool validate_email(const char *email);
bool validate_phone(const char *phone);
bool is_numeric(const char *str);
void display_datetime();
void create_backup();
void display_about();
void display_help();

// Exam system functions
void load_questions(Question questions[]);
void shuffle_questions(int order[], int count);

/*==============================
 * MAIN FUNCTION
 *==============================*/
int main() {
    FILE *fp;
    int option;

    // Try to open the data file
    if ((fp = safe_open(DATA_FILE, "rb+")) == NULL) {
        if ((fp = safe_open(DATA_FILE, "wb+")) == NULL) {
            set_color(COLOR_RED);
            printf("\n\nFatal Error: Cannot create data file.");
            set_color(COLOR_RESET);
            return 1;
        }
    }

    print_header();
    set_color(COLOR_GREEN);
    printf("\n\n\t\tENTRANCE MANAGEMENT SYSTEM v%s", VERSION);
    set_color(COLOR_RESET);
    printf("\n\n\t\tCREATED BY");
    printf("\n\n\t\tPrarambha Bashyal");
    printf("\n\n\n\t\tPress any key to continue\n\t");
    print_char('-', 64);
    getch();

    while (1) {
        print_header();
        printf("\n\t");
        print_char(':', 64);

        printf("\n\n\t\t\t1. REGISTER STUDENT");
        printf("\n\n\t\t\t2. MODIFY STUDENT RECORD");
        printf("\n\n\t\t\t3. CONDUCT ENTRANCE EXAMINATION");
        printf("\n\n\t\t\t4. DISPLAY ALL STUDENT RECORDS");
        printf("\n\n\t\t\t5. SEARCH FOR A STUDENT");
        printf("\n\n\t\t\t6. VIEW STUDENT RANKINGS");
        printf("\n\n\t\t\t7. CREATE BACKUP");
        printf("\n\n\t\t\t8. ABOUT");
        printf("\n\n\t\t\t9. HELP");
        printf("\n\n\t\t\t0. EXIT");

        printf("\n\t");
        printf("\n\t");
        print_char('*', 64);

        // Display current date and time
        display_datetime();

        printf("\n\n\t\tEnter Your Option :--> ");
        scanf("%d", &option);

        switch(option) {
            case 0:
                fclose(fp);
                set_color(COLOR_GREEN);
                printf("\n\n\t\tThank you for using the system.");
                set_color(COLOR_RESET);
                printf("\n\n\t\tExiting program...");
                return 0;
            case 1: add_student(fp); break;
            case 2: modify_student(fp); break;
            case 3: conduct_exam(fp); break;
            case 4: display_all_students(fp); break;
            case 5: search_student(fp); break;
            case 6: rank_students(fp); break;
            case 7: create_backup(); break;
            case 8: display_about(); break;
            case 9: display_help(); break;
            default:
                set_color(COLOR_RED);
                printf("\n\t\tInvalid option selected!");
                set_color(COLOR_RESET);
                printf("\n\t\tPress any key to continue...");
                getch();
        }
    }
    return 0;
}

/*==============================
 * CORE FUNCTIONALITY
 *==============================*/

void add_student(FILE *fp) {
    print_header();
    char another = 'y';
    Student s;

    // Guidelines for filling the form
    printf("\n\n\t");
    print_char('~', 85);
    set_color(COLOR_YELLOW);
    puts("\n\t\t*POINTS TO REMEMBER WHILE FILLING FORM");
    set_color(COLOR_RESET);
    puts("\n\t\ti.   One should pay proper attention while filling the form.");
    puts("\n\t\tii.  Phone number should contain only numeric digits.");
    puts("\n\t\tiii. You must remember your ID number for future reference.");
    puts("\n\t\tiv.  Records can be modified later if needed (additional fee applies).");
    puts("\n\t\tv.   ID number should be your last exam symbol number or citizenship");
    puts("\n\t\t     number without any special characters.");
    printf("\t\t");
    print_char('~', 85);
    printf("\n\n\n");
    getch();

    print_header();
    while(another == 'y' || another == 'Y') {
        // Clear any existing data in the struct
        memset(&s, 0, sizeof(Student));

        printf("\n\n\t\tEnter Full Name of Student:\t");
        fflush(stdin);
        safe_input(s.name, MAX_NAME);

        printf("\n\n\t\tEnter date of birth (YYYY-MM-DD):\t");
        fflush(stdin);
        safe_input(s.dob, MAX_DOB);

        printf("\n\n\t\tEnter Address:\t");
        fflush(stdin);
        safe_input(s.address, MAX_ADDRESS);

        do {
            printf("\n\n\t\tIdentification document no: \t");
            scanf("%d", &s.ID);

            // Check if ID already exists
            int exists = 0;
            Student temp;
            long current_pos = ftell(fp);

            rewind(fp);
            while(fread(&temp, sizeof(temp), 1, fp) == 1) {
                if(temp.ID == s.ID) {
                    exists = 1;
                    break;
                }
            }

            // Go back to our position in the file
            fseek(fp, current_pos, SEEK_SET);

            if(exists) {
                set_color(COLOR_RED);
                printf("\n\t\tThis ID already exists. Please enter a different ID.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        s.mark = 0;
        s.has_taken_exam = 0;

        do {
            printf("\n\n\t\tEnter student phone number:\t");
            fflush(stdin);
            safe_input(s.phone, MAX_PHONE);

            if(!validate_phone(s.phone)) {
                set_color(COLOR_RED);
                printf("\n\t\tInvalid phone number. Please enter only digits.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        do {
            printf("\n\n\t\tEnter email ID of Student:\t");
            fflush(stdin);
            safe_input(s.email, MAX_EMAIL);

            if(!validate_email(s.email)) {
                set_color(COLOR_RED);
                printf("\n\t\tInvalid email format. Please try again.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        printf("\n\n\t\tEnter guardian name of Student:\t");
        fflush(stdin);
        safe_input(s.guardian_name, MAX_NAME);

        printf("\n\n\t\tEnter student's guardian phone number:\t");
        fflush(stdin);
        safe_input(s.guardian_phone, MAX_PHONE);

        // Record registration timestamp
        s.registration_date = time(NULL);

        // Write to file
        fwrite(&s, sizeof(s), 1, fp);

        // Ask if user wants to add another student
        set_color(COLOR_YELLOW);
        printf("\n\n\t\tRegistration successful!");
        set_color(COLOR_RESET);
        printf("\n\n\t\tWant to enter another student info (Y/N)?\t");
        fflush(stdin);
        another = getche();

        if (another == 'y' || another == 'Y') {
            print_header();
        }
    }
}

void conduct_exam(FILE *fp) {
    print_header();
    int temp_id, flag = 0;
    Student s;
    float score;
    char answer;
    size_t record_size = sizeof(s);

    // Load and prepare exam questions
    Question questions[EXAM_QUESTIONS];
    load_questions(questions);

    // Create a random order for questions
    int question_order[EXAM_QUESTIONS];
    shuffle_questions(question_order, EXAM_QUESTIONS);

    // Guidelines for taking the exam
    printf("\n\n\n");
    printf("\t\t");
    print_char('~', 85);
    set_color(COLOR_YELLOW);
    puts("\n\t\t*POINTS TO REMEMBER WHILE GIVING EXAMINATION ");
    set_color(COLOR_RESET);
    puts("\n\t\ti.   You must enter your ID number to begin the entrance exam.");
    puts("\n\t\tii.  The exam duration is 20 minutes with 10 questions worth 10 marks total.");
    puts("\n\t\tiii. You can skip any question by pressing 'S'.");
    puts("\n\t\tiv.  Once skipped, questions can't be reattempted.");
    puts("\n\t\tv.   Each question is worth 1 mark.");
    puts("\n\t\tvi.  There is negative marking of 10% per wrong answer.");
    puts("\n\t\tvii. Your score will be displayed upon completion.");
    printf("\t\t");
    print_char('~', 85);
    printf("\n\n\n");
    getch();

    print_header();
    set_color(COLOR_YELLOW);
    printf("\n\n\t\tDo you want to take the exam? (Y/N)\t");
    set_color(COLOR_RESET);
    fflush(stdin);
    answer = getch();

    if (answer == 'y' || answer == 'Y') {
        printf("\n\n\tEnter Your ID Number: ");
        scanf("%d", &temp_id);

        // Find student record
        rewind(fp);
        flag = 0;
        while((fread(&s, record_size, 1, fp)) == 1) {
            if(s.ID == temp_id && s.has_taken_exam == 0) {
                flag = 1;
                break;
            }
        }

        if (flag == 1) {
            score = 0.0;

            // Set up timer
            time_t start_time = time(NULL);
            time_t end_time = start_time + (EXAM_TIME_MINUTES * 60);
            time_t current_time;

            for (int i = 0; i < EXAM_QUESTIONS; i++) {
                CLEAR_SCREEN;

                // Display timer
                current_time = time(NULL);
                if (current_time >= end_time) {
                    set_color(COLOR_RED);
                    printf("\n\n\t\tTIME'S UP!");
                    set_color(COLOR_RESET);
                    break;
                }

                int mins_left = (end_time - current_time) / 60;
                int secs_left = (end_time - current_time) % 60;

                set_color(COLOR_YELLOW);
                printf("\n\tTime Remaining: %02d:%02d", mins_left, secs_left);
                printf("\n\tQuestion %d of %d", i+1, EXAM_QUESTIONS);
                set_color(COLOR_RESET);

                // Get the question based on the randomized order
                int q_index = question_order[i];

                printf("\n\n\t%s\n", questions[q_index].question_text);

                // Display options
                for (int j = 0; j < 4; j++) {
                    printf("\n\t(%c) %s", 'A' + j, questions[q_index].options[j]);
                }

                // Get answer
                printf("\n\n\tYour answer (A/B/C/D or S to skip): ");
                fflush(stdin);
                answer = getch();
                answer = toupper(answer);

                // Process answer
                if (answer == 'S') {
                    printf("\n\tQuestion skipped.");
                    getch();
                } else if (answer >= 'A' && answer <= 'D') {
                    if (answer == questions[q_index].correct_answer) {
                        score += 1.0;
                        set_color(COLOR_GREEN);
                        printf("\n\tCorrect!");
                        set_color(COLOR_RESET);
                    } else {
                        score -= 0.1;
                        set_color(COLOR_RED);
                        printf("\n\tIncorrect! The correct answer was %c.",
                               questions[q_index].correct_answer);
                        set_color(COLOR_RESET);
                    }
                    getch();
                } else {
                    set_color(COLOR_RED);
                    printf("\n\tInvalid option! Press any key to try again.");
                    set_color(COLOR_RESET);
                    getch();
                    i--; // Repeat this question
                }
            }

            // Store the result
            rewind(fp);
            while((fread(&s, record_size, 1, fp)) == 1) {
                if(s.ID == temp_id) {
                    fseek(fp, -record_size, SEEK_CUR);
                    s.mark = score;
                    s.has_taken_exam = 1;
                    fwrite(&s, sizeof(s), 1, fp);
                    break;
                }
            }

            // Display result
            CLEAR_SCREEN;
            print_header();
            set_color(COLOR_GREEN);
            printf("\n\n\n\n\t\t\tCongratulations %s!", s.name);
            printf("\n\t\t\tYou scored: %.2f out of 10.0\n", score);
            set_color(COLOR_RESET);
            printf("\t\t\tBest of luck for the results!\n");
            printf("\t\t\tPress any key to return to the main menu...");
        } else {
            set_color(COLOR_RED);
            printf("\n\t\tEither you have not registered yet or you have already taken the exam.");
            set_color(COLOR_RESET);
        }
        getch();
    }
}

void modify_student(FILE *fp) {
    print_header();

    Student s;
    int temp_id, flag = 0;
    size_t record_size = sizeof(s);

    set_color(COLOR_YELLOW);
    printf("\n\n\tCAUTION: Modifying student records requires an additional fee of Rs 500.");
    set_color(COLOR_RESET);
    printf("\n\n\tEnter ID Number of Student to MODIFY: ");
    scanf("%d", &temp_id);

    rewind(fp);

    while((fread(&s, record_size, 1, fp)) == 1) {
        if(s.ID == temp_id) {
            flag = 1;
            break;
        }
    }

    if(flag == 1) {
        fseek(fp, -record_size, SEEK_CUR);
        set_color(COLOR_GREEN);
        printf("\n\n\t\tRECORD FOUND");
        set_color(COLOR_RESET);
        printf("\n\n\t\tEnter New Data for the Record");

        printf("\n\n\t\tEnter Full Name of Student\t");
        fflush(stdin);
        safe_input(s.name, MAX_NAME);

        printf("\n\n\t\tEnter date of birth (YYYY-MM-DD)\t");
        fflush(stdin);
        safe_input(s.dob, MAX_DOB);

        printf("\n\n\t\tEnter Address\t");
        fflush(stdin);
        safe_input(s.address, MAX_ADDRESS);

        do {
            printf("\n\n\t\tEnter student phone number:\t");
            fflush(stdin);
            safe_input(s.phone, MAX_PHONE);

            if(!validate_phone(s.phone)) {
                set_color(COLOR_RED);
                printf("\n\t\tInvalid phone number. Please enter only digits.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        do {
            printf("\n\n\t\tEnter email ID of Student:\t");
            fflush(stdin);
            safe_input(s.email, MAX_EMAIL);

            if(!validate_email(s.email)) {
                set_color(COLOR_RED);
                printf("\n\t\tInvalid email format. Please try again.");
                set_color(COLOR_RESET);
            } else {
                break;
            }
        } while(1);

        printf("\n\n\t\tEnter guardian name of Student\t");
        fflush(stdin);
        safe_input(s.guardian_name, MAX_NAME);

        printf("\n\n\t\tEnter student's guardian phone number\t");
        fflush(stdin);
        safe_input(s.guardian_phone, MAX_PHONE);

        fwrite(&s, sizeof(s), 1, fp);

        set_color(COLOR_GREEN);
        printf("\n\n\t\tRecord Updated Successfully!");
        set_color(COLOR_RESET);
    } else {
        set_color(COLOR_RED);
        printf("\n\n\t!!!! ERROR !!!! RECORD NOT FOUND");
        set_color(COLOR_RESET);
    }

    printf("\n\n\t");
    printf("Press any key to continue...");
    getch();
}

void display_all_students(FILE *fp) {
    print_header();
    Student s;
    size_t record_size = sizeof(s);
    int count = 0;

    rewind(fp);

    // First pass to count records
    while(fread(&s, record_size, 1, fp) == 1) {
        count++;
    }

    if(count == 0) {
        set_color(COLOR_RED);
        printf("\n\n\t\tNo student records found.");
        set_color(COLOR_RESET);
        printf("\n\n\t\tPress any key to continue...");
        getch();
        return;
    }

    rewind(fp);
    printf("\n\t\tTotal Students: %d\n", count);
    print_char('-', 80);

    int current_page = 1;
    int records_per_page = 5;
    int total_pages = (count + records_per_page - 1) / records_per_page;
    int start_record = 0;
    char choice;

    do {
        CLEAR_SCREEN;
        print_header();
        printf("\n\t\tStudent Records - Page %d of %d\n", current_page, total_pages);
        print_char('-', 80);

        rewind(fp);

        // Skip to start record
        for(int i = 0; i < start_record; i++) {
            fread(&s, record_size, 1, fp);
        }

        int displayed = 0;

        while(displayed < records_per_page && fread(&s, record_size, 1, fp) == 1) {
            printf("\n\t\tID: %d", s.ID);
            printf("\n\t\tNAME: %s", s.name);
            printf("\n\t\tDATE OF BIRTH: %s", s.dob);
            printf("\n\t\tADDRESS: %s", s.address);
            printf("\n\t\tPHONE: %s", s.phone);

            if(s.has_taken_exam != 0) {
                printf("\n\t\tEXAM MARK: %.2f", s.mark);
            } else {
                printf("\n\t\tEXAM STATUS: Not taken");
            }

            printf("\n\t\tEMAIL: %s", s.email);
            printf("\n\t\tGUARDIAN: %s", s.guardian_name);
            printf("\n\t\tGUARDIAN PHONE: %s", s.guardian_phone);

            // Format and display registration date
            char date_str[30];
            struct tm *tm_info = localtime(&s.registration_date);
            strftime(date_str, 30, "%Y-%m-%d %H:%M:%S", tm_info);
            printf("\n\t\tREGISTERED ON: %s", date_str);

            printf("\n\n\t\t");
            print_char('-', 50);
            printf("\n\n");

            displayed++;
        }

        printf("\n\n\t[P]revious Page | [N]ext Page | [B]ack to Main Menu: ");
        fflush(stdin);
        choice = getch();

        if(choice == 'n' || choice == 'N') {
            if(current_page < total_pages) {
                current_page++;
                start_record += records_per_page;
            }
        } else if(choice == 'p' || choice == 'P') {
            if(current_page > 1) {
                current_page--;
                start_record -= records_per_page;
            }
        }

    } while(choice != 'b' && choice != 'B');
}

void search_student(FILE *fp) {
    print_header();

    int search_option;
    Student s;
    size_t record_size = sizeof(s);
    char search_string[100];
    int search_id;
    int found = 0;

    printf("\n\n\t\tSEARCH OPTIONS");
    printf("\n\n\t\t1. Search by ID");
    printf("\n\t\t2. Search by Name");
    printf("\n\t\t3. Search by Address");
    printf("\n\t\t4. Search by Email");
    printf("\n\n\t\tEnter your choice: ");
    scanf("%d", &search_option);

    switch(search_option) {
        case 1:
            printf("\n\tEnter Student ID to search: ");
            scanf("%d", &search_id);

            rewind(fp);
            while(fread(&s, record_size, 1, fp) == 1) {
                if(s.ID == search_id) {
                    found = 1;
                    break;
                }
            }
            break;

        case 2:
            printf("\n\tEnter Student Name to search: ");
            fflush(stdin);
            safe_input(search_string, 100);

            rewind(fp);
            while(fread(&s, record_size, 1, fp) == 1) {
                if(strstr(s.name, search_string) != NULL) {
                    found = 1;
                    break;
                }
            }
            break;

        case 3:
            printf("\n\tEnter Address to search: ");
            fflush(stdin);
            safe_input(search_string, 100);

            rewind(fp);
            while(fread(&s, record_size, 1, fp) == 1) {
                if(strstr(s.address, search_string) != NULL) {
                    found = 1;
                    break;
                }
            }
            break;

        case 4:
            printf("\n\tEnter Email to search: ");
            fflush(stdin);
            safe_input(search_string, 100);

            rewind(fp);
            while(fread(&s, record_size, 1, fp) == 1) {
                if(strstr(s.email, search_string) != NULL) {
                    found = 1;
                    break;
                }
            }
            break;

        default:
            set_color(COLOR_RED);
            printf("\n\tInvalid search option!");
            set_color(COLOR_RESET);
    }

    if(found == 1) {
        set_color(COLOR_GREEN);
        printf("\n\n\t\tRECORD FOUND");
        set_color(COLOR_RESET);

        printf("\n\n\t\tNAME: %s", s.name);
        printf("\n\t\tDATE OF BIRTH: %s", s.dob);
        printf("\n\t\tADDRESS: %s", s.address);
        printf("\n\t\tID: %d", s.ID);
        printf("\n\t\tPHONE NUMBER: %s", s.phone);

        if (s.has_taken_exam != 0)
            printf("\n\t\tEXAM MARK: %.2f", s.mark);
        else
            printf("\n\t\tEXAM STATUS: Not taken");

        printf("\n\t\tEMAIL: %s", s.email);
        printf("\n\t\tGUARDIAN NAME: %s", s.guardian_name);
        printf("\n\t\tGUARDIAN PHONE: %s", s.guardian_phone);
    } else {
        set_color(COLOR_RED);
        printf("\n\n\t\t!!!! ERROR RECORD NOT FOUND !!!!");
        set_color(COLOR_RESET);
    }

    printf("\n\n\tPress any key to continue...");
    getch();
}

void rank_students(FILE *fp) {
    print_header();
    Student s;
    size_t record_size = sizeof(s);
    int count = 0;

    // First pass to count students who have taken the exam
    rewind(fp);
    while(fread(&s, record_size, 1, fp) == 1) {
        if(s.has_taken_exam != 0) {
            count++;
        }
    }

    if(count == 0) {
        set_color(COLOR_RED);
        printf("\n\n\t\tNo students have taken the exam yet.");
        set_color(COLOR_RESET);
        printf("\n\n\t\tPress any key to continue...");
        getch();
        return;
    }

    // Create an array to store student IDs and marks
    struct {
        int id;
        float mark;
        char name[MAX_NAME];
    } *rankings = malloc(count * sizeof(*rankings));

    if(rankings == NULL) {
        set_color(COLOR_RED);
        printf("\n\n\t\tMemory allocation failed!");
        set_color(COLOR_RESET);
        printf("\n\n\t\tPress any key to continue...");
        getch();
        return;
    }

    // Second pass to collect data
    rewind(fp);
    int index = 0;
    while(fread(&s, record_size, 1, fp) == 1) {
        if(s.has_taken_exam != 0) {
            rankings[index].id = s.ID;
            rankings[index].mark = s.mark;
            strcpy(rankings[index].name, s.name);
            index++;
        }
    }

    // Sort by marks (descending)
    for(int i = 0; i < count - 1; i++) {
        for(int j = 0; j < count - i - 1; j++) {
            if(rankings[j].mark < rankings[j+1].mark) {
                // Swap
                int temp_id = rankings[j].id;
                float temp_mark = rankings[j].mark;
                char temp_name[MAX_NAME];
                strcpy(temp_name, rankings[j].name);

                rankings[j].id = rankings[j+1].id;
                rankings[j].mark = rankings[j+1].mark;
                strcpy(rankings[j].name, rankings[j+1].name);

                rankings[j+1].id = temp_id;
                rankings[j+1].mark = temp_mark;
                strcpy(rankings[j+1].name, temp_name);
            }
        }
    }

    // Display the rankings
    printf("\n\n\t");
    print_char('*', 75);

    set_color(COLOR_YELLOW);
    printf("\n\n\t%-10s%-20s%-20s%-20s\n", "Rank", "Name", "ID", "Marks");
    set_color(COLOR_RESET);
    print_char('-', 75);

    // Display only students who passed (mark >= 3)
    int rank = 1;
    for(int i = 0; i < count; i++) {
        if(rankings[i].mark >= 3) {
            printf("\n\t%-10d%-20s%-20d%-20.2f",
                   rank++, rankings[i].name, rankings[i].id, rankings[i].mark);
        }
    }

    // Display pass rate statistics
    int passed = 0;
    for(int i = 0; i < count; i++) {
        if(rankings[i].mark >= 3) {
            passed++;
        }
    }

    printf("\n\n\t");
    print_char('-', 75);
    printf("\n\tPass rate: %.1f%% (%d out of %d students)",
           (float)passed / count * 100, passed, count);

    free(rankings);

    printf("\n\n\t");
    print_char('~', 75);
    printf("\n\n\tPress any key to continue...");
    getch();
}

/*==============================
 * HELPER FUNCTIONS
 *==============================*/

void print_char(char ch, int n) {
    while(n--) {
        putchar(ch);
    }
}

void print_header() {
    CLEAR_SCREEN;
    printf("\n\n\t");
    print_char('=', 16);

    set_color(COLOR_BLUE);
    printf("[ENTRANCE] [MANAGEMENT] [SYSTEM]");
    set_color(COLOR_RESET);

    print_char('=', 16);
    printf("\n");
}

void print_footer() {
    printf("\n\n\t");
    print_char('-', 64);
    printf("\n\t© 2025 Prarambha Bashyal. All Rights Reserved.");
    printf("\n\t");
    print_char('-', 64);
}

void print_separator(char ch) {
    printf("\n\t");
    print_char(ch, 64);
}

void safe_input(char *buffer, int size) {
    fgets(buffer, size, stdin);

    // Remove trailing newline if present
    size_t len = strlen(buffer);
    if(len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }

    // Clear input buffer if overflow occurred
    if(len == size-1 && buffer[len-1] != '\n') {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
    }
}

FILE* safe_open(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if(fp == NULL) {
        fprintf(stderr, "\nError opening file %s: %s", filename, strerror(errno));
    }
    return fp;
}

bool validate_email(const char *email) {
    // Basic email validation: check for @ symbol and at least one character on each side
    const char *at = strchr(email, '@');
    if(at == NULL || at == email || *(at+1) == '\0') {
        return false;
    }

    // Check for a period after the @ symbol
    const char *dot = strchr(at, '.');
    return (dot != NULL && dot > at && *(dot+1) != '\0');
}

bool validate_phone(const char *phone) {
    // Check that phone contains only digits, spaces, dashes
    for(int i = 0; phone[i] != '\0'; i++) {
        if(!isdigit(phone[i]) && phone[i] != '-' && phone[i] != ' ') {
            return false;
        }
    }
    return true;
}

bool is_numeric(const char *str) {
    while(*str) {
        if(!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

void display_datetime() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buffer[30];

    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\n\n\t\tCurrent Date & Time: %s", buffer);
}

void create_backup() {
    print_header();

    FILE *src, *dst;
    char backup_filename[100];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Create backup filename with timestamp
    strftime(backup_filename, 100, "backup_student_%Y%m%d_%H%M%S.dat", tm_info);

    src = safe_open(DATA_FILE, "rb");
    if(src == NULL) {
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not open source file!");
        set_color(COLOR_RESET);
        getch();
        return;
    }

    dst = safe_open(backup_filename, "wb");
    if(dst == NULL) {
        set_color(COLOR_RED);
        printf("\n\n\t\tError: Could not create backup file!");
        set_color(COLOR_RESET);
        fclose(src);
        getch();
        return;
    }

    // Copy file contents
    char buffer[1024];
    size_t bytes_read;

    printf("\n\n\t\tCreating backup...");

    while((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes_read, dst);
    }

    fclose(src);
    fclose(dst);

    set_color(COLOR_GREEN);
    printf("\n\n\t\tBackup created successfully: %s", backup_filename);
    set_color(COLOR_RESET);

    printf("\n\n\t\tPress any key to continue...");
    getch();
}

void display_about() {
    print_header();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tABOUT ENTRANCE MANAGEMENT SYSTEM");
    set_color(COLOR_RESET);
    printf("\n\t\tVersion %s", VERSION);

    printf("\n\n\t\tDeveloped by: Prarambha Bashyal");
    printf("\n\t\tLast Updated: July 2025");

    printf("\n\n\t\tThis software manages the entrance examination process,");
    printf("\n\t\tfrom student registration to exam administration and results.");

    printf("\n\n\t\tFeatures:");
    printf("\n\t\t- Student Registration");
    printf("\n\t\t- Record Management");
    printf("\n\t\t- Entrance Examination");
    printf("\n\t\t- Student Rankings");
    printf("\n\t\t- Data Backup");

    printf("\n\n\t\tContact:");
    printf("\n\t\tEmail: prarambha@example.com");

    print_footer();

    printf("\n\n\t\tPress any key to return to main menu...");
    getch();
}

void display_help() {
    print_header();

    set_color(COLOR_BLUE);
    printf("\n\n\t\tHELP & INSTRUCTIONS");
    set_color(COLOR_RESET);

    printf("\n\n\t1. REGISTER STUDENT:");
    printf("\n\t   - Use this option to add new student records");
    printf("\n\t   - You'll need to provide personal details and contact information");
    printf("\n\t   - Each student must have a unique ID number");

    printf("\n\n\t2. MODIFY STUDENT RECORD:");
    printf("\n\t   - Use this to update existing student information");
    printf("\n\t   - You'll need the student's ID number to locate their record");
    printf("\n\t   - Note: There is a fee of Rs 500 for record modification");

    printf("\n\n\t3. CONDUCT ENTRANCE EXAMINATION:");
    printf("\n\t   - Students can take their entrance exam through this option");
    printf("\n\t   - The exam consists of 10 questions and lasts 20 minutes");
    printf("\n\t   - Students need their ID number to access the exam");
    printf("\n\t   - Each student can take the exam only once");

    printf("\n\n\t4. DISPLAY STUDENT RECORDS:");
    printf("\n\t   - View all registered students and their information");
    printf("\n\t   - Records are displayed in pages for easier navigation");

    printf("\n\n\t5. SEARCH FOR A STUDENT:");
    printf("\n\t   - Find specific student records using various search criteria");
    printf("\n\t   - You can search by ID, name, address, or email");

    printf("\n\n\t6. VIEW STUDENT RANKINGS:");
    printf("\n\t   - See how students performed in the entrance examination");
    printf("\n\t   - Students are ranked by their exam scores");
    printf("\n\t   - Only students with scores of 3 or above are ranked");

    printf("\n\n\t7. CREATE BACKUP:");
    printf("\n\t   - Make a safety copy of all student data");
    printf("\n\t   - Backups are timestamped for easy identification");

    printf("\n\n\tFor technical support, please contact:");
    printf("\n\tprarambha@example.com");

    printf("\n\n\t\tPress any key to return to main menu...");
    getch();
}

/*==============================
 * EXAM SYSTEM FUNCTIONS
 *==============================*/

void load_questions(Question questions[]) {
    // In a real implementation, these would be loaded from a file
    // Hardcoding for demonstration purposes

    strcpy(questions[0].question_text, "The word 'engineer' has its primary stress on its ______ syllable:");
    strcpy(questions[0].options[0], "first");
    strcpy(questions[0].options[1], "second");
    strcpy(questions[0].options[2], "third");
    strcpy(questions[0].options[3], "fourth");
    questions[0].correct_answer = 'C';
    questions[0].difficulty = 2;
    strcpy(questions[0].category, "English");

    strcpy(questions[1].question_text, "Two and two _____ four.");
    strcpy(questions[1].options[0], "makes");
    strcpy(questions[1].options[1], "make");
    strcpy(questions[1].options[2], "have made");
    strcpy(questions[1].options[3], "will be made");
    questions[1].correct_answer = 'A';
    questions[1].difficulty = 1;
    strcpy(questions[1].category, "English");

    strcpy(questions[2].question_text, "pH of 0.2 N H2SO4 is:");
    strcpy(questions[2].options[0], "0.69");
    strcpy(questions[2].options[1], "1.2");
    strcpy(questions[2].options[2], "0.76");
    strcpy(questions[2].options[3], "0.56");
    questions[2].correct_answer = 'A';
    questions[2].difficulty = 3;
    strcpy(questions[2].category, "Chemistry");

    strcpy(questions[3].question_text, "Catalytic oxidation of ammonia in air forms:");
    strcpy(questions[3].options[0], "NO2");
    strcpy(questions[3].options[1], "N2O5");
    strcpy(questions[3].options[2], "N2O");
    strcpy(questions[3].options[3], "NO");
    questions[3].correct_answer = 'D';
    questions[3].difficulty = 3;
    strcpy(questions[3].category, "Chemistry");

    strcpy(questions[4].question_text, "The magnetism of the magnet is due to:");
    strcpy(questions[4].options[0], "the spin motion of electron");
    strcpy(questions[4].options[1], "cosmic ray");
    strcpy(questions[4].options[2], "the earth");
    strcpy(questions[4].options[3], "pressure of big magnet inside the earth");
    questions[4].correct_answer = 'A';
    questions[4].difficulty = 2;
    strcpy(questions[4].category, "Physics");

    strcpy(questions[5].question_text, "The maximum percentage of ingredients in cement is that of:");
    strcpy(questions[5].options[0], "magnesia");
    strcpy(questions[5].options[1], "lime");
    strcpy(questions[5].options[2], "alumina");
    strcpy(questions[5].options[3], "silica");
    questions[5].correct_answer = 'B';
    questions[5].difficulty = 4;
    strcpy(questions[5].category, "Chemistry");

    strcpy(questions[6].question_text, "The second derivative of f(x) = 1/x at point (1,1) is equal to:");
    strcpy(questions[6].options[0], "1");
    strcpy(questions[6].options[1], "-1");
    strcpy(questions[6].options[2], "2");
    strcpy(questions[6].options[3], "-2");
    questions[6].correct_answer = 'C';
    questions[6].difficulty = 4;
    strcpy(questions[6].category, "Mathematics");

    strcpy(questions[7].question_text, "Which of the following is a logic gate?");
    strcpy(questions[7].options[0], "PUT");
    strcpy(questions[7].options[1], "THEN");
    strcpy(questions[7].options[2], "NOR");
    strcpy(questions[7].options[3], "WHEN");
    questions[7].correct_answer = 'C';
    questions[7].difficulty = 2;
    strcpy(questions[7].category, "Computer Science");

    strcpy(questions[8].question_text, "If the line 2x+3y+4+k(-x+y+5)=0 is horizontal then the value of k is:");
    strcpy(questions[8].options[0], "0");
    strcpy(questions[8].options[1], "1");
    strcpy(questions[8].options[2], "3");
    strcpy(questions[8].options[3], "2");
    questions[8].correct_answer = 'D';
    questions[8].difficulty = 5;
    strcpy(questions[8].category, "Mathematics");

    strcpy(questions[9].question_text, "IUPAC name of CH3-CH2-CH2-CH(OCH3)-CO Br is:");
    strcpy(questions[9].options[0], "2-methoxylpentanoyl bromide");
    strcpy(questions[9].options[1], "3-methoxylpentanoyl bromide");
    strcpy(questions[9].options[2], "3-methoxyhexanoyl bromide");
    strcpy(questions[9].options[3], "2-methoxyhexanoyl bromide");
    questions[9].correct_answer = 'A';
    questions[9].difficulty = 5;
    strcpy(questions[9].category, "Chemistry");
}

void shuffle_questions(int order[], int count) {
    // Initialize order array
    for(int i = 0; i < count; i++) {
        order[i] = i;
    }

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Shuffle using Fisher-Yates algorithm
    for(int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = order[i];
        order[i] = order[j];
        order[j] = temp;
    }
}