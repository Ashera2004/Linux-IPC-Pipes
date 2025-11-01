# Linux-IPC--Pipes
Linux-IPC-Pipes


# Ex03-Linux IPC - Pipes

# AIM:
To write a C program that illustrate communication between two process using unnamed and named pipes

# DESIGN STEPS:

### Step 1:

Navigate to any Linux environment installed on the system or installed inside a virtual environment like virtual box/vmware or online linux JSLinux (https://bellard.org/jslinux/vm.html?url=alpine-x86.cfg&mem=192) or docker.

### Step 2:

Write the C Program using Linux Process API - pipe(), fifo()

### Step 3:

Testing the C Program for the desired output. 

# PROGRAM:

## C Program that illustrate communication between two process using unnamed pipes using Linux API system calls
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/wait.h>

// Function declarations
void server(int, int); 
void client(int, int); 

int main() { 
    int p1[2], p2[2], pid; 

    // Create two pipes
    pipe(p1); 
    pipe(p2); 

    pid = fork(); 

    if (pid == 0) { 
        // Child process → acts as Server
        close(p1[1]); // Close write end of pipe1
        close(p2[0]); // Close read end of pipe2
        server(p1[0], p2[1]); 
        exit(0);
    } 

    // Parent process → acts as Client
    close(p1[0]); // Close read end of pipe1
    close(p2[1]); // Close write end of pipe2
    client(p1[1], p2[0]); 
    
    wait(NULL); // Wait for child process to finish
    return 0; 
} 

// -------------------- SERVER FUNCTION --------------------
void server(int rfd, int wfd) { 
    int n; 
    char fname[2000]; 
    char buff[2000];

    // Read filename from pipe
    n = read(rfd, fname, 2000);
    fname[n] = '\0';

    // Open the file for reading
    int fd = open(fname, O_RDONLY);
    if (fd < 0) { 
        // If file cannot be opened, send error message
        write(wfd, "can't open", 9); 
    } else { 
        // Read file contents and send to client
        n = read(fd, buff, 2000); 
        write(wfd, buff, n); 
        close(fd);
    } 
}

// -------------------- CLIENT FUNCTION --------------------
void client(int wfd, int rfd) {
    int n; 
    char fname[2000];
    char buff[2000];

    // Input filename from user
    printf("Enter filename: ");
    scanf("%s", fname);

    // Send filename to server
    printf("\nClient sending request for file '%s'... Please wait...\n", fname);
    write(wfd, fname, 2000);

    // Read file contents from server
    n = read(rfd, buff, 2000);
    buff[n] = '\0';

    // Display server response
    printf("\nThe results of the client are:\n\n");
    write(1, buff, n);
    printf("\n");
}


```




## OUTPUT

<img width="805" height="457" alt="ex3_1" src="https://github.com/user-attachments/assets/4ff7db71-cad2-463b-a315-07b099791fbd" />


## C Program that illustrate communication between two process using named pipes using Linux API system calls

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "/tmp/my_fifo"
#define FILE_NAME "hello.txt"

// Function declarations
void server();
void client();

int main() {
    pid_t pid;

    // Create FIFO (named pipe) if it doesn’t already exist
    mkfifo(FIFO_FILE, 0666);

    pid = fork();  // Create a child process

    if (pid > 0) {
        // Parent process acts as the SERVER
        sleep(1);  // Give client time to set up
        server();
    } 
    else if (pid == 0) {
        // Child process acts as the CLIENT
        client();
    } 
    else {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

// -------------------- SERVER FUNCTION --------------------
void server() {
    int fifo_fd, file_fd;
    char buffer[1024];
    ssize_t bytes_read;

    // Open the file to read
    file_fd = open(FILE_NAME, O_RDONLY);
    if (file_fd == -1) {
        perror("Error opening hello.txt");
        exit(EXIT_FAILURE);
    }

    // Open FIFO for writing
    fifo_fd = open(FIFO_FILE, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Error opening FIFO");
        exit(EXIT_FAILURE);
    }

    printf("Server: Sending contents of '%s' to client...\n\n", FILE_NAME);

    // Read file contents and write to FIFO
    while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
        write(fifo_fd, buffer, bytes_read);
    }

    printf("\nServer: File data sent successfully.\n");

    close(file_fd);
    close(fifo_fd);
}

// -------------------- CLIENT FUNCTION --------------------
void client() {
    int fifo_fd;
    char buffer[1024];
    ssize_t bytes_read;

    // Open FIFO for reading
    fifo_fd = open(FIFO_FILE, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Error opening FIFO");
        exit(EXIT_FAILURE);
    }

    printf("Client: Waiting to receive data from server...\n\n");

    // Read data from FIFO and display on screen
    while ((bytes_read = read(fifo_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    printf("\n\nClient: Data received successfully.\n");

    close(fifo_fd);
}

```



## OUTPUT

<img width="793" height="501" alt="ex3_2" src="https://github.com/user-attachments/assets/13e0ea7c-e9f6-4ce8-bb7a-02cff328301a" />

# RESULT:
The program is executed successfully.
