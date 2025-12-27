/* Student Management Project */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NAME_LEN 50
#define COURSE_LEN 50
#define UNI_LEN 10

#define FILE_BUE  "BUE.txt"
#define FILE_AAST "AAST.txt"

/*this info of excel from ai */
#define FILE_ALL  "StudentData.csv"

/* student structure */
typedef struct {
    char uni[UNI_LEN];
    int id;
    char name[NAME_LEN];
    float gpa;
    char course[COURSE_LEN];
} Student;

/* get file size */
long getFileSizeBytes(char fileName[])
{
    FILE* f;
    long size;

    f = fopen(fileName, "r");
    if (f == NULL)
        return 0;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fclose(f);

    return size;
}

/* count students in university file */
int countStudentsInFile(char fileName[])
{
    FILE* f;
    int count = 0;
    int id;
    char name[NAME_LEN];
    float gpa;
    char course[COURSE_LEN];

    f = fopen(fileName, "r");
    if (f == NULL)
        return 0;

    while (fscanf(f, "%d %s %f %s", &id, name, &gpa, course) == 4)
    {
        count++;
    }

    fclose(f);
    return count;
}

/* add student to university file */
void addToUniFile(char fileName[], Student s)
{
    FILE* f = fopen(fileName, "a");
    if (f == NULL)
    {
        return;
    }

    fprintf(f, "%d %s %.2f %s\n", s.id, s.name, s.gpa, s.course);
    fclose(f);
}

/* add student to master CSV */
void addToCSV(Student s)
{
    FILE* f = fopen(FILE_ALL, "a");
    if (f == NULL)
    {
        return;
    }

    fprintf(f, "%s,%d,%s,%.2f,%s\n",
            s.uni, s.id, s.name, s.gpa, s.course);

    fclose(f);
}

/* load students from CSV */
int loadCSV(Student* students, int max)
{
    FILE* f = fopen(FILE_ALL, "r");
    int count = 0;

    if (f == NULL)
        return 0;

    while (count < max &&
           fscanf(f, "%9[^,],%d,%49[^,],%f,%49[^\n]",
                  students[count].uni,
                  &students[count].id,
                  students[count].name,
                  &students[count].gpa,
                  students[count].course) == 5)
    {
        count++;
    }

    fclose(f);
    return count;
}

/* display one student */
void displayOne(Student* students, int i)
{
    printf("University: %s\n", students[i].uni);
    printf("ID: %d\n", students[i].id);
    printf("Name: %s\n", students[i].name);
    printf("GPA: %.2f\n", students[i].gpa);
    printf("Course: %s\n", students[i].course);
}

/* filter students by university */
int filterUni(Student* students, int total, Student* filtered, int uniChoice)
{
    int i;
    int count = 0;

    for (i = 0; i < total; i++)
    {
        if (uniChoice == 1)
        {
            if (strcmp(students[i].uni, "BUE") == 0)
            {
                filtered[count] = students[i];
                count++;
            }
        }
        else if (uniChoice == 2)
        {
            if (strcmp(students[i].uni, "AAST") == 0)
            {
                filtered[count] = students[i];
                count++;
            }
        }
        else if (uniChoice == 3)
        {
            filtered[count] = students[i];
            count++;
        }
    }

    return count;
}

/* display list of students */
void display(Student* students, int total, char title[])
{
    int i;

    printf("\n%s\n", title);
    printf("Number of students: %d\n", total);

    for (i = 0; i < total; i++)
    {
        printf("\nStudent %d\n", i + 1);
        displayOne(students, i);
    }
}

/* sort students by GPA (bubble sort) */
void sortByGPA(Student* students, int total)
{
    int i, j;
    Student temp;

    for (i = 0; i < total - 1; i++)
    {
        for (j = 0; j < total - 1 - i; j++)
        {
            if (students[j].gpa > students[j + 1].gpa)
            {
                temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}

/* search GPA */
void searchGPA(Student* students, int total, float gpa)
{
    int i;
    int found = 0;

    for (i = 0; i < total; i++)
    {
        if (fabs(students[i].gpa - gpa) < 0.01)
        {
            displayOne(students, i);
            found = 1;
        }
    }

    if (!found)
        printf("No student found with GPA %.2f\n", gpa);
}

/* add new student */
void addStudent(int capacity)
{
    Student s;
    int choice;
    int current;

    printf("1) BUE\n2) AAST\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        strcpy(s.uni, "BUE");
        current = countStudentsInFile(FILE_BUE);
    }
    else if (choice == 2)
    {
        strcpy(s.uni, "AAST");
        current = countStudentsInFile(FILE_AAST);
    }
    else
        return;

    if (current >= capacity)
    {
        printf("File is full\n");
        return;
    }

    printf("ID: ");
    scanf("%d", &s.id);

    printf("Name: ");
    scanf("%s", s.name);

    printf("GPA: ");
    scanf("%f", &s.gpa);

    printf("Course: ");
    scanf("%s", s.course);

    if (choice == 1)
        addToUniFile(FILE_BUE, s);
    else
        addToUniFile(FILE_AAST, s);

    addToCSV(s);
}

/* menu */
void menu()
{
    printf("\n1) Add student\n");
    printf("2) Display students\n");
    printf("3) Search GPA\n");
    printf("4) File sizes\n");
    printf("5) Exit\n");
}

/* main */
int main()
{
    Student* students;
    Student* filtered;
    int capacity;
    int max;
    int total = 0;
    int choice;
    int uniChoice;
    float gpa;

    printf("Enter capacity per university: ");
    scanf("%d", &capacity);

    max = capacity * 2;
    students = (Student*)malloc(max * sizeof(Student));

    total = loadCSV(students, max);

    do
    {
        menu();
        scanf("%d", &choice);

        if (choice == 1)
        {
            addStudent(capacity);
            total = loadCSV(students, max);
        }
        else if (choice == 2)
        {
            filtered = (Student*)malloc(total * sizeof(Student));
            printf("1) BUE  2) AAST  3) BOTH: ");
            scanf("%d", &uniChoice);

            int c = filterUni(students, total, filtered, uniChoice);
            display(filtered, c, "Students");
            free(filtered);
        }
        else if (choice == 3)
        {
            printf("Enter GPA: ");
            scanf("%f", &gpa);
            searchGPA(students, total, gpa);
        }
        else if (choice == 4)
        {
            printf("BUE.txt size: %ld bytes\n", getFileSizeBytes(FILE_BUE));
            printf("AAST.txt size: %ld bytes\n", getFileSizeBytes(FILE_AAST));
            printf("CSV size: %ld bytes\n", getFileSizeBytes(FILE_ALL));
        }

    } while (choice != 5);

    free(students);
    return 0;
}
