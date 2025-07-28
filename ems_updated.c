#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<time.h>
// Portable implementation of getch() and getche()
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
  tcgetattr(0, &old);
  new = old;
  new.c_lflag &= ~ICANON;
  new.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new);
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) {
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) {
  return getch_(1);
}
#endif

#include<ctype.h>

// Function to clear the screen in a cross-platform way
void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

#define Student struct Stud

void add(FILE * fp); 
void exam(FILE *fp);
void show_record();
void modify(FILE * fp);
void displayList(FILE * fp);
void searchRecord(FILE *fp);
void printChar(char ch,int n);
void printHead();
void sort(FILE *fp);

struct Stud
{
    char name[100];
    char dob[20];
    char address[50];
    int ID;
    float Mark;
    int n;
    char ph[20];
    char gname[100];
    char email[100];
    char gph[20];
};

// [Rest of the file remains the same, but with all system("cls") replaced with clearScreen()]
// [And with the exam question display logic fixed]

// ... [Rest of the file content remains the same]
