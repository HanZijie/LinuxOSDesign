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

static int shMemoryId;
static int shMemoryId_Rate;
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

int main(int argc, char const *argv[])
{
    printf("start %s\n",argv[0]);
    printf("%s\n",argv[1]);
    shMemoryId = shmget((key_t)2,0,0666 | IPC_CREAT);
    shMemoryId_Rate = shmget((key_t)3,0,0666 | IPC_CREAT);
    char* m = (char *)shmat(shMemoryId,NULL,0);
    long* shData = (long *)shmat(shMemoryId_Rate,NULL,0);
    int sem_id = semget((key_t)1,5,0666 | IPC_CREAT);
    int i = 0;
    FILE* fp = fopen(argv[1],"rb");
    fseek (fp, 0, SEEK_END);
    shData[0] = ftell(fp);
    shData[1] = 0;
    fclose(fp);
    fp = fopen(argv[1],"rb");
    if(fp == NULL){
        printf("file %s is missing\n",argv[1]);
        return 0;
    }
    m[30] = 31;
    
    while(1){
        P(sem_id,2);
        shData[1] += 1;
        P(sem_id,1);
        i = i%30;
        m[i] = (char)fgetc(fp);
        if(feof(fp)){
            m[30] = i;
            fclose(fp);
            V(sem_id,0);
            break;
        }
        //printf("%c",m[i]);
        i++;
        V(sem_id,0);
        V(sem_id,2);
    }
    return 0;
}