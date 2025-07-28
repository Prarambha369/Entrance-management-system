#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#ifdef _WIN32
    #include <conio.h>
    #define CLEAR_SCREEN() system("cls")
    #define PAUSE() system("pause")
#else
    #define CLEAR_SCREEN() printf("\033[2J\033[1;1H")
    #define PAUSE() { printf("Press ENTER to continue..."); getchar(); }
    int getch() { int c; system("stty raw -echo"); c = getchar(); system("stty cooked echo"); return c; }
    int getche() { int c = getch(); putchar(c); return c; }
#endif

#define Student struct Stud

void add(FILE * fp);
void exam(FILE *fp);
void modify(FILE * fp);
void displayList(FILE * fp);
void searchRecord(FILE *fp);
void printChar(char ch,int n);
void printHead();
void sort(FILE *fp);

struct Stud {
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
    int option;

    if((fp=fopen("student.txt","rb+"))==NULL)
    {
        if((fp=fopen("student.txt","wb+"))==NULL)
        {
            printf("can't open file");
            return 1;
        }
    }

    printHead();
    printf("\n\n\t\tCREATED BY");
    printf("\n\n\t\tMister Bashyal");
    printf("\n\n\t\tPROJECT ON ENTRANCE MANAGEMENT SYSTEM");
    printf("\n\n\n\t\tpress any key to continue\n\t");
    printChar('-',64);
    getch();

    while(1)
    {
        printHead();
        printf("\n\t");
        printChar(':',64);
        printf("\n\n\t\t\t1. FORM FOR STUDENT");
        printf("\n\n\t\t\t2. MODIFY STUDENT RECORD");
        printf("\n\n\t\t\t3. ENTRANCE EXAMINATION");
        printf("\n\n\t\t\t4. DISPLAY STUDENT RECORD");
        printf("\n\n\t\t\t5. SEARCH RECORD");
        printf("\n\n\t\t\t6. RANK OF STUDENTS");
        printf("\n\n\t\t\t0. EXIT");
        printf("\n\t");
        printf("\n\t");
        printChar('*',64);
        printf("\n\n\t\tEnter Your Option :--> ");
        if (scanf("%d",&option) != 1) {
            printf("\n\t\tInvalid input. Exiting.\n");
            break;
        }
        while (getchar() != '\n');

        switch(option)
        {
            case 0: fclose(fp); return 0;
            case 1: add(fp); break;
            case 2: modify(fp); break;
            case 3: exam(fp); break;
            case 4: displayList(fp); break;
            case 5: searchRecord(fp); break;
            case 6: sort(fp); break;
            default:
                printf("\n\t\tYou Pressed wrong key");
                printf("\n\t\tProgram terminated");
                getch();
                fclose(fp);
                exit(0);
        }
    }
    fclose(fp);
    return 0;
}

void printChar(char ch,int n)
{
    while(n--) putchar(ch);
}

void printHead()
{
    CLEAR_SCREEN();
    printf("\n\n\t");
    printChar('=',16);
    printf("[ENTRANCE] [MANAGEMENT] [SYSTEM]");
    printChar('=',16);
    printf("\n");
}

void sort(FILE * fp)
{
    printHead();
    Student s;
    int siz = sizeof(s), count = 0, i, j, troll;
    float tmark;
    struct sort_el { int roll; float mark; char name[100]; } *rank = NULL;

    // Count only students who have taken the exam
    rewind(fp);
    while((fread(&s, siz, 1, fp)) == 1) {
        if (s.n != 0)
            count++;
    }
    if (count == 0) {
        printf("\nNo students have taken the exam yet.\n");
        PAUSE();
        return;
    }

    rank = malloc(count * sizeof(*rank));
    if (!rank) {
        printf("Memory allocation failed.");
        return;
    }

    // Fill the array
    rewind(fp);
    int idx = 0;
    while((fread(&s, siz, 1, fp)) == 1) {
        if (s.n != 0) {
            rank[idx].roll = s.ID;
            rank[idx].mark = s.Mark;
            strncpy(rank[idx].name, s.name, 99);
            rank[idx].name[99] = '\0';
            idx++;
        }
    }

    // Sort by marks descending
    for (i = 0; i < count-1; i++)
        for (j = 0; j < count-1-i; j++)
            if (rank[j].mark < rank[j+1].mark) {
                tmark = rank[j].mark; rank[j].mark = rank[j+1].mark; rank[j+1].mark = tmark;
                troll = rank[j].roll; rank[j].roll = rank[j+1].roll; rank[j+1].roll = troll;
                char tempName[100];
                strcpy(tempName, rank[j].name);
                strcpy(rank[j].name, rank[j+1].name);
                strcpy(rank[j+1].name, tempName);
            }

    printf("\n\n\t");
    printChar('*',75);
    printf("\n\n\t\tRank\t\tRoll No.\t\tMarks\t\tName\n");
    for (i = 0; i < count; i++)
        printf("\n\t\t%d\t\t%d\t\t%.2f\t\t%s", i+1, rank[i].roll, rank[i].mark, rank[i].name);
    printf("\n\n\t");
    printChar('~',75);
    getch();
    free(rank);
}

void add(FILE * fp)
{
    printHead();
    char another='y';
    Student s;
    int siz = sizeof(s);

    fseek(fp,0,SEEK_END);
    printf("\n\n\n");
    printf("\t\t");
    printChar('~',85);
    puts("\n\t\t*POINTS TO REMEMBER WHILE FILLING FORM");
    puts("\n\t\ti.  One should pay proper attention while filling the form.");
    puts("\n\t\tii. phone number and identification document number should be numeric (<10) i.e character");
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
        fgets(s.name,100,stdin);
        s.name[strcspn(s.name, "\n")] = '\0';

        printf("\n\n\t\tEnter date of birth of Student:\t");
        fgets(s.dob,20,stdin);
        s.dob[strcspn(s.dob, "\n")] = '\0';

        printf("\n\n\t\tEnter Address:\t");
        fgets(s.address,50,stdin);
        s.address[strcspn(s.address, "\n")] = '\0';

        printf("\n\n\t\tIdentification document no: \t");
        scanf("%d",&s.ID);
        while (getchar() != '\n');

        s.Mark=0;
        s.n=0;

        printf("\n\n\t\tEnter student phone number:\t");
        fgets(s.ph,20,stdin);
        s.ph[strcspn(s.ph, "\n")] = '\0';

        printf("\n\n\t\tEnter email ID Student:\t");
        fgets(s.email,100,stdin);
        s.email[strcspn(s.email, "\n")] = '\0';

        printf("\n\n\t\tEnter guardian name of Student:\t");
        fgets(s.gname,100,stdin);
        s.gname[strcspn(s.gname, "\n")] = '\0';

        printf("\n\n\t\tEnter student's guardian phone number:\t");
        fgets(s.gph,20,stdin);
        s.gph[strcspn(s.gph, "\n")] = '\0';

        fwrite(&s,sizeof(s),1,fp);
        printf("\n\n\t\tWant to enter another student info (Y/N)\t");
        another=getche();
        printf("\n");
    }
}

void exam(FILE *fp)
{
    printHead();
    int tempRoll,flag=0,siz=sizeof(Student),r;
    Student s;
    float countr;
    char another,cha;
    fseek(fp,0,SEEK_END);
    printf("\n\n\n");
    printf("\t\t");
    printChar('~',85);
    puts("\n\t\t*POINTS TO REMEMBER WHILE GIVING EXAMINATION ");
    puts("\n\t\ti.   One should enter their identification document number to begin the entrance exam.");
    puts("\n\t\tii.  Entrance is taken for 20 minute having total 10 marks.");
    puts("\n\t\tiii. Examinee can skip any question by pressing 's' ");
    puts("\n\t\tiv.  Once skipped questions can't be reattempted.");
    puts("\n\t\tv.   It consists of 10 questions each having 1 mark.");
    puts("\n\t\tvi.  There is a negative marking of 10%% marks per each wrong answer.");
    puts("\n\t\tvii. Once the answer is submitted cannot be recorrected.");
    puts("\n\t\tviii.Total score is displayed when all the answers are submitted.");
    printf("\t\t");
    printChar('~',85);
    printf("\n\n\n");
    getch();
    printHead();
    printf("\n\n\t\t Do you want to take exam  (Y/N)\t");
    another=getch();
    printf("%c\n", another);

    if (another=='y'||another=='Y')
    {
        printf("\n\n\tEnter Identification Number of Student: ");
        scanf("%d",&tempRoll);
        while(getchar()!='\n');
        rewind(fp);
        flag = 0;
        while((fread(&s,siz,1,fp))==1)
        {
            if(s.ID==tempRoll && s.n==0)
            {
                flag=1;
                break;
            }
        }
        if (flag==1)
        {
            countr=0.0;
            int i;
            bool arr[10]={0};
            int A[10];
            time_t t;
            srand((unsigned)time(&t));
            for (i=0;i<10;i++)
            {
                int r = rand()%10;
                if(!arr[r])
                    A[i]= r;
                else {
                    i--;
                    continue;
                }
                arr[r]=1;
            }
            for(i=0;i<10;i++)
            {
                switch(A[i])
                {
                case 1:
a1:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.The word 'engineer' has its primary stress on its ______ syllable:\n");
                    printf("\n\n\t(A) first \t\t(B) second \n\t(C) third \t\t(D) fourth");
                    cha=getch();
                    if (cha=='c'||cha=='C') { countr++; break; }
                    else if(cha=='a'||cha=='A'||cha=='b'||cha=='B'||cha=='d'||cha=='D') { countr-=.1; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a1; }
                case 2:
a2:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.Two and two _____ four.");
                    printf("\n\n\t(A) makes \t\t(B) make \n\t(C) have made \t\t(D) will be made ");
                    cha=getch();
                    if (cha=='a'||cha=='A') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='c'||cha=='C'||cha=='b'||cha=='B'||cha=='d'||cha=='D') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a2; }
                case 3:
a3:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.pH of 0.2 N H2SO4 is:");
                    printf("\n\n\t(A) 0.69 \t\t(B) 1.2 \n\t(C) 0.76 \t\t(D) 0.56");
                    cha=getch();
                    if (cha=='a'||cha=='A') { countr++; break; }
                    else if(cha=='c'||cha=='C'||cha=='b'||cha=='B'||cha=='d'||cha=='D') { countr-=.1; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a3; }
                case 4:
a4:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.Catalytic oxidation of ammonia in air forms:");
                    printf("\n\n\t(A)NO2 \t\t(B) N2O5 \n\t(C) N2O \t\t(D) NO");
                    cha=getch();
                    if (cha=='d'||cha=='D') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='a'||cha=='A'||cha=='b'||cha=='B'||cha=='c'||cha=='C') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a4; }
                case 5:
a5:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.The magnetism of the magnet is due to:");
                    printf("\n\n\t(A) the spin motion of electron \n\t(B) cosmic ray \n\t(C) the earth \n\t(D) pressure of big magnet inside the earth ");
                    cha=getch();
                    if (cha=='a'||cha=='A') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='d'||cha=='D'||cha=='b'||cha=='B'||cha=='c'||cha=='C') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a5; }
                case 6:
a6:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.The maximum percentage of ingredients in cement is that of:");
                    printf("\n\n\t(A) magnesia \t(B) lime \n\t(C) alumina \t(D) silica");
                    cha=getch();
                    if (cha=='b'||cha=='B') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='a'||cha=='A'||cha=='d'||cha=='D'||cha=='c'||cha=='C') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a6; }
                case 7:
a7:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.The second derivative of f(x) = 1/x at point (1,1) is equal to:");
                    printf("\n\n\t(A) 1 \t(B) -1 \n\t(C) 2 \t(D) -2");
                    cha=getch();
                    if (cha=='c'||cha=='C') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='a'||cha=='A'||cha=='b'||cha=='B'||cha=='d'||cha=='D') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a7; }
                case 8:
a8:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ.Which of the following is a logic gate?");
                    printf("\n\n\tA.PUT\tB.THEN\n\tC.NOR\tD.WHEN");
                    cha=getch();
                    if (cha=='c'||cha=='C') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='a'||cha=='A'||cha=='b'||cha=='B'||cha=='d'||cha=='D') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a8; }
                case 9:
a9:                 CLEAR_SCREEN();
                    printf("\n\n\n\tQ: If the line 2x+3y+4+k(-x+y+5)=0 is horizontal then the value of k is:");
                    printf("\n\n\t(A) 0 \t(B) 1 \n\t(C) 3 \t(D) 2");
                    cha=getch();
                    if (cha=='d'||cha=='D') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='a'||cha=='A'||cha=='b'||cha=='B'||cha=='c'||cha=='C') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a9; }
                case 0:
a10:                CLEAR_SCREEN();
                    printf("\n\n\tQ: IUPAC name of CH3-CH2-CH2-CH(OCH3)-COBr is:");
                    printf("\n\t(A) 2-methoxylpentanoyl bromide \t(B) 3-methoxylpentanoyl bromide \n\t(C) 3-methoxyhexanoyl bromide \t(D) 2-methoxyhexanoyl bromide");
                    cha=getch();
                    if (cha=='a'||cha=='A') { countr++; break; }
                    else if(cha=='s'||cha=='S') { break; }
                    else if(cha=='b'||cha=='B'||cha=='c'||cha=='C'||cha=='d'||cha=='D') { countr-=.1; break; }
                    else { printf("\n\n\n\n\t\t\t\t<error in choosing option:>"); getch(); goto a10; }
                default: break;
                }
            }
            rewind(fp);
            while((fread(&s,siz,1,fp))==1)
            {
                if(s.ID==tempRoll)
                {
                    fseek(fp,-siz,SEEK_CUR);
                    s.Mark=countr;
                    s.n=1;
                    fwrite(&s,sizeof(s),1,fp);
                    break;
                }
            }
            CLEAR_SCREEN();
            printf("\n\n\n\n\t\t\tCongratulations %s you scored=%f\n",s.name,s.Mark);
            printf("\t\t\tBest of luck for result\n");
            printf("\t\t\tpress any key to go homepage\t");
            getch();
        }
        else
        {
            printf("\n\t\t\t EITHER YOU HAVE NOT REGISTERED YET OR YOU HAVE ALREADY GIVEN EXAM\n");
            getch();
        }
    }
}

void modify(FILE * fp)
{
    printHead();
    Student s;
    int flag=0,tempRoll,siz=sizeof(s);

    printf("\n\n\t Precaution:one shouldn't enter a string character while entering ID number");
    printf("\n\n\tEnter  identification document  Number of Student to MODIFY the Record : ");
    scanf("%d",&tempRoll);
    while(getchar()!='\n');

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
        fseek(fp,-siz,SEEK_CUR);
        printf("\n\n\t\tRECORD FOUND");
        printf("\n\n\t\tEnter New Data for the Record");

        printf("\n\n\t\tEnter Full Name of Student\t");
        fgets(s.name,100,stdin);
        s.name[strcspn(s.name, "\n")] = '\0';

        printf("\n\n\t\tEnter date of birth of Student\t");
        fgets(s.dob,20,stdin);
        s.dob[strcspn(s.dob, "\n")] = '\0';

        printf("\n\n\t\tEnter Address\t");
        fgets(s.address,50,stdin);
        s.address[strcspn(s.address, "\n")] = '\0';

        printf("\n\n\t\tEnter student phone number\t");
        fgets(s.ph,20,stdin);
        s.ph[strcspn(s.ph, "\n")] = '\0';

        printf("\n\n\t\tEnter email ID Student\t");
        fgets(s.email,100,stdin);
        s.email[strcspn(s.email, "\n")] = '\0';

        printf("\n\n\t\tEnter guardian name of Student\t");
        fgets(s.gname,100,stdin);
        s.gname[strcspn(s.gname, "\n")] = '\0';

        printf("\n\n\t\tEnter student's guardian phone number\t");
        fgets(s.gph,20,stdin);
        s.gph[strcspn(s.gph, "\n")] = '\0';
        fwrite(&s,sizeof(s),1,fp);
    }
    else
    {
        printf("\n\n\t!!!! ERROR !!!! RECORD NOT FOUND");
    }
    printf("\n\n\t");
    PAUSE();
}

void displayList(FILE * fp)
{
    printHead();
    Student s;
    int siz=sizeof(s);

    rewind(fp);
    while((fread(&s,siz,1,fp))==1)
    {
        printf("\n\t\tNAME : %s",s.name);
        printf("\n\t\tDATE OF BIRTH:%s",s.dob);
        printf("\n\t\tADDRESS: %s",s.address);
        printf("\n\t\tIDENTIFICATION DOCUMENT NO.:%d",s.ID);
        printf("\n\t\tPHONE NUMBER:%s",s.ph);
        if (s.n!=0)
            printf("\n\t\tmark :%f",s.Mark);
        printf("\n\t\tGUARDIAN NAME:%s",s.gname);
        printf("\n\t\tEMAIL ADDRESS OF STUDENT:%s",s.email);
        printf("\n\t\tGUARDIAN PHONE NUMBER:%s",s.gph);
        printf("\n\n\t\t");
        printChar('x',40);
        printf("\n\n");
    }
    printf("\n\n\n\t");
    printChar('*',65);
    printf("\n\n\t");
    PAUSE();
}

void searchRecord(FILE *fp)
{
    printHead();
    int tempRoll,flag,siz=sizeof(Student);
    Student s;
    char another='y';

    while(another=='y'||another=='Y')
    {
        flag = 0;
        printf("\n\n\tEnter Identification Number of Student to search the record : ");
        scanf("%d",&tempRoll);
        while(getchar()!='\n');

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
            printf("\n\t\tNAME : %s",s.name);
            printf("\n\t\tDATE OF BIRTH:%s",s.dob);
            printf("\n\t\tADDRESS: %s",s.address);
            printf("\n\t\tIDENTIFICATION DOCUMENT NO.: %d",s.ID);
            printf("\n\t\tPHONE NUMBER:%s",s.ph);
            printf("\n\t\tGUARDIAN NAME:%s",s.gname);
            printf("\n\t\tEMAIL OF STUDENT:%s",s.email);
            printf("\n\t\tGUARDIAN PHONE NUMBER:%s",s.gph);
        }
        else printf("\n\n\t\t!!!! ERROR RECORD NOT FOUND !!!!");
        printf("\n");
        printChar('~',65);
        printf("\n\n\t\tWant to enter another search (Y/N)");
        another=getch();
        printf("\n");
    }
}