/* Justin Gallagher, 17JAN2022
 Project 0: Implementing Two Unix/Linux Utilities Part A
 Due Wednesday by 1pm Points 5 Submitting a file upload Available after Jan 11 at 6pm
 CIS 3207, Section 004 //Spring 2022 //Instructor: Professor Gene Kwatny
Grading for Project 0 – Total 10 Points
Part A 5 Points    Deliverable in 1 week (beginning of lab on 1/19/2022)
tuls – correctly uses the library functions scandir(), opendir(), readdir()  (1 pt)
tuls – correctly identifies directories vs files (0.5 pts)
tuls - correctly lists the current directory contents (1.5 pt)
tuls - correctly lists the current directory and recursively lists directory contents from the current directory (2 pts)*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void listDirectoryContents(char *inPathDirectory, const int indentCounter);

int main(int argc, char **argv) {

/*    USED FOR TESTING ON HOME MACHINE, DO NOT DELETE
    argv[1] = "/Users/justingallagher/Downloads"; // Simulate argc = 1, argv = Downloads
    argv[1] = "/Users/justingallagher/Desktop"; // Simulate argc = 1, argv = Desktop
    argv[1] = "."; // / Simulate argc = 1, argv = Current run path
    argc = 2;*/

    // If the user adds 0, or 2+ arguments, the default will be set to current directory
    if (argc != 2) {
        argv[1] = ".";
    }

    char *inDirName = argv[1]; // inPathDirectory pointer to pass to argv[1] string to function

    if ((opendir(argv[1])) == NULL) {
        printf("tuls: cannot open directory\n");
        exit(EXIT_FAILURE);  // Set exit to = 1 and terminate program
    }

    // Function to list current directory contents
    printf("[%s]\n", argv[1]);
    listDirectoryContents(inDirName, 0);

    // If a directory arg is passed by the user, this will always print the current directory
    if (argc == 2) {
        printf("\n");
        printf("[Current Directory]\n");
        listDirectoryContents(".", 0);
    }

    return 0;
}

void listDirectoryContents(char *inPathDirectory, const int indentCounter) {
    int i;
    char path[1000]; // The maximum length for a file name is 255 bytes.
    struct dirent *dir; // Declare direct Struct
    DIR *d = opendir(inPathDirectory); // Declare current directory stream of inPathDirectory

    if (!d) // if d == 0, return
        return;

    // Print contents of directory
    while ((dir = readdir(d)) != NULL) {  // returns a pointer to the next directory entry

        // if d_name is not equal to "." or ".." execute code
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {

            // This will print "_" for every time we recursively enter the function
            for (i = 0; i < indentCounter; i++) {
                printf("_");
            }

            printf("%s\n", dir->d_name); // Print the null terminated filename

            // If d_type is a directory, this will print the current indent(+1) and list the directory in []
            if (dir->d_type == DT_DIR) { // DT_DIR is a directory
                for (i = 0; i < indentCounter + 1; i++) {
                    printf(" ");
                }
                printf("[%s]\n", dir->d_name); // Print the null terminated filename

            }

            // This will adjust path to the found directory re-run the function
            strcpy(path, inPathDirectory); // path = "inPathDirectory"
            strcat(path, "/");  // path = "inPathDirectory/"
            strcat(path, dir->d_name); // path = "inPathDirectory/d_name"

            // If directory found, this will pass completed path and increase indent (+1) and list contents
            listDirectoryContents(path, indentCounter + 1);
        }
    }

    closedir(d); // Close the current directory
}
