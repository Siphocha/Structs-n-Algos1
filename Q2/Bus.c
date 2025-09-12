#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//Bus stop will be its own node. Everything else branches out.
typedef struct BusStop {
    char name[50];
    struct BusStop* prev;
    struct BusStop* next;
} BusStop;

//Function prototypes
BusStop* create_bus_stop(const char* name);
void add_stop(BusStop** head, BusStop** tail, const char* name);
void display_forward(BusStop* head);
void display_backward(BusStop* tail);
void nav_forward(BusStop* head);
void nav_backward(BusStop* tail);
void free_list(BusStop* head);

int main() {
    BusStop* head = NULL;
    BusStop* tail = NULL;
    int choice;
    char stop_name[50];

    printf("!!!!! Bus-Stop Nav System !!!!!\n");

    //PRE adding bus stops for my own sanity.
    add_stop(&head, &tail, "Home");
    add_stop(&head, &tail, "Musoro-Stop-1");
    add_stop(&head, &tail, "Big-Hill-1");
    add_stop(&head, &tail, "ALU Entrance");
    add_stop(&head, &tail, "Campus");

    while (1) {
        printf("\nMenu:\n");
        printf("1. New bus stop\n");
        printf("2. All forward Bus stops\n");
        printf("3. All backward Bus stops\n");
        printf("4. 3-Second Delay Forwards\n");
        printf("5. 3-Second Delay Backwards\n");
        printf("6. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("New Bus stop name: ");
                fgets(stop_name, sizeof(stop_name), stdin);
                stop_name[strcspn(stop_name, "\n")] = 0; // Remove newline
                add_stop(&head, &tail, stop_name);
                printf("Bus stop '%s' added successfully!\n", stop_name);
                break;

            case 2:
                printf("\nBus stops in forward order:\n");
                display_forward(head);
                break;

            case 3:
                printf("\nBus stops in backward order:\n");
                display_backward(tail);
                break;

            case 4:
                printf("\nGOING FORWARDS\n");
                nav_forward(head);
                break;

            case 5:
                printf("\nGOING BACKWARDS\n");
                nav_backward(tail);
                break;

            case 6:
                printf("Program Complete. See you!\n");
                free_list(head);
                return 0;

            default:
                printf("Invalid Choice! Try again.\n");
        }
    }
    return 0;
}

//New bus stop node
BusStop* create_bus_stop(const char* name) {
    BusStop* new_stop = (BusStop*)malloc(sizeof(BusStop));
    if (new_stop == NULL) {
        //just incase node creation fails
        printf("Memory Error...Somehow!\n");
        exit(1);
    }
    strcpy(new_stop->name, name);
    new_stop->prev = NULL;
    new_stop->next = NULL;
    return new_stop;
}

//Add a new bus stop at the end of the list
void add_stop(BusStop** head, BusStop** tail, const char* name) {
    BusStop* new_stop = create_bus_stop(name);

    if (*head == NULL) {
        *head = new_stop;
        *tail = new_stop;
    } else {
        (*tail)->next = new_stop;
        new_stop->prev = *tail;
        *tail = new_stop;
    }
}

//All bus stops in sequential order
void display_forward(BusStop* head) {
    BusStop* current = head;
    int count = 1;

    if (head == NULL) {
        printf("No bus stops currently!\n");
        return;
    }

    while (current != NULL) {
        printf("%d. %s\n", count++, current->name);
        current = current->next;
    }
}

//All stops in backward order
void display_backward(BusStop* tail) {
    BusStop* current = tail;
    int count = 1;

    if (tail == NULL) {
        printf("No bus stops available!\n");
        return;
    }

    while (current != NULL) {
        printf("%d. %s\n", count++, current->name);
        current = current->prev;
    }
}

//3-Second delay nav forward
void nav_forward(BusStop* head) {
    BusStop* current = head;
    int count = 1;

    if (head == NULL) {
        printf("No bus stops to navigate!\n");
        return;
    }

    while (current != NULL) {
        printf("Stop %d: %s\n", count++, current->name);
        if (current->next != NULL) {
            printf("Next stop in 3 seconds...\n");
            sleep(3);
        } else {
            printf("Final Destination Reached!\n");
        }
        current = current->next;
    }
}

// Navigate backward with 3-second delay
void nav_backward(BusStop* tail) {
    BusStop* current = tail;
    int count = 1;

    if (tail == NULL) {
        printf("No bus stops to navigate!\n");
        return;
    }

    while (current != NULL) {
        printf("Stop %d: %s\n", count++, current->name);
        if (current->prev != NULL) {
            printf("Moving to previous stop in 3 seconds...\n");
            sleep(3);
        } else {
            printf("Reached starting point!\n");
        }
        current = current->prev;
    }
}

//Let go of all that memory. Let go of the memories!!!
void free_list(BusStop* head) {
    BusStop* current = head;
    BusStop* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}