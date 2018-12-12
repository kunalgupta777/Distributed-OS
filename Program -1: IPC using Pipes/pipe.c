// Communication between parent and child process using pipes

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <sys/wait.h> 

void parent_process( int file_descriptor1[2], int file_descriptor2[2]) {
    wait(NULL);
    printf("\n Parent Process is Running!");
    int x = getppid();
    printf("\n Process ID: %d",x);

    close(file_descriptor2[1]);
    char received_message[100];
    read(file_descriptor2[0], received_message, 100);
    printf("\n Child Sent the message: %s", received_message);
    close(file_descriptor2[0]);

    printf("\n Enter a message to be sent to Child:");
    char message_to_be_sent[100];
    scanf("%s", message_to_be_sent);
    
    close(file_descriptor1[0]);
    write(file_descriptor1[1], message_to_be_sent, strlen(message_to_be_sent)+1);
    close(file_descriptor1[1]);
    

}

void child_process( int file_descriptor1[2], int file_descriptor2[2]) {
    printf("\n Child Process is Running!");
    printf("\n Process ID is: %d", getpid());

    // closing reading end of 2nd pipe
    close(file_descriptor1[1]);
    char received_message[100];
    read(file_descriptor1[1],received_message, 100);
    printf("\n Message Sent By Parent: %s", received_message);
    close(file_descriptor1[0]);

    char message_to_be_sent[100];
    printf("\n Enter a message to be sent to Parent:");
    scanf("%s", message_to_be_sent);
    close(file_descriptor2[0]);
    write(file_descriptor2[1], message_to_be_sent, strlen(message_to_be_sent)+1);
    close(file_descriptor2[1]);
}

int main() {
    
    int file_descriptor1[2];   // Used to store the ends of 1st pipe
    int file_descriptor2[2];   // Used to store the ends of 2nd pipe
    
    // This program shows communication between 2 processes ( parent and child ) using 2 pipes
    char input_string[100];
    pid_t process;

    if( pipe(file_descriptor1) == -1) {
        printf("Error in Creating Pipe!");
        return 1;
    }
    if( pipe(file_descriptor2) == -1) {
        printf("Error in Creating Pipe!");
        return 1;
    }

    process = fork();            // Creating a parent and child process
    
    if( process < 0) {
        printf("Fork Failed!");
        return 1;
    }
    // Control goes to parent process 
    else if ( process > 0) {
        parent_process(file_descriptor1, file_descriptor2);
    }
    // Control goes to child process
    else if( process == 0) {
        child_process(file_descriptor1, file_descriptor2);
    }
    return 0;
}