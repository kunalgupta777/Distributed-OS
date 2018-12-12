// FIFO implementation in C
// this program first writes in FIFO and then reads it

#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main() {
    int file_descriptor;

    //path for FIFO file.
    char *path = "fifo_file";

    mkfifo( path , 0666);
    // 0066 is the permission
    char ch;
    char message_to_be_written[100];
    char message_received[100];
    do {

        // Writing to FIFO
        
        printf("\n Enter a Message to For Process 2:");
        fgets(message_to_be_written, 100, stdin);
        file_descriptor = open(path, O_WRONLY);
        write(file_descriptor, message_to_be_written, strlen(message_to_be_written)+1);
        close(file_descriptor);

        // Reading from FIFO
        file_descriptor = open(path, O_RDONLY);
        read(file_descriptor, message_received, sizeof(message_received));
        printf("Process 2 says: %s", message_received);
        close(file_descriptor);
        printf("\nDo you want to continue? (y/n):");
        scanf("%c",&ch);
    } while(ch == 'y' || ch == 'Y');

    return 0;
}