//program 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>

#define SHM_KEY 123456
#define SEM_KEY_1 654321
#define SEM_KEY_2 654322

int main() {
    int shmid = shmget(SHM_KEY, 100, 0644 | IPC_CREAT);
    char *data = shmat(shmid, (void *)0, 0);
    strcpy(data, "NULL,NULL"); // Initialize shared memory with "NULL,NULL"

    int semid_1 = semget(SEM_KEY_1, 1, 0666 | IPC_CREAT);
    semctl(semid_1, 0, SETVAL, 1); // Initialize semaphore 1 to 1 (unlocked)
    int semid_2 = semget(SEM_KEY_2, 1, 0666 | IPC_CREAT);
    semctl(semid_2, 0, SETVAL, 0); // Initialize semaphore 2 to 0 (locked)

    int held = -1;

    while (held < 100) {
        struct sembuf sb = {0, -1, 0};
        if(1){
            semop(semid_1, &sb, 1); // Wait for semaphore 1
        }

        char *temp_data = strdup(data);
        char *number = strtok(temp_data, ",");
        char *sqd = strtok(NULL, ",");

        if(strcmp(number, "NULL") == 0){
            printf(" it should be waiting for user...\n");
            getchar();
            held++;
            sprintf(data, "%d,NULL", held); // Write back to data
        }
        else{
            held++;
            sprintf(data, "%d,NULL", held); // Write back to data
            printf("gen: %d\n",held);
        }

        free(temp_data);

        sb.sem_op = 1;
        semop(semid_2, &sb, 1); // Signal semaphore 2
    }

    return 0;
}