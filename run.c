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


        switch(option)
        {
            case 0: return 1;
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

//----printing character ch at n times ------

void printChar(char ch,int n)
{
    while(n--)
    {
        putchar(ch);
    }
}

//-----Printing Head Line of the program -----

void printHead()
{ system("cls");

printf("\n\n\t");
printChar('=',16);
printf("[ENTRANCE] [MANAGEMENT] [SYSTEM]");
printChar('=',16);
printf("\n");
}


// ===========sorting====================================
void sort(FILE * fp)
{
 printHead();
   Student s;
 int i,b,j,troll,siz=sizeof(s),a=0;
 float tmark;
 struct sort
 {
 	int roll;
 	float mark;
 }rank[a];

    rewind(fp);
    while((fread(&s,siz,1,fp))==1)
    {
        a+=1;
    }
	rewind(fp);
	b=0;
    while((fread(&s,siz,1,fp))==1)
    {
		rank[b].roll=s.ID;
		rank[b].mark=s.Mark;
     b+=1;
    }

      for (i=0;i<a;i++)
    {
    	for(j=0;j<a-1;j++)
    	{
    		if (rank[j].mark<rank[j+1].mark)
    		{
    			tmark=rank[j].mark;
    			rank[j].mark=rank[j+1].mark;
    			rank[j+1].mark=tmark;

    			troll=rank[j].roll;
    			rank[j].roll=rank[j+1].roll;
    			rank[j+1].roll=troll;
			}
				}
	}
	printf("\n\n\t");
	printChar('*',75);
	printf("\n\n\t\tRanking\t\t\tRoll No.\t\t\tMarks\n");
	for (i=0;i<a;i++)
	{
		if (rank[i].mark>=3)
		printf("\n\t\t%d\t\t\t%d\t\t\t%f",i+1,rank[i].roll,rank[i].mark);
	}
	printf("\n\n\t");
	printChar('~',75);
    	getch();

  }

// ==========FILLING STUDENT FORM==========================

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