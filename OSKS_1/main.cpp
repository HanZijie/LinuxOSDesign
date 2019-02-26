#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>

pid_t p1;
pid_t p2;
pid_t p3,p4,p5;

static int sem_id;
void P(int semid,int index)
{
    struct sembuf sem;	
    sem.sem_num = index;
    sem.sem_op = -1;	
    sem.sem_flg = 0; //操作标记：0或IPC_NOWAIT等
    semop(semid,&sem,1);	//1:表示执行命令的个数	
    return;
}

void V(int semid,int index)
{	
    struct sembuf sem;	
    sem.sem_num = index;
    sem.sem_op =  1;
    sem.sem_flg = 0;	
    semop(semid,&sem,1);	
    return;
}

static int shMemoryId;
static int shMemoryId_Rate;
char shareMemory[31];
long shDataRate[10];

int main(int argc, char const *argv[])
{
    sem_id = semget((key_t)1,5,0666 | IPC_CREAT);
    shMemoryId = shmget((key_t)2,sizeof(shareMemory),0666 | IPC_CREAT);
    shMemoryId_Rate = shmget((key_t)3,sizeof(shDataRate),0666 | IPC_CREAT);
    char* m = (char *)shmat(shMemoryId,NULL,0);
    semctl(sem_id,0,SETVAL,0);
    semctl(sem_id,1,SETVAL,29);
    semctl(sem_id,2,SETVAL,1);
    semctl(sem_id,3,SETVAL,1);
    semctl(sem_id,4,SETVAL,1);
    char* execv1_argv[3];
    char* execv2_argv[3];

    if(argc == 1){
        printf("Command line parameters missing (2 parameters required at least)\n");
        return 0;
    }else{

        execv1_argv[0] = "./get";
        execv1_argv[1] = (char *)malloc(sizeof(argv[1])+1);
        strcpy(execv1_argv[1],argv[1]);
        execv1_argv[2] = NULL;

        if(argc == 3){

            execv2_argv[0] = "./put";
            execv2_argv[1] = (char *)malloc(sizeof(argv[2])+1);
            strcpy(execv2_argv[1],argv[2]);
            execv2_argv[2] = NULL;
        }else{

            execv2_argv[0] = "./put";
            execv2_argv[1] = (char *)malloc(sizeof(argv[2])+7);
            sprintf(execv2_argv[1],"%s(copy)",argv[1]);
            execv2_argv[2] = NULL;

        }
    }
    p1 = fork();
    if(p1 == 0){
        printf("./get is created\n");
        printf("%s\n",execv1_argv[1]);
        execv("./get",execv1_argv);
        exit(0);
    }else{
        p2 = fork();
        if(p2 == 0){
            execv("./put",execv2_argv);
            exit(0);
        }else{
            p3 = fork();
            if(p3 == 0){
                execv("./QtGet/QtCustomProgressbar",NULL);
                exit(0);
            }else{
                p4 = fork();
                if(p4 == 0){
                    execv("./QtPut/QtCustomProgressbar",NULL);
                    exit(0);
                }else{
                    p5 = fork();
                    if(p5 == 0){
                        execv("./QtCpu/QtCustomProgressbar",NULL);
                        exit(0);
                    }else{
                        int *addr;
                        wait(addr);
                        wait(addr);
                        semctl(sem_id,1,IPC_RMID);
                        shmctl(shMemoryId,IPC_RMID,0);
                        shmctl(shMemoryId_Rate,IPC_RMID,0);
                        return 0;
                    }
                }
            }
            
        }
    }
    return 0;
}