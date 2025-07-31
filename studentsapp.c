#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define SUBJECTS 3

typedef struct {
    char name[MAX_NAME];
    int rollNo;
    float marks[SUBJECTS];
    float total;
    float average;
    int isPass;
} Student;

void calculateResult(Student *s) {
    s->total = 0;
    s->isPass = 1;
    for (int i = 0; i < SUBJECTS; i++) {
        s->total += s->marks[i];
        if (s->marks[i] < 35)
            s->isPass = 0;
    }
    s->average = s->total / SUBJECTS;
}

void addStudent(Student **students, int *count) {
    (*count)++;
    *students = realloc(*students, (*count) * sizeof(Student));
    if (*students == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    Student *s = &(*students)[*count - 1];
    printf("Enter name: ");
    scanf(" %[^\n]", s->name);
    printf("Enter roll number: ");
    scanf("%d", &s->rollNo);

    printf("Enter marks for %d subjects:\n", SUBJECTS);
    for (int i = 0; i < SUBJECTS; i++) {
        do {
            printf("Subject %d: ", i + 1);
            scanf("%f", &s->marks[i]);
            if (s->marks[i] < 0 || s->marks[i] > 100) {
                printf("Invalid mark. Enter between 0 and 100.\n");
            }
        } while (s->marks[i] < 0 || s->marks[i] > 100);
    }

    calculateResult(s);
    printf("Student added successfully.\n\n");
}

void printStudent(const Student *s) {
    printf("\nName     : %s\n", s->name);
    printf("Roll No  : %d\n", s->rollNo);
    for (int i = 0; i < SUBJECTS; i++)
        printf("Marks %d  : %.2f\n", i + 1, s->marks[i]);
    printf("Total    : %.2f\n", s->total);
    printf("Average  : %.2f\n", s->average);
    printf("Result   : %s\n", s->isPass ? "PASS" : "FAIL");
    printf("------------------------------\n");
}

void displayAllStudents(Student *students, int count) {
    if (count == 0) {
        printf("No student records to display.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printStudent(&students[i]);
    }
}

void findStudent(Student *students, int count) {
    int roll;
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    for (int i = 0; i < count; i++) {
        if (students[i].rollNo == roll) {
            printStudent(&students[i]);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", roll);
}

void classSummary(Student *students, int count) {
    if (count == 0) {
        printf("No student data available for summary.\n");
        return;
    }

    float totalClassAvg = 0;
    int passCount = 0;
    for (int i = 0; i < count; i++) {
        totalClassAvg += students[i].average;
        if (students[i].isPass)
            passCount++;
    }

    printf("Class Average     : %.2f\n", totalClassAvg / count);
    printf("Pass Percentage   : %.2f%%\n", (float)passCount * 100 / count);
}

void exportToFile(Student *students, int count) {
    FILE *fp = fopen("marksheet.txt", "w+");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    fprintf(fp, "---- Student Marksheet ----\n\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "Name     : %s\n", students[i].name);
        fprintf(fp, "Roll No  : %d\n", students[i].rollNo);
        for (int j = 0; j < SUBJECTS; j++)
            fprintf(fp, "Marks %d  : %.2f\n", j + 1, students[i].marks[j]);
        fprintf(fp, "Total    : %.2f\n", students[i].total);
        fprintf(fp, "Average  : %.2f\n", students[i].average);
        fprintf(fp, "Result   : %s\n", students[i].isPass ? "PASS" : "FAIL");
        fprintf(fp, "------------------------------\n");
    }
    fclose(fp);
    printf("Marksheet exported to 'marksheet.txt'\n");
}

int main() {
    Student *students = NULL;
    int count = 0;
    int choice;

    do {
        printf("\n--- Student Marksheet Menu ---\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Find Student by Roll No\n");
        printf("4. Class Summary\n");
        printf("5. Export Marksheet to File\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(&students, &count); break;
            case 2: displayAllStudents(students, count); break;
            case 3: findStudent(students, count); break;
            case 4: classSummary(students, count); break;
            case 5: exportToFile(students, count); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    free(students);
    return 0;
}
