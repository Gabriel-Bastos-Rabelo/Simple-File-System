# Simple File System in C for Operating Systems Course

This project is a simple file system implemented in C for an Operating Systems course. The file system simulates a virtual disk stored in a single file and supports basic file operations such as creating, listing, and displaying the contents of files. The file system uses contiguous allocation for file storage and maintains a file table to manage the files.

## Instructions for Compilation:

To compile the program, use the following command:

`gcc -o fileSystem fileSystem.c`


## Instructions for Execution:

To execute the program, use the following commands:

1. **Add a New File:**

    `./fileSystem cat <filename> <content>`


2. **List Existing Files:**

    `./fileSystem ls`


3. **View the Content of a File:**

    `./fileSystem more <filename>`



`

    