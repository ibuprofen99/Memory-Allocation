#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>

struct Block {
    int id;
    int start;
    int end;
    struct Block* next;
};

struct Block* head = NULL;
int pm_size;
int algorithm;

void enterParameters() {
    printf("Enter size of physical memory: ");
    scanf("%d", &pm_size);
    printf("Enter hole-fitting algorithm (0=first fit, 1=best fit): ");
    scanf("%d", &algorithm);
}

void printMemoryAllocation() {
    printf("ID   Start   End\n");
    printf("-----------------\n");
    struct Block* current = head;
    while (current != NULL) {
        printf("%-5d%-8d%-8d\n", current->id, current->start, current->end);
        current = current->next;
    }
}

void allocateMemory(int algorithm) {
    if (algorithm == 0) {
        int id, size;
        printf("Enter block id: ");
        scanf("%d", &id);
        printf("Enter block size: ");
        scanf("%d", &size);

        struct Block* current = head;
        while (current != NULL) {
            if (current->id == id) {
                printf("Error: Duplicate block id.\n");
                return;
            }
            current = current->next;
        }

        current = head;
        struct Block* prev = NULL;
        struct Block* newBlock = (struct Block*)malloc(sizeof(struct Block));
        newBlock->id = id;
        newBlock->next = NULL;

        if (current == NULL) {
            if (size <= pm_size) {
                newBlock->start = 0;
                newBlock->end = size;
                head = newBlock;
                printf("\n");
            }
            else {
                printf("Error: Not enough space to allocate memory.\n");
                free(newBlock);
            }
            return;
        }


        if (current->start >= size) {
            newBlock->start = 0;
            newBlock->end = size;
            newBlock->next = current;
            head = newBlock;
            printf("\n");
            return;
        }

        while (current != NULL && current->next != NULL) {
            if (current->next->start - current->end >= size) {
                newBlock->start = current->end;
                newBlock->end = newBlock->start + size;

                newBlock->next = current->next;
                current->next = newBlock;

                printf("\n");
                return;
            }

            prev = current;
            current = current->next;
        }

        if (pm_size - current->end >= size) {
            newBlock->start = current->end;
            newBlock->end = newBlock->start + size;
            current->next = newBlock;

            printf("\n");
            return;
        }

        printf("Error: Not enough space to allocate memory.\n");
        free(newBlock);
    }
    else if (algorithm == 1)
    {
        int id, size;
        printf("Enter block id: ");
        scanf("%d", &id);
        printf("Enter block size: ");
        scanf("%d", &size);

        struct Block* current = head;
        while (current != NULL) {
            if (current->id == id) {
                printf("Error: Duplicate block id.\n");
                return;
            }
            current = current->next;
        }

        // Allocate memory to the last block
        current = head;
        struct Block* prev = NULL;
        struct Block* newBlock = (struct Block*)malloc(sizeof(struct Block));
        newBlock->id = id;
        newBlock->next = NULL;

        // Check if there are no allocated blocks
        if (current == NULL) {
            if (size <= pm_size) {
                newBlock->start = 0;
                newBlock->end = size;
                head = newBlock;
                printf("\n");
            }
            else {
                printf("Error: Not enough space to allocate memory.\n");
                free(newBlock);
            }
            return;
        }

        // Find the last allocated block
        while (current->next != NULL) {
            prev = current;
            current = current->next;
        }

        // Allocate memory to the last block
        if ((current->end + size) <= pm_size) {
            newBlock->start = current->end;
            newBlock->end = newBlock->start + size;

            current->next = newBlock;

            printf(".\n");
        }
        else {
            // If there isn't sufficient memory, then look for a gap to fit the block
            current = head;
            while (current != NULL) {
                if ((current->next == NULL || (current->next->start - current->end) >= size) && size <= (pm_size - current->end)) {
                    // Found a gap or reached the end of the memory
                    newBlock->start = current->end;
                    newBlock->end = newBlock->start + size;

                    newBlock->next = current->next;
                    current->next = newBlock;

                    printf("\n");
                    return;
                }

                current = current->next;
            }

            // No suitable gap found
            printf("Error: Not enough space to allocate memory.\n");
            free(newBlock);
        }
    }
}


void deallocateMemory() {
    int id;
    printf("Enter block id: ");
    scanf("%d", &id);

    struct Block* current = head;
    struct Block* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                head = current->next;
            }

            free(current);
            printf("Memory deallocated successfully.\n");
            return;
        }

        prev = current;
        current = current->next;
    }

    // Block not found
    printf("Error: Block with given id not found.\n");
}

void defragmentMemory() {
    struct Block* current = head;
    int start = 0;

    while (current != NULL) {
        int blockSize = current->end - current->start;
        current->start = start;
        current->end = start + blockSize;
        start = current->end;
        current = current->next;
    }

    printf("Memory defragmented successfully.\n");
}

void freeMemory() {
    struct Block* current = head;
    while (current != NULL) {
        struct Block* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
}

int main() {
    int choice;

    do {
        printf("\nMemory allocation\n");
        printf("-----------------\n");
        printf("1) Enter parameters\n");
        printf("2) Allocate memory for block\n");
        printf("3) Deallocate memory for block\n");
        printf("4) Defragment memory\n");
        printf("5) Quit program\n");
        printf("Enter selection: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            enterParameters();
            break;
        case 2:
            allocateMemory(algorithm);
            printMemoryAllocation();
            break;
        case 3:
            deallocateMemory();
            printMemoryAllocation();
            break;
        case 4:
            defragmentMemory();
            printMemoryAllocation();
            break;
        case 5:
            freeMemory();
            printf("Quitting program...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }


    } while (choice != 5);

    return 0;
}