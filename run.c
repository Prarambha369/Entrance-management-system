#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<time.h>
#include<conio.h>
#include<ctype.h>
#define Student struct Stud

void add(FILE * fp); //to add to list
void exam(FILE *fp);//for entrance examination
void show_record();
void modify(FILE * fp);//to modify a record
void displayList(FILE * fp);//display whole list
void searchRecord(FILE *fp);//find a particular record
void printChar(char ch,int n);//printing a character ch n times
void printHead();//printing head line for each screen
void sort(FILE *fp);//sorting

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

int main()
{
 FILE * fp;
 Student s;
int option;
char another;

if((fp=fopen("K:\\student.txt","rb+"))==NULL)
{
    if((fp=fopen("K:\\student.txt","wb+"))==NULL)
       {
           printf("can't open file");
           return 0;
       }
}

printHead();
printf("\n\n\t\tCREATED BY");
printf("\n\n\t\tPRASHANT BASYAL");
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