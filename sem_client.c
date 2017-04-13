/**
 * Compile this file as "sem_client" for the pair of programs to work properly
 * (view the info on sem_server.c on how to run this)
 *
 * gcc -o sem_client sem_client.c -Wall -Werror
 *
 * This is needed because the server does an `exec` of this program
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

#include "common.h"
int var_count;
int main(int argc, char *argv[]) {
    int sem_fd;
    key_t sem_key;
    int sem_id;
    //int i;
    struct sembuf sop;
	//char *p;
    // Recover the sem_key from file
    sem_fd = open(SEM_KEY_FILE, O_RDONLY);
	printf("sem_id_client = %d\n",sem_fd);
    if (sem_fd < 0) 
	{
        perror("Could not open sem key for reading");
        exit(1);
    }

    // Technically speaking, the read could read less than sizeof(key_t)
    // Which would be wrong.
    // But in our case, it is not likely to happen...
    if (read(sem_fd, &sem_key, sizeof(key_t)) != sizeof(key_t)) {
        perror("Error reading the semaphore key");
        exit(2);
    }

    // Done getting the semaphore key
    close(sem_fd);

    // Now obtain the (hopefully) existing sem
    sem_id = semget(sem_key, 0, 0);
    if (sem_id < 0) {
        perror("Could not obtain semaphore");
        exit(3);
    }

    //for (i = 0; i < 5; ++i) {
	while(1)
{
        sop.sem_num = 0;
        sop.sem_op = -1;
        sop.sem_flg = SEM_UNDO;
        printf("Client #%d waiting\n", getpid());
        semop(sem_id, &sop, 1);
        printf("Client #%d acquired. Sleeping\n", getpid());
        sleep(1);
        printf("Client #%d releasing\n", getpid());
        sop.sem_op = 0;
        semop(sem_id, &sop, 1);
	printf("Client count===========================%d\n",var_count++);
    }

    exit(0);
}
