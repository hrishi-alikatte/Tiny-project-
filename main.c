#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_PROGRAM_LENGTH 50
#define MAX_DEPARTMENT_LENGTH 50
#define MAX_STUDENTS_PER_PROFESSOR 10
#define MAX_STUDENTS 100
#define MAX_PROFESSORS 20

// Structure definitions
struct Student {
    int id;
    char name[MAX_NAME_LENGTH];
    char program[MAX_PROGRAM_LENGTH];
    float gpa;
};

struct Professor {
    int id;
    char name[MAX_NAME_LENGTH];
    char department[MAX_DEPARTMENT_LENGTH];
    int supervised_students[MAX_STUDENTS_PER_PROFESSOR];
    int num_supervised_students;
};

// Global variables
struct Student** students;
struct Professor** professors;
int student_count = 0;
int professor_count = 0;

// Utility function prototypes
void clearInputBuffer();
int getValidInt(const char* prompt);
float getValidFloat(const char* prompt, float min, float max);
void getString(const char* prompt, char* buffer, int max_length);
int findStudentById(int id);
int findProfessorById(int id);
void addStudent();
void displayAllStudents();
void searchStudent();
void modifyStudent();
void deleteStudent();
void addProfessor();
void displayAllProfessors();
void assignStudentToProfessor();
void displayStudentsByProfessor();
void sortStudents();
void saveDataToFile();
void loadDataFromFile();
void displayMenu();
void freeMemory();

// Main function
int main() {
    students = (struct Student**)calloc(MAX_STUDENTS, sizeof(struct Student*));
    professors = (struct Professor**)calloc(MAX_PROFESSORS, sizeof(struct Professor*));

    loadDataFromFile();

    int choice;
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: modifyStudent(); break;
            case 5: deleteStudent(); break;
            case 6: addProfessor(); break;
            case 7: displayAllProfessors(); break;
            case 8: assignStudentToProfessor(); break;
            case 9: displayStudentsByProfessor(); break;
            case 10: sortStudents(); break;
            case 11: saveDataToFile(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n"); break;
        }
    } while (choice != 0);

    freeMemory();
    return 0;
}

// Utility Functions
void clearInputBuffer() {
    while (getchar() != '\n');
}

int getValidInt(const char* prompt) {
    int value;
    printf("%s", prompt);
    scanf("%d", &value);
    clearInputBuffer();
    return value;
}

float getValidFloat(const char* prompt, float min, float max) {
    float value;
    do {
        printf("%s (%.2f - %.2f): ", prompt, min, max);
        scanf("%f", &value);
        clearInputBuffer();
    } while (value < min || value > max);
    return value;
}

void getString(const char* prompt, char* buffer, int max_length) {
    printf("%s", prompt);
    fgets(buffer, max_length, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
}

// Core Functionalities
int findStudentById(int id) {
    for (int i = 0; i < student_count; ++i) {
        if (students[i] && students[i]->id == id) return i;
    }
    return -1;
}

int findProfessorById(int id) {
    for (int i = 0; i < professor_count; ++i) {
        if (professors[i] && professors[i]->id == id) return i;
    }
    return -1;
}

void addStudent() {
    if (student_count >= MAX_STUDENTS) {
        printf("Student limit reached.\n");
        return;
    }

    struct Student* s = (struct Student*)malloc(sizeof(struct Student));
    s->id = getValidInt("Enter student ID: ");
    getString("Enter student name: ", s->name, MAX_NAME_LENGTH);
    getString("Enter program: ", s->program, MAX_PROGRAM_LENGTH);
    s->gpa = getValidFloat("Enter GPA", 0.0, 4.0);

    students[student_count++] = s;
    printf("Student added successfully.\n");
}

void displayAllStudents() {
    if (student_count == 0) {
        printf("No students to display.\n");
        return;
    }
    for (int i = 0; i < student_count; ++i) {
        struct Student* s = students[i];
        printf("\nID: %d\nName: %s\nProgram: %s\nGPA: %.2f\n", s->id, s->name, s->program, s->gpa);
    }
}

void searchStudent() {
    int id = getValidInt("Enter student ID to search: ");
    int index = findStudentById(id);
    if (index >= 0) {
        struct Student* s = students[index];
        printf("Found - ID: %d | Name: %s | Program: %s | GPA: %.2f\n", s->id, s->name, s->program, s->gpa);
    } else {
        printf("Student not found.\n");
    }
}

void modifyStudent() {
    int id = getValidInt("Enter student ID to modify: ");
    int index = findStudentById(id);
    if (index >= 0) {
        struct Student* s = students[index];
        getString("Enter new name: ", s->name, MAX_NAME_LENGTH);
        getString("Enter new program: ", s->program, MAX_PROGRAM_LENGTH);
        s->gpa = getValidFloat("Enter new GPA", 0.0, 4.0);
        printf("Student updated.\n");
    } else {
        printf("Student not found.\n");
    }
}

void deleteStudent() {
    int id = getValidInt("Enter student ID to delete: ");
    int index = findStudentById(id);
    if (index >= 0) {
        free(students[index]);
        for (int i = index; i < student_count - 1; ++i) {
            students[i] = students[i + 1];
        }
        student_count--;
        printf("Student deleted.\n");
    } else {
        printf("Student not found.\n");
    }
}

void addProfessor() {
    if (professor_count >= MAX_PROFESSORS) {
        printf("Professor limit reached.\n");
        return;
    }

    struct Professor* p = (struct Professor*)malloc(sizeof(struct Professor));
    p->id = getValidInt("Enter professor ID: ");
    getString("Enter professor name: ", p->name, MAX_NAME_LENGTH);
    getString("Enter department: ", p->department, MAX_DEPARTMENT_LENGTH);
    p->num_supervised_students = 0;

    professors[professor_count++] = p;
    printf("Professor added successfully.\n");
}

void displayAllProfessors() {
    if (professor_count == 0) {
        printf("No professors to display.\n");
        return;
    }
    for (int i = 0; i < professor_count; ++i) {
        struct Professor* p = professors[i];
        printf("\nID: %d\nName: %s\nDepartment: %s\nSupervising %d students\n", 
               p->id, p->name, p->department, p->num_supervised_students);
    }
}

void assignStudentToProfessor() {
    int prof_id = getValidInt("Enter professor ID: ");
    int student_id = getValidInt("Enter student ID to assign: ");

    int p_index = findProfessorById(prof_id);
    int s_index = findStudentById(student_id);

    if (p_index >= 0 && s_index >= 0) {
        struct Professor* p = professors[p_index];
        if (p->num_supervised_students >= MAX_STUDENTS_PER_PROFESSOR) {
            printf("Professor already supervises max number of students.\n");
            return;
        }
        p->supervised_students[p->num_supervised_students++] = student_id;
        printf("Student assigned.\n");
    } else {
        printf("Professor or student not found.\n");
    }
}

void displayStudentsByProfessor() {
    int prof_id = getValidInt("Enter professor ID: ");
    int index = findProfessorById(prof_id);
    if (index < 0) {
        printf("Professor not found.\n");
        return;
    }
    struct Professor* p = professors[index];
    printf("Professor %s supervises:\n", p->name);
    for (int i = 0; i < p->num_supervised_students; ++i) {
        int student_id = p->supervised_students[i];
        int s_index = findStudentById(student_id);
        if (s_index >= 0) {
            struct Student* s = students[s_index];
            printf(" - %s (ID: %d, GPA: %.2f)\n", s->name, s->id, s->gpa);
        }
    }
}

void sortStudents() {
    for (int i = 0; i < student_count - 1; ++i) {
        for (int j = i + 1; j < student_count; ++j) {
            if (students[i]->gpa < students[j]->gpa) {
                struct Student* temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    printf("Students sorted by GPA (descending).\n");
}

void saveDataToFile() {
    FILE* f = fopen("students.dat", "wb");
    fwrite(&student_count, sizeof(int), 1, f);
    for (int i = 0; i < student_count; ++i)
        fwrite(students[i], sizeof(struct Student), 1, f);
    fclose(f);

    f = fopen("professors.dat", "wb");
    fwrite(&professor_count, sizeof(int), 1, f);
    for (int i = 0; i < professor_count; ++i)
        fwrite(professors[i], sizeof(struct Professor), 1, f);
    fclose(f);

    printf("Data saved.\n");
}

void loadDataFromFile() {
    FILE* f = fopen("students.dat", "rb");
    if (f) {
        fread(&student_count, sizeof(int), 1, f);
        for (int i = 0; i < student_count; ++i) {
            students[i] = (struct Student*)malloc(sizeof(struct Student));
            fread(students[i], sizeof(struct Student), 1, f);
        }
        fclose(f);
    }

    f = fopen("professors.dat", "rb");
    if (f) {
        fread(&professor_count, sizeof(int), 1, f);
        for (int i = 0; i < professor_count; ++i) {
            professors[i] = (struct Professor*)malloc(sizeof(struct Professor));
            fread(professors[i], sizeof(struct Professor), 1, f);
        }
        fclose(f);
    }
}

void freeMemory() {
    for (int i = 0; i < student_count; ++i)
        free(students[i]);
    for (int i = 0; i < professor_count; ++i)
        free(professors[i]);
    free(students);
    free(professors);
}

void displayMenu() {
    printf("\n====== MENU ======\n");
    printf("1. Add Student\n");
    printf("2. Display All Students\n");
    printf("3. Search Student\n");
    printf("4. Modify Student\n");
    printf("5. Delete Student\n");
    printf("6. Add Professor\n");
    printf("7. Display All Professors\n");
    printf("8. Assign Student to Professor\n");
    printf("9. Display Students by Professor\n");
    printf("10. Sort Students by GPA\n");
    printf("11. Save Data to File\n");
    printf("0. Exit\n");
}
