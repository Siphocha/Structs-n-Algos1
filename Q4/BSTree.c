#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define MAX_LINE 150

//Initial node holding students
typedef struct Student {
    char first_name[MAX_NAME];
    char last_name[MAX_NAME];
    int grade;
} Student;

//Initial node holding BST base case(foundation)node.
typedef struct BSTNode {
    Student student;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

//Function prototypes
BSTNode* create_bst_node(Student student);
BSTNode* insert_bst(BSTNode* root, Student student);
BSTNode* build_bst_from_file(const char* filename);
BSTNode* search_bst(BSTNode* root, const char* last_name);
void search_student(BSTNode* root);
void print_student(Student student);
void free_bst(BSTNode* root);
void to_lower_case(char* str);
void print_in_order(BSTNode* root);

int main() {
    printf("!!!!! Student BST Search System !!!!!\n");

    //Build BST from file
    BSTNode* root = build_bst_from_file("students.txt");
    if (root == NULL) {
        printf("students.text is missing\n");
        return 1;
    }

    printf("BST successfully built\n");

    int choice;
    while (1) {
        printf("\n!!!!! Menu !!!!!\n");
        printf("1. Search student by last name\n");
        printf("2. View all students (in-order traversal)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                search_student(root);
                break;
            case 2:
                printf("\nAll students (sorted by last name):\n");
                print_in_order(root);
                break;
            case 3:
                printf("Program Complete\n");
                free_bst(root);
                return 0;
            default:
                printf("Invalid choice! Pick from options\n");
        }
    }

    free_bst(root);
    return 0;
}

//Creation of new BST node
BSTNode* create_bst_node(Student student) {
    BSTNode* new_node = (BSTNode*)malloc(sizeof(BSTNode));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_node->student = student;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

//Last name as key to insert student info
BSTNode* insert_bst(BSTNode* root, Student student) {
    if (root == NULL) {
        return create_bst_node(student);
    }

    //ALL LAST NAMES TO LOWERCASE!
    char current_last_name[MAX_NAME];
    char new_last_name[MAX_NAME];

    strcpy(current_last_name, root->student.last_name);
    strcpy(new_last_name, student.last_name);

    to_lower_case(current_last_name);
    to_lower_case(new_last_name);

    int cmp = strcmp(new_last_name, current_last_name);

    if (cmp < 0) {
        root->left = insert_bst(root->left, student);
    } else if (cmp > 0) {
        root->right = insert_bst(root->right, student);
    } else {
        root->right = insert_bst(root->right, student);
    }

    return root;
}

//Build BST from file
BSTNode* build_bst_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    BSTNode* root = NULL;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;

        Student student;
        //parse file with last name first name and grade for completion.
        if (sscanf(line, "%s %s %d",
                  student.first_name,
                  student.last_name,
                  &student.grade) == 3) {
            root = insert_bst(root, student);
        }
    }

    fclose(file);
    return root;
}

//Searching BST fro student last name.
BSTNode* search_bst(BSTNode* root, const char* last_name) {
    if (root == NULL) return NULL;

    char current_last_name[MAX_NAME];
    char search_last_name[MAX_NAME];

    strcpy(current_last_name, root->student.last_name);
    strcpy(search_last_name, last_name);

    to_lower_case(current_last_name);
    to_lower_case(search_last_name);

    int cmp = strcmp(search_last_name, current_last_name);

    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return search_bst(root->left, last_name);
    } else {
        return search_bst(root->right, last_name);
    }
}

//Interactive search function
void search_student(BSTNode* root) {
    char last_name[MAX_NAME];
    printf("Enter last name to search: ");
    scanf("%s", last_name);

    BSTNode* result = search_bst(root, last_name);
    if (result != NULL) {
        printf("\nStudent found!\n");
        print_student(result->student);

        //Check if we missed students.
        printf("\nChecking for other students with same last name...\n");
        int count = 0;

        //Now check left subtree using last names too
        BSTNode* temp = result->left;
        while (temp != NULL) {
            char temp_last_name[MAX_NAME];
            strcpy(temp_last_name, temp->student.last_name);
            to_lower_case(temp_last_name);

            char search_name[MAX_NAME];
            strcpy(search_name, last_name);
            to_lower_case(search_name);

            if (strcmp(temp_last_name, search_name) == 0) {
                print_student(temp->student);
                count++;
            }
            temp = temp->left;
        }

        //Searching right subtree for same last name
        temp = result->right;
        while (temp != NULL) {
            char temp_last_name[MAX_NAME];
            strcpy(temp_last_name, temp->student.last_name);
            to_lower_case(temp_last_name);

            char search_name[MAX_NAME];
            strcpy(search_name, last_name);
            to_lower_case(search_name);

            if (strcmp(temp_last_name, search_name) == 0) {
                print_student(temp->student);
                count++;
            }
            temp = temp->right;
        }

        if (count > 0) {
            printf("Found %d additional student(s) with the same last name.\n", count);
        }

    } else {
        printf("Student with last name '%s' not found.\n", last_name);
    }
}

//Finally output student info
void print_student(Student student) {
    printf("Name is: %s %s, Grade: %d\n",
           student.first_name, student.last_name, student.grade);
}

//Freeing memory that was used for BST
void free_bst(BSTNode* root) {
    if (root == NULL) return;
    free_bst(root->left);
    free_bst(root->right);
    free(root);
}

//Converting string to lowercase
void to_lower_case(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

//Print BST in-order (sorted by last name)
void print_in_order(BSTNode* root) {
    if (root == NULL) return;
    print_in_order(root->left);
    print_student(root->student);
    print_in_order(root->right);
}