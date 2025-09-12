#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//TreeNode to hold it all
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

//Function prototypes AGAIN this is my C life now.
TreeNode* create_node(int data);
TreeNode* build_tree_from_array(int arr[], int size);
void print_tree(TreeNode* root, int space);
TreeNode* find_node(TreeNode* root, int value);
void print_leaf_nodes(TreeNode* root);
void print_siblings(TreeNode* root, int value);
void print_parent(TreeNode* root, int value);
void print_grandchildren(TreeNode* root, int value);
void free_tree(TreeNode* root);

int main() {
    //No dynamic entry this time. More interesting froma  set piece on this one.
    int arr[20] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85, 5, 15, 27, 33, 47};

    printf("Binary tree from array... is being created\n");
    TreeNode* root = build_tree_from_array(arr, 20);

    printf("\n!!!!! Binary Tree Structure !!!!!\n");
    print_tree(root, 0);

    printf("\n1. Root(Base) Node: %d\n", root->data);

    printf("\n2. Leaf Nodes: ");
    print_leaf_nodes(root);
    printf("\n");

    int choice;
    while (1) {
        printf("\n!!!!! ACTUAL Menu !!!!!\n");
        printf("3. Find node siblings\n");
        printf("4. Find node parents\n");
        printf("5. grandchildren of a node\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 6) break;

        int value;
        printf("Enter node value: ");
        scanf("%d", &value);

        switch (choice) {
            case 3:
                printf("Siblings of %d: ", value);
                print_siblings(root, value);
                break;
            case 4:
                printf("Parent of %d: ", value);
                print_parent(root, value);
                break;
            case 5:
                printf("Grandchildren of %d: ", value);
                print_grandchildren(root, value);
                break;
            default:
                printf("Invalid choice!\n");
        }
    }

    free_tree(root);
    return 0;
}

//New node tree creation
TreeNode* create_node(int data) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

//Binary tree from  array input.
TreeNode* build_tree_from_array(int arr[], int size) {
    if (size == 0) return NULL;

    TreeNode* root = create_node(arr[0]);
    TreeNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    int i = 1;
    while (i < size) {
        TreeNode* current = queue[front++];

        //left child
        if (i < size) {
            current->left = create_node(arr[i++]);
            queue[rear++] = current->left;
        }

        //right child
        if (i < size) {
            current->right = create_node(arr[i++]);
            queue[rear++] = current->right;
        }
    }

    return root;
}

//Tree in 90 degree printed out format in terminal
void print_tree(TreeNode* root, int space) {
    if (root == NULL) return;

    space += 10;
    print_tree(root->right, space);

    printf("\n");
    for (int i = 10; i < space; i++) printf(" ");
    printf("%d\n", root->data);

    print_tree(root->left, space);
}

//Finding node values
TreeNode* find_node(TreeNode* root, int value) {
    if (root == NULL) return NULL;
    if (root->data == value) return root;

    TreeNode* left = find_node(root->left, value);
    if (left != NULL) return left;

    return find_node(root->right, value);
}

//Print all nodes with no kids (sad life)
void print_leaf_nodes(TreeNode* root) {
    if (root == NULL) return;

    if (root->left == NULL && root->right == NULL) {
        printf("%d ", root->data);
        return;
    }

    print_leaf_nodes(root->left);
    print_leaf_nodes(root->right);
}

//Print nodes on same level "Siblings"
void print_siblings(TreeNode* root, int value) {
    if (root == NULL || root->data == value) {
        printf("No siblings (root node or not found)\n");
        return;
    }

    //Just incase they dont exist, check
    TreeNode* target = find_node(root, value);
    if (target == NULL) {
        printf("Node not found!\n");
        return;
    }

    //Finding parent and getting siblings
    TreeNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        TreeNode* current = queue[front++];

        if (current->left != NULL) {
            if (current->left->data == value) {
                if (current->right != NULL) {
                    printf("%d\n", current->right->data);
                } else {
                    printf("No siblings\n");
                }
                return;
            }
            queue[rear++] = current->left;
        }

        if (current->right != NULL) {
            if (current->right->data == value) {
                if (current->left != NULL) {
                    printf("%d\n", current->left->data);
                } else {
                    printf("No siblings\n");
                }
                return;
            }
            queue[rear++] = current->right;
        }
    }

    printf("Node not found here or anywhere!\n");
}

//Print out your parents (the horror)
void print_parent(TreeNode* root, int value) {
    if (root == NULL || root->data == value) {
        printf("No parent (root node)\n");
        return;
    }

    TreeNode* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        TreeNode* current = queue[front++];

        if (current->left != NULL) {
            if (current->left->data == value) {
                printf("%d\n", current->data);
                return;
            }
            queue[rear++] = current->left;
        }

        if (current->right != NULL) {
            if (current->right->data == value) {
                printf("%d\n", current->data);
                return;
            }
            queue[rear++] = current->right;
        }
    }

    printf("Node not found!\n");
}

//Node grandkids
void print_grandchildren(TreeNode* root, int value) {
    TreeNode* target = find_node(root, value);
    if (target == NULL) {
        printf("Node not found!\n");
        return;
    }

    bool found = false;
    printf("[");

    if (target->left != NULL) {
        if (target->left->left != NULL) {
            printf("%d ", target->left->left->data);
            found = true;
        }
        if (target->left->right != NULL) {
            printf("%d ", target->left->right->data);
            found = true;
        }
    }

    if (target->right != NULL) {
        if (target->right->left != NULL) {
            printf("%d ", target->right->left->data);
            found = true;
        }
        if (target->right->right != NULL) {
            printf("%d ", target->right->right->data);
            found = true;
        }
    }

    if (!found) {
        printf("No grandchildren");
    }
    printf("]\n");
}

//Let my memory go free
void free_tree(TreeNode* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}