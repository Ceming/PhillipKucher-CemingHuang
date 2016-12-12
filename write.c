#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main() {
    int semid = semget(ftok("makefile", 42), 1, 0);

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;

    semop(semid, &sb, 1);
    
    int shmid = shmget(ftok("makefile",24), sizeof(int), 0644);
    int *shmnum = shmat(shmid, 0, 0);
    int fd = open("semaphone.txt", O_APPEND | O_RDWR);
    lseek(fd, -(*shmnum), SEEK_END);
    char line[*shmnum + 1];
    int count = read(fd, line, *shmnum);
    line[count] = NULL;
    if (strlen(line) != 0){
      printf("Previous line: %s", line);
    }
    
    char input[256];
    fgets(input, sizeof(input), stdin);
    *shmnum = strlen(input);

    write(fd, input, *shmnum);
    close(fd);
    
    sb.sem_op = 1;
    semop(semid, &sb, 1);

    printf("Line added!\n");
    return 0;
}
