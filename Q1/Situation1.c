#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_STUDENTS 100

typedef struct {
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    int grade;
} Student;

//Ready made prototypes
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

}