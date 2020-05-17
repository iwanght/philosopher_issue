#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "philosopher.h" 
#include "chopstick.h"

int main(int argc, char * argv[])
{
    printf("Main pid %08X\n", getpid());
    int philosopher_count  = 5; 
    int chopstick_count = philosopher_count;   
    int count  = 5; 
    if(argc >= 2)
    {
       count = strtoul(argv[1], NULL, 0);
       if(1 >= count)
       { 
	   printf("illegal parameter: %d\n", count);
	   return -1;
       }
       philosopher_count = count;
       chopstick_count   = philosopher_count; 
    }
    create_chopsticks(chopstick_count);
    create_philosophers(philosopher_count);

    wait_all_philosophers_die();
    /* destroy_chopsticks(); */

    printf("Main exit\n");   
    return 0;
}
