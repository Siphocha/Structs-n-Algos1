#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME 50
#define MAX_STUDENTS 100

typedef struct {
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];
    int grade;
} Student;

// Function prototypes
int read_students(const char *filename, Student students[]);
void quick_sort(Student students[], int low, int high);
int partition(Student students[], int low, int high);
void write_sorted_students(const char *filename, Student students[], int count);
void swap(Student *a, Student *b);

int main() {
    Student students[MAX_STUDENTS];
    int student_count;
    clock_t start, end;
    double cpu_time_used;

    //Simple read from the original file
    student_count = read_students("students.txt", students);
    if (student_count == 0) {
        printf("No students found\n");
        return 1;
    }

    printf("Read %d students from file.\n", student_count);

    //This is specifcially for measuring sorting time
    start = clock();
    quick_sort(students, 0, student_count - 1);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    //Sorted students to file
    write_sorted_students("output_1.txt", students, student_count);

    printf("Completed in %.6f seconds.\n", cpu_time_used);
    printf("Data is in output_1.txt\n");

    return 0;
}

int read_students(const char *filename, Student students[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    int count = 0;
    char line[150];

    //Handling that line size and count. As well as edge cases with new lines.
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_STUDENTS) {
        line[strcspn(line, "\n")] = 0;

        char first_name[MAX_NAME];
        char last_name[MAX_NAME];
        int grade;

        if (sscanf(line, "%s %s %d", first_name, last_name, &grade) == 3) {
            strcpy(students[count].first_name, first_name);
            strcpy(students[count].last_name, last_name);
            students[count].grade = grade;
            count++;
        }
    }
    fclose(file);
    return count;
}

//Quick sort algorithm to cut array and recursive sort after
void quick_sort(Student students[], int low, int high) {
    if (low < high) {

        int pi = partition(students, low, high);

        quick_sort(students, low, pi - 1);
        quick_sort(students, pi + 1, high);
    }
}

//Selective partitionin for full order
int partition(Student students[], int low, int high) {
    int pivot = students[high].grade;
    int i = (low - 1);  //Index the smallest integer

    for (int n = low; n <= high - 1; n++) {
        if (students[n].grade <= pivot) {
            i++;
            swap(&students[i], &students[n]);
        }
    }
    swap(&students[i + 1], &students[high]);
    return (i + 1);
}

void swap(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

//Recursively go through adn reorder what was there properly for output
void write_sorted_students(const char *filename, Student students[], int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening output file");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %d\n",
                students[i].first_name,
                students[i].last_name,
                students[i].grade);
    }

    fclose(file);
}