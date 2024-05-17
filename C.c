//program 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>

#define SHM_KEY 123456
#define SEM_KEY_1 654321
#define SEM_KEY_3 654323

int main() {
    int shmid = shmget(SHM_KEY, 100, 0644 | IPC_CREAT);
    char *data = shmat(shmid, (void *)0, 0);

    int semid_1 = semget(SEM_KEY_1, 1, 0666 | IPC_CREAT);
    //semctl(semid_1, 0, SETVAL, 1); // Initialize semaphore 1 to 1 (unlocked)
    int semid_3 = semget(SEM_KEY_3, 1, 0666 | IPC_CREAT);
    //semctl(semid_3, 0, SETVAL, 0); // Initialize semaphore 3 to 0 (locked)
    char *last_data = strdup("NULL,NULL");

    while (1) {
        struct sembuf sb = {0, -1, 0};
        semop(semid_3, &sb, 1); // Wait for semaphore 3
        if (strcmp(data, last_data) != 0) {
            printf("Newly Stored Value: %s\n", data);
            free(last_data);
            last_data = strdup(data);
        }
        else{
            printf("Newly Stored Value: %s\n", data);
            free(last_data);
            last_data = strdup(data);
        }

        sb.sem_op = 1;
        semop(semid_1, &sb, 1); // Signal semaphore 1
    }

    free(last_data);

    return 0;
}
