/*
 * ENTRANCE MANAGEMENT SYSTEM
 * Functions for managing student entrance exams, records, and results
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h>

#define Student struct Stud

// Linux implementation of getch() and getche()
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

char getche() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

void add(FILE * fp);
void exam(FILE *fp);
void show_record();
void modify(FILE * fp);
void displayList(FILE * fp);
void searchRecord(FILE *fp);
void printChar(char ch,int n);
void printHead();
void sort(FILE *fp);
void welcomeAnimation();
void exitAnimation();

struct Stud
{
    char name[100];
    char dob[20];
    char address[50];
    int ID;
    float Mark;
    int n;
    int rank;  // student's rank after exam results
    char ph[20];
    char gname[100];
    char email[100];
    char gph[20];

};


int main()
{
    FILE * fp;
    Student s;
    int option;
    char another;

    if((fp=fopen("data/student.dat","rb+"))==NULL)
    {
        // Create data directory if it doesn't exist
        system("mkdir -p data");
        if((fp=fopen("data/student.dat","wb+"))==NULL)
        {
            printf("Can't create data file\n");
            return 0;
        }
    }

    welcomeAnimation();
printHead();
printf("\n\n\t\tCREATED BY");
printf("\n\n\t\tPrarambha Bashyal");
printf("\n\n\t\tPROJECT ON ENTRANCE MANAGEMENT SYSTEM");
printf("\n\n\n\t\tpress any key to continue\n\t");
printChar('-',64);
getch();

while(1)
{
    printHead();
    printf("\n\t");
    printChar(':',64);

    printf("\n\n\t\t\t1. FORM FOR STUDENT");//ADD
    printf("\n\n\t\t\t2. MODIFY STUDENT RECORD");
    printf("\n\n\t\t\t3. ENTRANCE EXAMINATION");//MODIFY
    printf("\n\n\t\t\t4. DISPLAY STUDENT RECORD");//DISPLAY
    printf("\n\n\t\t\t5. SEARCH RECORD");// RECORD
    printf("\n\n\t\t\t6. RANK OF STUDENTS");//SORTING OF RANK OF STUDENT
    printf("\n\n\t\t\t0. EXIT");//TERMINATE THE PROGRAM
    printf("\n\t");
	printf("\n\t");
	printChar('*',64);
    printf("\n\n\t\tEnter Your Option :--> ");
    scanf("%d",&option);
    // Clear input buffer to prevent leftover newline from affecting getch
    int c; while ((c = getchar()) != '\n' && c != EOF);

    switch(option)
    {
        case 0:
            exitAnimation();
            return 1;
                break;
        case 1: add(fp);
                break;
        case 2: modify(fp);
                break;
        case 3: exam(fp);
                break;
        case 4: displayList(fp);
                break;
        case 5: searchRecord(fp);
                break;
        case 6: sort(fp);
				break;
        default: printf("\n\t\tYou Pressed wrong key");
                  printf("\n\t\tProgram terminated");
                  getch();
                  exit(0);

    }
}
return 1;

}

/* Print Character Helper */
void printChar(char ch,int n)
{
    while(n--)
    {
        putchar(ch);
    }
}

/* Print Program Header */
void printHead()
{ system("clear");

printf("\n\n\t");
printChar('=',16);
printf("[ENTRANCE] [MANAGEMENT] [SYSTEM]");
printChar('=',16);
printf("\n");
}

/* Student Ranking System */
void sort(FILE * fp)
{
    printHead();
    Student s;
    int siz=sizeof(s);
    int count = 0;

    // First count exam-taken records only
    rewind(fp);
    while(fread(&s, siz, 1, fp) == 1) {
        if(s.n != 0) count++;
    }

    // Create dynamic array based on count
    struct {
        int roll;
        float mark;
        char name[100];
    } *rank = malloc(count * sizeof(*rank));

    if (!rank) {
        printf("\n\n\t\tError: Not enough memory!");
        return;
    }

    // Read only exam-taken records into array
    rewind(fp);
    int i = 0;
    while(fread(&s, siz, 1, fp) == 1) {
        if(s.n != 0) {
            rank[i].roll = s.ID;
            rank[i].mark = s.Mark;
            strncpy(rank[i].name, s.name, sizeof(rank[i].name));
            rank[i].name[sizeof(rank[i].name)-1] = '\0';
            i++;
        }
    }

    // Sort the records
    for(i = 0; i < count; i++) {
        for(int j = 0; j < count-1; j++) {
            if(rank[j].mark < rank[j+1].mark) {
                float tmark = rank[j].mark;
                rank[j].mark = rank[j+1].mark;
                rank[j+1].mark = tmark;

                int troll = rank[j].roll;
                rank[j].roll = rank[j+1].roll;
                rank[j+1].roll = troll;
            }
        }
    }

    // Display results
    printf("\n\n\t");
    printChar('*',75);
    printf("\n\n\t\tRank\tName\t\t\tRoll No.\tMarks\n");
    for(i = 0; i < count; i++) {
        printf("\n\t\t%d\t %-15s %d\t\t%.2f", i+1, rank[i].name, rank[i].roll, rank[i].mark);
    }
    printf("\n\n\t");
    printChar('~',75);
    printf("\n\n\t\tPress Enter to continue...");
    getchar();
    getchar();

    // Free allocated memory
    // free(rank); // defer freeing until after updating file

    // update each student's rank in file
    rewind(fp);
    for(int k=0; k<count; k++) {
        rewind(fp);
        Student tmp;
        while(fread(&tmp, siz, 1, fp) == 1) {
            if(tmp.ID == rank[k].roll) {
                tmp.rank = k+1;
                fseek(fp, -siz, SEEK_CUR);
                fwrite(&tmp, siz, 1, fp);
                break;
            }
        }
    }

   // Now free the rank array
   free(rank);
}

/* Student Registration Form */
void add(FILE * fp)

{
printHead();

char another='y';
Student s;
fseek(fp,0,SEEK_END);
 printf("\n\n\n");
    printf("\t\t");
    printChar('~',85);
    puts("\n\t\t*POINTS TO REMEMBER WHILE FILLING FORM");
    puts("\n\t\ti.  One should pay proper attention while filling the form.");
	puts("\n\t\tii. phone number and identification document number should be numeric (<10) i.e charcter");
	puts("\n\t\t    from 0 to 9.");
	puts("\n\t\tiii.one should remember their identification document number as it is the key factor");
	puts("\n\t\t    for other steps  ");
	puts("\n\t\tiv. one can modify their form record in modify portion with plenty of rs 500");
	puts("\n\t\tv.  identification document number should be last exam symbol number or ");
	puts("\n\t\t    citizenship number without any special sign(eg-,/_)  ");
	printf("\t\t");
	printChar('~',85);
	printf("\n\n\n");
	getch();
	printHead();
while(another=='y'||another=='Y')
{

    printf("\n\n\t\tEnter Full Name of Student:\t");
    fflush(stdin);
    fgets(s.name,100,stdin);
    s.name[strlen(s.name)-1]='\0';

    printf("\n\n\t\tEnter date of birth of Student:\t");
    fflush(stdin);
    fgets(s.dob,20,stdin);
    s.dob[strlen(s.dob)-1]='\0';

    printf("\n\n\t\tEnter Address:\t");
    fflush(stdin);
    fgets(s.address,50,stdin);
    s.address[strlen(s.address)-1]='\0';

    printf("\n\n\t\tIdentification document no: \t");
    scanf("%d",&s.ID);

	s.Mark=0;
	s.n=0;
    s.rank=0;

    printf("\n\n\t\tEnter student phone number:\t");
    fflush(stdin);
    fgets(s.ph,20,stdin);
    s.ph[strlen(s.ph)-1]='\0';


  printf("\n\n\t\tEnter email ID Student:\t");
    fflush(stdin);
    fgets(s.email,100,stdin);
    s.email[strlen(s.email)-1]='\0';


    printf("\n\n\t\tEnter guardian name of Student:\t");
    fflush(stdin);
    fgets(s.gname,100,stdin);
    s.gname[strlen(s.gname)-1]='\0';


    printf("\n\n\t\tEnter student's guardian phone number:\t");
    fflush(stdin);
    fgets(s.gph,20,stdin);
    s.gph[strlen(s.gph)]='\0';

    fwrite(&s,sizeof(s),1,fp);
    printf("\n\n\t\tWant to enter another student info (Y/N)\t");
    fflush(stdin);
    another=getche();
}
}

/* Entrance Examination System */
void exam(FILE *fp)
{
    printHead();
    Student s;
    int tempRoll, flag = 0;
    int siz = sizeof(s);
    float countr = 0.0;
    char another, cha;

    printf("\n\n\n\t\t");
    printChar('~',85);
    puts("\n\t\t*POINTS TO REMEMBER WHILE GIVING EXAMINATION ");
    puts("\n\t\ti.   Enter your ID number to start the exam");
    puts("\n\t\tii.  Total duration: 20 minutes");
    puts("\n\t\tiii. Total marks: 10 (1 mark per question)");
    puts("\n\t\tiv.  Press 'S' to skip a question");
    puts("\n\t\tv.   Negative marking: -0.1 for wrong answers");
    puts("\n\t\tvi.  Results will be shown immediately");
    printf("\t\t");
    printChar('~',85);
    printf("\n\n\n\tPress any key to continue...");
    getch();

    system("clear");
    printf("\n\n\tEnter your ID Number to start the exam: ");
    scanf("%d", &tempRoll);

    // Check if student exists and hasn't taken exam
    flag = 0;
    rewind(fp);
    while(fread(&s, siz, 1, fp) == 1)
    {
        if(s.ID == tempRoll && s.n == 0)
        {
            flag = 1;
            break;
        }
    }

    if(flag == 1)
    {
        system("clear");
        printf("\n\n\tWelcome %s to the Linux Basic Knowledge Test!", s.name);
        printf("\n\tPress any key when you're ready to begin...");
        getch();

        for(int i = 0; i < 10; i++)
        {
            system("clear");
            printf("\n\n\tQuestion %d of 10\n", i + 1);
            printChar('-', 50);

            switch(i)
            {
                case 0:
                    printf("\n\n\tQ1. What command is used to list files in Linux?\n");
                    printf("\n\t(A) dir\t\t(B) ls\n\t(C) show\t(D) list\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'b' || cha == 'B') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was B) ls");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 1:
                    printf("\n\n\tQ2. Which directory contains system configuration files in Linux?\n");
                    printf("\n\t(A) /bin\t\t(B) /home\n\t(C) /etc\t(D) /root\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'c' || cha == 'C') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was C) /etc");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 2:
                    printf("\n\n\tQ3. What command is used to change permissions in Linux?\n");
                    printf("\n\t(A) chmod\t(B) chown\n\t(C) perm\t(D) allow\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'a' || cha == 'A') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was A) chmod");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 3:
                    printf("\n\n\tQ4. Which command shows running processes?\n");
                    printf("\n\t(A) show\t(B) procs\n\t(C) ps\t\t(D) tasks\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'c' || cha == 'C') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was C) ps");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 4:
                    printf("\n\n\tQ5. What is the correct command to create a directory?\n");
                    printf("\n\t(A) makedir\t(B) create\n\t(C) md\t\t(D) mkdir\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'd' || cha == 'D') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was D) mkdir");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 5:
                    printf("\n\n\tQ6. Which command is used to view file contents?\n");
                    printf("\n\t(A) cat\t\t(B) show\n\t(C) view\t(D) read\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'a' || cha == 'A') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was A) cat");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 6:
                    printf("\n\n\tQ7. What is the root user's UID in Linux?\n");
                    printf("\n\t(A) 100\t\t(B) 1\n\t(C) 0\t\t(D) 999\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'c' || cha == 'C') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was C) 0");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 7:
                    printf("\n\n\tQ8. Which command is used to find files in Linux?\n");
                    printf("\n\t(A) search\t(B) locate\n\t(C) find\t(D) where\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'c' || cha == 'C') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was C) find");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 8:
                    printf("\n\n\tQ9. What command shows disk usage?\n");
                    printf("\n\t(A) du\t\t(B) disk\n\t(C) usage\t(D) df\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'a' || cha == 'A') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was A) du");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;

                case 9:
                    printf("\n\n\tQ10. Which command is used to extract a tar archive?\n");
                    printf("\n\t(A) unzip\t(B) extract\n\t(C) tar -x\t(D) untar\n");
                    printf("\n\tYour answer (A/B/C/D or S to skip): ");
                    fflush(stdin);
                    cha = getch();
                    printf("%c", cha);
                    if(cha == 'c' || cha == 'C') {
                        printf("\n\n\tCorrect!");
                        countr++;
                    } else if(cha == 's' || cha == 'S') {
                        printf("\n\n\tQuestion skipped.");
                    } else if(cha != '\n') {
                        printf("\n\n\tIncorrect! The correct answer was C) tar -x");
                        countr -= 0.1;
                    }
                    printf("\n\n\tPress any key to continue...");
                    getch();
                    break;
            }
        }

        // Update student's marks
        rewind(fp);
        while(fread(&s, siz, 1, fp) == 1)
        {
            if(s.ID == tempRoll)
            {
                fseek(fp, -siz, SEEK_CUR);
                s.Mark = countr;
                s.n = 1;  // Mark exam as taken
                fwrite(&s, sizeof(s), 1, fp);
                break;
            }
        }

        // Show results
        system("clear");
        printf("\n\n\n\n\t\t\t╔════════════════════════════════╗\n");
        printf("\t\t\t║          EXAM RESULTS          ║\n");
        printf("\t\t\t╠════════════════════════════════╣\n");
        printf("\t\t\t║ Name : %-23s║\n", s.name);
        printf("\t\t\t║ Score: %-23.2f║\n", countr);
        printf("\t\t\t╚════════════════════════════════╝\n");

        if(countr >= 7.0)
            printf("\n\t\t\tExcellent performance!");
        else if(countr >= 5.0)
            printf("\n\t\t\tGood job!");
        else
            printf("\n\t\t\tKeep practicing!");

        printf("\n\n\t\t\tPress any key to continue...");
        getch();
    }
    else
    {
        printf("\n\n\t\tError: Either you haven't registered or you've already taken the exam!");
        printf("\n\t\tPress any key to continue...");
        getch();
    }
}

/* Student Record Modification */
void modify(FILE * fp)
{
printHead();

Student s;
int i,flag=0,tempRoll,siz=sizeof(s);
float cgpa;
printf("\n\n\t Precaution:one shouldn't enter a string character while entering ID number");
printf("\n\n\tEnter  identification document  Number of Student to MODIFY the Record : ");
scanf("%d",&tempRoll);

rewind(fp);

while((fread(&s,siz,1,fp))==1)
{
    if(s.ID==tempRoll)
        {flag=1;
        break;
        }
}

if(flag==1)
    {
    fseek(fp,-siz,SEEK_CUR);
    printf("\n\n\t\tRECORD FOUND");
    printf("\n\n\t\tEnter New Data for the Record");

    printf("\n\n\t\tEnter Full Name of Student\t");
    fflush(stdin);
    fgets(s.name,100,stdin);
    s.name[strlen(s.name)-1]='\0';

    printf("\n\n\t\tEnter date of birth of Student\t");
    fflush(stdin);
    fgets(s.dob,20,stdin);
    s.dob[strlen(s.dob)-1]='\0';

    printf("\n\n\t\tEnter Address\t");
    fflush(stdin);
    fgets(s.address,50,stdin);
    s.address[strlen(s.address)-1]='\0';

    printf("\n\n\t\tEnter student phone number\t");
    fflush(stdin);
    fgets(s.ph,20,stdin);
    s.ph[strlen(s.ph)-1]='\0';


  printf("\n\n\t\tEnter email ID Student\t");
    fflush(stdin);
    fgets(s.email,100,stdin);
    s.email[strlen(s.email)-1]='\0';


    printf("\n\n\t\tEnter guardian name of Student\t");
    fflush(stdin);
    fgets(s.gname,100,stdin);
    s.gname[strlen(s.gname)-1]='\0';


    printf("\n\n\t\tEnter student's guardian phone number\t");
    fflush(stdin);
    fgets(s.gph,20,stdin);
    s.gph[strlen(s.gph)]='\0';
    fwrite(&s,sizeof(s),1,fp);
}

else
{
	printf("\n\n\t!!!! ERROR !!!! RECORD NOT FOUND");
}

printf("\n\n\t");
printf("Press Enter to continue...");
getchar();
getchar();
}

/* Display Student Records */
void displayList(FILE * fp)
{
    printHead();
    Student s;
    int i,siz=sizeof(s);

    rewind(fp);

    while((fread(&s,siz,1,fp))==1)
    {
        printf("\n\t\tNAME            : %s", s.name);
        printf("\n\t\tDATE OF BIRTH   : %s", s.dob);
        printf("\n\t\tADDRESS         : %s", s.address);
        printf("\n\t\tID NUMBER       : %d", s.ID);
        printf("\n\t\tPHONE NUMBER    : %s", s.ph);
        if (s.n!=0) {
            printf("\n\t\tMARKS          : %.2f", s.Mark);
            printf("\n\t\tRANK           : %d", s.rank);
        }
        printf("\n\t\tGUARDIAN NAME   : %s", s.gname);
        printf("\n\t\tEMAIL ADDRESS   : %s", s.email);
        printf("\n\t\tGUARDIAN PHONE  : %s", s.gph);
        printf("\n\n\t\t");
        printChar('x',40);
        printf("\n");
    }
    printf("\n\t");
    printChar('*',65);
    printf("\n\n\t\tPress Enter to continue...");
    getchar();
    getchar();
}

void searchRecord(FILE *fp)
{
    printHead();
    int tempRoll,flag=0,siz,i;
    Student s;
    char another='y';

    siz=sizeof(s);

    while(another=='y'||another=='Y')
    {
        printf("\n\n\tEnter Identification Number to search: ");
        scanf("%d",&tempRoll);

        rewind(fp);
        while((fread(&s,siz,1,fp))==1)
        {
            if(s.ID==tempRoll)
            {
                flag=1;
                break;
            }
        }
        if(flag==1)
        {
            printf("\n\t\tNAME            : %s", s.name);
            printf("\n\t\tDATE OF BIRTH   : %s", s.dob);
            printf("\n\t\tADDRESS         : %s", s.address);
            printf("\n\t\tID NUMBER       : %d", s.ID);
            printf("\n\t\tPHONE NUMBER    : %s", s.ph);
            printf("\n\t\tGUARDIAN NAME   : %s", s.gname);
            printf("\n\t\tEMAIL ADDRESS   : %s", s.email);
            printf("\n\t\tGUARDIAN PHONE  : %s", s.gph);
            if(s.n!=0) printf("\n\t\tRANK           : %d", s.rank);
        }
        else
            printf("\n\n\t\t!!!! ERROR: RECORD NOT FOUND !!!!");

        printf("\n");
        printChar('~',65);
        printf("\n\n\t\tSearch another record? (Y/N): ");
        fflush(stdin);
        another=getch();
    }
}

/* Welcome Animation */
void welcomeAnimation() {
    int i, j;
    system("clear");
    for(i = 0; i < 3; i++) {
        system("clear");
        printf("\n\n\n");
        printf("\t\t\t┌──────────────────────────────────────┐\n");
        printf("\t\t\t│                                      │\n");
        printf("\t\t\t│      Welcome to EMS %c               │\n", i % 2 ? '*' : '+');
        printf("\t\t\t│                                      │\n");
        printf("\t\t\t│      Entrance Management System      │\n");
        printf("\t\t\t│                                      │\n");
        printf("\t\t\t└──────────────────────────────────────┘\n");
        sleep(1);
    }
    printf("\n\n\t\t\tLoading");
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            printf(".");
            fflush(stdout);
            usleep(200000); // 200ms delay
        }
        printf("\b\b\b   \b\b\b");
        fflush(stdout);
    }
}

/* Exit Animation */
void exitAnimation() {
    int i;
    system("clear");
    printf("\n\n\n");
    printf("\t\t\t┌──────────────────────────────────────┐\n");
    printf("\t\t\t│                                      │\n");
    printf("\t\t\t│    Thank You for Using EMS!          │\n");
    printf("\t\t\t│                                      │\n");
    printf("\t\t\t│         See You Next Time!           │\n");
    printf("\t\t\t│                                      │\n");
    printf("\t\t\t└──────────────────────────────────────┘\n");

    printf("\n\n\t\t\t");
    for(i = 0; i < 3; i++) {
        printf("\rExiting   ");
        fflush(stdout);
        usleep(300000);
        printf("\rExiting.  ");
        fflush(stdout);
        usleep(300000);
        printf("\rExiting.. ");
        fflush(stdout);
        usleep(300000);
        printf("\rExiting...");
        fflush(stdout);
        usleep(300000);
    }
    system("clear");
}
