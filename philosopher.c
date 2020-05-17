#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

int philosophers_count = 0;
pid_t *philosophers = NULL;
bool create_philosophers(int count)
{
    if(count <= 0)
    {
        return false;
    }
    philosophers = (pid_t *)malloc(sizeof(pid_t) * count);
    if(NULL == philosophers)
    {
        perror("malloc failed. ");
        return false;
    }
    int no;
    for(no = 0; no < count; no++)
    {
        pid_t philosopher = 0;
        philosopher = fork();
        if(philosopher < 0)
        {
            perror("fork failed! \n");
            break;
        }
        if(0 == philosopher)
	{
            char str_philosopher_no[]    = "123456789";
            char str_philosopher_count[] = "123456789";
            snprintf(str_philosopher_no, strlen(str_philosopher_no), "%d", no);
            snprintf(str_philosopher_count,  strlen(str_philosopher_count), "%d", count);
            /*                          argv[0]           argv[1] */
            if(execlp("./philosopher", "./philosopher", 
                     str_philosopher_no, str_philosopher_count, (char *)0) < 0)
            {
                perror("execlp failed.");
            }
            exit(0);
	} 
        philosophers[no] = philosopher;
    }
    philosophers_count = no;
    return ((no > 0) && (no >= count));
}

void wait_all_philosophers_die()
{
    int i;
    for(i = 0; i < philosophers_count; i++)
    {
	waitpid(philosophers[i], NULL, 0);
	printf("philosopher%d (%08X) died.\n", i, philosophers[i]);
    }
    free(philosophers);
}
