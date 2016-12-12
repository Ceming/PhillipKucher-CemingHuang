#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};


int main(int argc, char *argv[]){
  int semid;
  int key = ftok("makefile" , 42);
  int sc;
 
  int shmid;
  int key2 = ftok("makefile", 24);

  int fd;
        
  if (strncmp(argv[1], "-c", strlen(argv[1])) == 0){
    semid = semget(key, 1, IPC_CREAT | 0644);
    shmid = shmget(key2, sizeof(int), IPC_CREAT | 0644);
    fd = open("semaphone.txt", O_CREAT | O_RDWR | O_TRUNC);
    union semun su;
    su.val = 1;
    sc = semctl(semid, 0, SETVAL, su);
    printf("Semaphore created: %d\n", sc);
  }

  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    char line[100];
    fd = open("semaphone.txt", O_RDONLY);
    int count = read(fd, line, 100);
    line[count] = NULL;
    printf("Content:\n%s", line);
    close(fd);
  }

  else if(strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    semid = semget(key, 1, 0);
    sc = semctl(semid, 0, IPC_RMID);
    printf("semaphore removed: %d\n", sc);
    int shmid = shmget(ftok("makefile",24), sizeof(int), 0644);
    int *shmnum = shmat(shmid, 0, 0);
    shmdt(shmnum);
    shmctl(shmid, IPC_RMID, NULL);
    char line[100];
    fd = open("semaphone.txt", O_RDONLY);
    int count = read(fd, line, 100);
    printf("Content:\n%s", line);
    close(fd);
  }
	

  return NULL;

}
