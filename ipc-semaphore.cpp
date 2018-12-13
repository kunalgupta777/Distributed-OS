#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX_CHILD 4
#define SEM_MUTEX 0

int main()
{
    int sem_id = getuid();

    int glob_sem_id = semget(sem_id, 1, 0666);

    if (glob_sem_id == -1)
    {
        // Create new semafor.
        glob_sem_id = semget(sem_id, 1, 0666 | IPC_CREAT);
        if (glob_sem_id == -1)
        {
            printf("Error while creating semaphore.\n");
            exit(1);
        }
    }

    int ret = semctl(glob_sem_id, SEM_MUTEX, SETVAL, 1);

    if (ret == -1)
    {
        printf("Error while creating SEM_MUTEX.\n");
        exit(1);
    }

    int mypipe[2];

    if (pipe(mypipe) == -1)
    {
        printf("Error while creating pipe.\n");
        exit(1);
    }

    int pid = fork();

    if (pid == -1)
    {
        printf("Error while forking process.\n");
        exit(1);
    }

    if (pid != 0)
    {
        // Parent process.
        close(mypipe[1]);

        char buffer[100];
        int l = 0;

        while (1)
        {
            l = read(mypipe[0], buffer, sizeof(buffer));
            if (l == -1)
            {
                exit(1);
            }
            else
            {
                write(1, buffer, l);
            }
        }
    }
    else
    {
        struct sembuf op[1];

        // Child process.
        for (int children = 0; children < MAX_CHILD; children += 1)
        {

            int childpid = fork();

            if (childpid == 0)
            {
                close(mypipe[0]);

                char buffer[100];
                int l = 0;

                srand(getpid());
                // printf("%d: and %d \n", getpid(), children);
                while (1)
                {
                    op[0].sem_num = SEM_MUTEX;
                    op[0].sem_op = -1;
                    op[0].sem_flg = 0;

                    int ret = semop(glob_sem_id, op, 1);
                    if (ret == -1)
                    {
                        printf("Error while entering critical section. %s\n", strerror(errno));
                        exit(1);
                    }

                    /****** Critical section. ******/
                    l = sprintf(buffer, "%d: ", getpid());
                    write(mypipe[1], buffer, l);
                    for (int i = 0; i < 10; i += 1)
                    {
                        usleep(40000);
                        l = sprintf(buffer, "%d ", rand() % 100);
                        write(mypipe[1], buffer, l);
                    }
                    l = sprintf(buffer, "\n");
                    write(mypipe[1], buffer, l);
                    /***** Critical section end *****/

                    op[0].sem_num = SEM_MUTEX;
                    op[0].sem_op = 1;
                    op[0].sem_flg = 0;

                    ret = semop(glob_sem_id, op, 1);
                    if (ret == -1)
                    {
                        printf("Error while leaving critical section.\n");
                        exit(1);
                    }
                    break;
                }
            }
        }

    }

    return 0;
}