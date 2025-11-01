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
