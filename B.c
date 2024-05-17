//program 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>

#define SHM_KEY 123456
#define SEM_KEY_2 654322
#define SEM_KEY_3 654323

int main() {
    int shmid = shmget(SHM_KEY, 100, 0644 | IPC_CREAT);
    char *data = shmat(shmid, (void *)0, 0);

    int semid_2 = semget(SEM_KEY_2, 1, 0666 | IPC_CREAT);
    //semctl(semid_2, 0, SETVAL, 0); // Initialize semaphore 2 to 0 (locked)
    int semid_3 = semget(SEM_KEY_3, 1, 0666 | IPC_CREAT);
    semctl(semid_3, 0, SETVAL, 0); // Initialize semaphore 3 to 0 (locked)
    while (1) {
        struct sembuf sb = {0, -1, 0};
        semop(semid_2, &sb, 1); // Wait for semaphore 2

        char *temp_data = strdup(data);
        char *number_str = strtok(temp_data, ",");
        char *sqd = strtok(NULL, ",");
        int number = atoi(number_str);

        if (strcmp(sqd, "NULL") == 0) {
            int new_sqd = number * 2;
            printf("doubled: %d\n",new_sqd);
            sprintf(data, "%d,%d", number, new_sqd); // Write back to data
        }

        free(temp_data);

        sb.sem_op = 1;
        semop(semid_3, &sb, 1); // Signal semaphore 3
    }

    return 0;
}