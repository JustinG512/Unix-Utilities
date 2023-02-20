/* Justin Gallagher, 25JAN2022
Project 0: Implementing Two Unix/Linux Utilities Part B
Due Wednesday by 1pm Points 5 Submitting a file upload Available after Jan 11 at 6pm
CIS 3207, Section 004 //Spring 2022 //Instructor: Professor Gene Kwatny

The second utility you will build is called tucp, a variant of the UNIX tool cp. In this project,
 this command is used to copy files or a group of files. It creates an exact image (copy) of a file on a disk.
 The tucp command requires at least two filenames in its arguments.

 The following are the possible command usages:
tucp Sourcefile Destinationfile
tucp Sourcefile Directory
tucp Sourcefile-1 Sourcefile-2 Sourcefile-3 Sourcefile-n Directory

 If the tucp command contains two file names, then it copies the contents of the 1st file to the 2nd file.
 If the 2nd file doesn’t exist, then it first creates a file with the specified name and the content of the first
 file is copied to it. But if the second file already exists, then it is simply overwritten without any warning.

 If the tucp command has one or more arguments, specifying source file names and following those arguments,
 an argument specifying a directory name, then the command copies each source file to the destination directory with
 the same source file name. The destination file is created if did not exist, or overwritten if it already existed.


 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/fcntl.h>

#define PERMS 0666  // -rw-rw-rw-	0666	read & write

void copyFileContents(char *inPathDirectory[], int inArgc);

int main(int argc, char **argv) {


    /// >tucp file -> file
    /// >tucp file -> directory
    /// >tucp file -> (loop) -> directory

    /*The copeFileContents function will pass the arguments entered by the user along with the incoming count of
     * arguments that were passed during execution.
     * minimum >tucp file directory, or >tucp file directory
     * NO Max  >tucp file file file file (...) directory*/
    copyFileContents(argv, argc);

    return 0;
}

void copyFileContents(char *inPathDirectory[], int inArgc) {

    /* Three struct stat below we are finding out what the first, second, and last argument are.  In this program, it is possible that
     * the second argument and the last argument are the same, but we will flag those entries later on in the
     * program with the argument count*/

    // Retrieve information about first argument entered by user
    struct stat statPath;
    stat(inPathDirectory[1], &statPath);
    int q = stat(inPathDirectory[1], &statPath);

    // Retrieve information about second argument entered by user
    struct stat secondStatPath;
    stat(inPathDirectory[2], &secondStatPath);

    // Retrieve information about last argument entered by user
    struct stat lastStatPath; // only used if inArgc >=4
    stat(inPathDirectory[inArgc - 1], &lastStatPath);

    // perror will produce and write to console if there is an issue with the first argument passed by the user
    if (q == -1) {
        perror("Stat");
        exit(EXIT_FAILURE);
    }

    // The program requires >tucp argv1 argv2 to run successfully.
    if (inArgc < 3) {
        printf("Required input: >tucp argv1 argv2!\n");
        exit(EXIT_FAILURE);
    }

    /* This if/else statement will do two things.  If the requirements are met of the if statement (Argument Count
     * is equal to 3 AND the second argument entered by the user is a directory.  If the if requirements are not met
     * the else statement will treat the second argument entered by the user and attempt to overwrite the file*/
    if (inArgc == 3) {
        if (inArgc == 3 && S_ISDIR(secondStatPath.st_mode)) {
            char *sourceFilePath = inPathDirectory[1];  //
            char *destinationFilePath = inPathDirectory[2];
            char path[1000]; // The maximum length for a file name is 255 bytes.

            int fileOne, fileTwo, n;
            char buf[BUFSIZ];


            if ((fileOne = open(sourceFilePath, O_RDONLY, 0)) == -1) {
                printf("tucp: can't open %s\n", sourceFilePath);
            } else {
                strcpy(path, destinationFilePath); // path = "inPathDirectory"
                strcat(path, "/");  // path = "inPathDirectory/"
                strcat(path, sourceFilePath); // path = "inPathDirectory/d_name"

                if ((fileTwo = creat(path, PERMS)) == -1)
                    printf("tucp: can't create %s, mode %03o\n",
                           path, PERMS);
                while ((n = read(fileOne, buf, BUFSIZ)) > 0)
                    if (write(fileTwo, buf, n) != n)
                        printf("tucp: write error on file %s\n", path);
            }

            /* If inArgc == 3 AND the second argument entered by the user is not a directory, the else will treat the
             * second argument as a regular file and copy the contents into the same directory.  The file type is not
             * specified in this application, so the user must enter the extension if the file is to be recognized as
             * so. */
        } else {

            char *sourceFilePath = inPathDirectory[1];
            char *destinationFilePath = inPathDirectory[2];

            int fileOne, fileTwo, n;
            char buf[BUFSIZ];

            if ((fileOne = open(sourceFilePath, O_RDONLY, 0)) == -1)
                printf("tucp: can't open %s\n", sourceFilePath);
            if ((fileTwo = creat(destinationFilePath, PERMS)) == -1)
                printf("tucp: can't create %s, mode %03o\n",
                       destinationFilePath, PERMS);
            while ((n = read(fileOne, buf, BUFSIZ)) > 0)
                if (write(fileTwo, buf, n) != n)
                    printf("tucp: write error on file %s\n", destinationFilePath);

        }
    }

    /* If inArgc is greater than or equal to 4 the program is designed all of the arguments entered into the last
     * argument which MUST be a directory as that is how the program was designed for the assignment.  If any of the
     * files are not able to be accessed, the error will be displayed to the user through the console and the
     * program will continue to the next argument until the directory is reached*/
    if (inArgc >= 4) {

        /*The for loop will copy the contents of the current argument into the directory that has been specified
         * by the user.  The loop will continue to run until all files have attempted to be copied into the
         * directory*/
        for (int i = 1; i < inArgc - 1; ++i) {

            char *sourceFilePath = inPathDirectory[i];
            char *destinationFilePath = inPathDirectory[inArgc - 1];
            char path[1000]; // The maximum length for a file name is 255 bytes.

            int fileOne, fileTwo, n;
            char buf[BUFSIZ];

            if ((fileOne = open(sourceFilePath, O_RDONLY, 0)) == -1) {
                printf("tucp: can't open %s\n", sourceFilePath);
            } else {
                strcpy(path, destinationFilePath); // path = "FolderFound"
                strcat(path, "/");  // path = "FolderFound/"
                strcat(path, sourceFilePath); // path = "FolderFound/SourceFilePath.txt"

                if ((fileTwo = creat(path, PERMS)) == -1)
                    printf("tucp: can't create %s, mode %03o\n",
                           path, PERMS);
                while ((n = read(fileOne, buf, BUFSIZ)) > 0)
                    if (write(fileTwo, buf, n) != n)
                        printf("tucp: write error on file %s\n", path);
            }


        }


    }


}


