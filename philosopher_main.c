#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "chopstick.h"

const char* gettime();
const int DURATION_100MS = 100000;
const int DURATION_1S    = 1000000;
int main(int argc, char *argv[])
{
    pid_t parent = getppid();
    pid_t self   = getpid();
    int no = -1;
    int totalNum = -1;
    if(argc >= 2)
    {
        no = strtoul(argv[1], NULL, 0);
    }
    if(argc >= 3)
    {
        totalNum = strtoul(argv[2], NULL, 0);
    }
    int first_chopstick_no = (no %2 == 0) ? no : (no + 1) % totalNum ;
    int second_chopstick_no= (no %2 != 0) ? no : (no + 1) % totalNum ;
    int eating_count   = 0;
    int conflict_count = 0;
	unsigned int rand_time; 
	FILE* file_rand = fopen("/dev/random", "rb");
	fread(&rand_time, sizeof(rand_time), 1, file_rand);

    while(1)
    {
	    fread(&rand_time, sizeof(rand_time), 1, file_rand);
     	rand_time = rand_time % DURATION_1S  + DURATION_100MS;
        pick_up_chopstick(first_chopstick_no);
        if(try_pick_up_chopstick(second_chopstick_no) < 0)    
        {
	        put_down_chopstick(first_chopstick_no);	
            conflict_count++;
            printf("phisolopher%d (%08X) is thinking due to conflict. conflict_count: %d\n",
                   no, self, conflict_count);
	        usleep(rand_time);
	        continue;
		}

	    eating_count++;
	   /* printf("phisolopher%d (%08X) is eating. parent is %08X, total philosopher number is %d\n", 
               no, self, parent, totalNum); */
        printf("phisolopher%d (%08X) is eating with chopstick%d and chopstick%d, eating_count: %d\n", 
               no, self, first_chopstick_no, second_chopstick_no, eating_count);
	    fread(&rand_time, sizeof(rand_time), 1, file_rand);
     	rand_time = rand_time % DURATION_1S  + DURATION_100MS;
        usleep(rand_time);
        printf("phisolopher%d (%08X) is done   with chopstick%d and chopstick%d, eating_count: %d\n", 
               no, self, first_chopstick_no, second_chopstick_no, eating_count);
        put_down_chopstick(second_chopstick_no);
        put_down_chopstick(first_chopstick_no);
        printf("phisolopher%d (%08X) is thinking.\n", no,self);
	    fread(&rand_time, sizeof(rand_time), 1, file_rand);
     	rand_time = rand_time % DURATION_1S  + DURATION_100MS;
        usleep(rand_time);
    }
    return 0;
}

const char* gettime()
{
    static char time_buf[] = "[2016-08-05:15:37.30000]";

	time_t t;
	struct tm *tmp;

    time(&t);
    tmp = localtime(&t);

	strftime(time_buf, strlen(time_buf), "[%F:%H:%M:%S]", tmp);
	return time_buf;
}
