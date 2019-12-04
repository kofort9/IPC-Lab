#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

//GOAL:
//Modify to pass 10 integer values from the Parent to the Child -> print
//then return those values from Child back to the Parent -> print again.

void  ClientProcess(int []); //Child process of server
 //receive the shared memory in its address space
 // no shared memory operations are required

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;

     if (argc != 11) { // increase  the number of  integers allowed
          printf("Use: %s #1 #2 #3 #4... #10\n", argv[0]);
          exit(1);
     }
     // shared memory of four integers with adress space
     // increase size to 10
     ShmID = shmget(IPC_PRIVATE, 10*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("Server has received a shared memory of ten integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");


     ShmPTR[1] = atoi(argv[2]);
     ShmPTR[2] = atoi(argv[3]);
     ShmPTR[3] = atoi(argv[4]);
     ShmPTR[4] = atoi(argv[5]);

     for (size_t i = 1; i < 10; i++) {
        ShmPTR[i] = atoi(argv[i+1]);
     }
     //original print statment
     //printf("Server has filled %d %d %d %d %d in shared memory...\n",
            //ShmPTR[0], ShmPTR[1], ShmPTR[2], ShmPTR[3], ShmPTR[4]);
    printf("Server has filled ");
    for (size_t i = 0; i < 10; i++) {
      printf("%d ", ShmPTR[i]);
    }
    printf(" in shared memory...\n" );

     printf("Server is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          ClientProcess(ShmPTR);
          exit(0);
     }

     wait(&status);
     printf("Server has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("Server has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("Server has removed its shared memory...\n");
     printf("Server exits...\n");
     exit(0);
}

void  ClientProcess(int  SharedMem[])
{
     printf("   Client process started\n");
     //Orignal print statment
     // printf("   Client found %d %d %d %d %d in shared memory\n",
     //            SharedMem[0], SharedMem[1], SharedMem[2],
     //            SharedMem[3],SharedMem[4]);
     printf("Client found ");
     for (size_t i = 0; i < 10; i++) {
       printf("%d ", SharedMem[i]);
     }
     printf(" in shared memory\n");
     printf("   Client is about to exit\n");
}
