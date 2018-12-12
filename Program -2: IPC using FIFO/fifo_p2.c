// FIFO implementation in C
// this program first reads from FIFO then writes to it.

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
    char message_received[100];
    char message_to_be_written[100];
    do {

        // Reading from FIFO
        file_descriptor = open(path, O_RDONLY);
        
        read(file_descriptor, message_received, sizeof(message_received));
        printf("Process 1 says: %s", message_received);
        close(file_descriptor);

        // Writing to FIFO
       
        printf("\n Enter a Message for Process 1:");
        fgets(message_to_be_written, 100, stdin);
        file_descriptor = open(path, O_WRONLY);
        write(file_descriptor, message_to_be_written, strlen(message_to_be_written)+1);
        close(file_descriptor);

        printf("\nDo you want to continue? (y/n):");
        scanf("%c", &ch);
    } while(ch == 'y' || ch == 'Y');

    return 0;
}