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
