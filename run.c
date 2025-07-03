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