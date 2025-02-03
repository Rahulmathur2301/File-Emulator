#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for files
struct File {
    char name[50];
    struct File* next;
};

// Define a structure for directories (using binary tree)
struct Directory {
    char name[50];
    struct Directory* left;
    struct Directory* right;
    struct File* files;
};

// Function to create a new directory
struct Directory* createDirectory(char name[]) {
    struct Directory* newDir = (struct Directory*)malloc(sizeof(struct Directory));
    strcpy(newDir->name, name);
    newDir->left = NULL;
    newDir->right = NULL;
    newDir->files = NULL;
    return newDir;
}

// Function to create a new file
struct File* createFile(char name[]) {
    struct File* newFile = (struct File*)malloc(sizeof(struct File));
    strcpy(newFile->name, name);
    newFile->next = NULL;
    return newFile;
}

// Function to create a new file system (directory)
struct Directory* createFileSystem(char name[]) {
    return createDirectory(name);
}

// Function to change directory
struct Directory* changeDirectory(struct Directory* currentDir, char name[]) {
    if (strcmp(name, "..") == 0) {
        return currentDir->left; // Go up one level
    } else if (currentDir->left != NULL && strcmp(currentDir->left->name, name) == 0) {
        return currentDir->left;
    } else if (currentDir->right != NULL && strcmp(currentDir->right->name, name) == 0) {
        return currentDir->right;
    } else {
        printf("Directory not found.\n");
        return currentDir;
    }
}

// Function to add a file to a directory
void addFile(struct Directory* dir, char name[]) {
    struct File* newFile = createFile(name);
    newFile->next = dir->files;
    dir->files = newFile;
}

// Function to list files in a directory
void listFiles(struct Directory* dir) {
    struct File* current = dir->files;
    printf("Files in %s directory:\n", dir->name);
    while (current != NULL) {
        printf("- %s\n", current->name);
        current = current->next;
    }
}

// Function to delete a file from a directory
void deleteFile(struct Directory* dir, char name[]) {
    struct File* current = dir->files;
    struct File* prev = NULL;
    
    while (current != NULL && strcmp(current->name, name) != 0) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("File not found in the directory.\n");
    } else {
        if (prev == NULL) {
            // Deleting the first file in the directory
            dir->files = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
        printf("File '%s' deleted successfully.\n", name);
    }
}

// Function to delete a directory
void deleteDirectory(struct Directory* parent, struct Directory* toDelete) {
    if (parent->left == toDelete) {
        parent->left = NULL;
    } else if (parent->right == toDelete) {
        parent->right = NULL;
    }
    
    // Free memory for files in the directory
    struct File* currentFile = toDelete->files;
    while (currentFile != NULL) {
        struct File* temp = currentFile;
        currentFile = currentFile->next;
        free(temp);
    }
    
    free(toDelete);
    printf("Directory deleted successfully.\n");
}

int main() {
    struct Directory* root = createFileSystem("Root");
    struct Directory* currentDir = root;
    int choice;
    char name[50];

    do {
        printf("\nFile System Emulator Menu:\n");
        printf("1. Create Directory\n");
        printf("2. Change Directory\n");
        printf("3. List Files\n");
        printf("4. Add File\n");
        printf("5. Delete File\n");
        printf("6. Delete Directory\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter directory name: ");
                scanf("%s", name);
                if (currentDir->left == NULL) {
                    currentDir->left = createDirectory(name);
                } else if (currentDir->right == NULL) {
                    currentDir->right = createDirectory(name);
                } else {
                    printf("Cannot create more directories in this level.\n");
                }
                break;
            case 2:
                printf("Enter directory name to change (or '..' to go up one level): ");
                scanf("%s", name);
                currentDir = changeDirectory(currentDir, name);
                break;
            case 3:
                listFiles(currentDir);
                break;
            case 4:
                printf("Enter file name to add: ");
                scanf("%s", name);
                addFile(currentDir, name);
                break;
            case 5:
                printf("Enter file name to delete: ");
                scanf("%s", name);
                deleteFile(currentDir, name);
                break;
            case 6:
                if (currentDir == root) {
                    printf("Cannot delete root directory.\n");
                } else {
                    deleteDirectory(root, currentDir);
                    currentDir = root;
                }
                break;
            case 7:
                printf("Exiting File System Emulator. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 7);

    // Free allocated memory for directories and files
    // Note: You should implement proper memory deallocation logic here

    return 0;
}
