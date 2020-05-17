#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

const char chopstick_name_eg[] = "/chopstick123456789";
const char chopstick_name_fm[] = "/chopstick%d";
#define CHOPSTICK_NAME_LEN (sizeof(chopstick_name_eg)/sizeof(chopstick_name_eg[0]) - 1)
typedef struct strChopstick
{
    int no;
    char name[CHOPSTICK_NAME_LEN + 1];
    sem_t *sem;  
    bool isDirty;
} ChopStick;

int chopstick_count = 0;
ChopStick *chopsticks = NULL;

bool create_chopsticks(int count)
{
    int i;
    chopsticks = (ChopStick *)malloc(sizeof(ChopStick) * count);
    if(NULL == chopsticks)
    {
        perror("malloc failed. ");
        return false;
    }
    for(i = 0; i < count; i++)
    {
        sem_t *sem = NULL;
        char chopstick_name[CHOPSTICK_NAME_LEN + 1];
        snprintf(chopstick_name, CHOPSTICK_NAME_LEN, chopstick_name_fm, i);
        sem = sem_open(chopstick_name, O_CREAT | O_EXCL, S_IRWXU, 1);
        while((SEM_FAILED == sem) && (errno == EEXIST))
        {
            perror("sem_open failed. ");
            sem_close(sem);
            sem_unlink(chopstick_name); 
            sem = sem_open(chopstick_name, O_CREAT | O_EXCL, S_IRWXU, 1);
        }
        if(SEM_FAILED == sem)
        {
            perror("sem_open failed finally. ");
            break;
        }
        chopsticks[i].no = i;
        strcpy(chopsticks[i].name, chopstick_name);
        chopsticks[i].sem = sem; 
        chopsticks[i].isDirty = true;
        printf("%s is opened!\n", chopsticks[i].name);
    } 
    /* clean up when failed to get resoures */
    if(i < count)
    {
        for(i = 0; i < count; i++)
        {
            sem_close(chopsticks[i].sem);
            sem_unlink(chopsticks[i].name);
        }
        free(chopsticks);
        return false;
    }
    chopstick_count = count;
    return true;
}

void destroy_chopsticks()
{
	int i;
    for(i = 0; i < chopstick_count; i++)
    {
        sem_close(chopsticks[i].sem);
        sem_unlink(chopsticks[i].name);
        printf("%s is removed!\n", chopsticks[i].name);
    }
    free(chopsticks);
}

int pick_up_chopstick(int no)
{
    char chopstick_name[CHOPSTICK_NAME_LEN + 1];
    snprintf(chopstick_name, CHOPSTICK_NAME_LEN, chopstick_name_fm, no);
    sem_t *sem = sem_open(chopstick_name, O_CREAT, S_IRWXU, 1);
    if(SEM_FAILED == sem)
    {
	    perror("sem_open failed. ");
	    return -1;
    }

    int ret;
    ret = sem_wait(sem);
    if(ret < 0)
    {
        perror("sem_wait error. ");
    }
    return ret;
}

int try_pick_up_chopstick(int no)
{
    char chopstick_name[CHOPSTICK_NAME_LEN + 1];
    snprintf(chopstick_name, CHOPSTICK_NAME_LEN, chopstick_name_fm, no);
    sem_t *sem = sem_open(chopstick_name, O_CREAT, S_IRWXU, 1);
    if(SEM_FAILED == sem)
    {
	    perror("sem_open failed. ");
	    return -1;
    }

    int ret;
    ret = sem_trywait(sem);
    if(ret < 0)
    {
        perror("confilt found. ");
    }
    return ret;
}
int put_down_chopstick(int no)
{
    char chopstick_name[CHOPSTICK_NAME_LEN + 1];
    snprintf(chopstick_name, CHOPSTICK_NAME_LEN, chopstick_name_fm, no);
    sem_t *sem = sem_open(chopstick_name, O_CREAT, S_IRWXU, 1);
    if(SEM_FAILED == sem)
    {
	    perror("sem_open failed. ");
	    return -1;
    }
    int ret;
    ret = sem_post(sem);
    if(ret < 0)
    {
        perror("sem_post error. ");
    }
    return ret;
}
