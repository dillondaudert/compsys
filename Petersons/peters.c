/*Simple implementation of Peterson's Algorithm for synchonization between two 
* processes
*/

/* define _BSD_SOURCE for usleep compatibility */
#define _BSD_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LOOP 60

/* global variables */
bool flag[2] = {false, false};
int turn;
int count = 0;
char printchar[2] = {'a', 'b'};


void *t_func(void *tid)
{
    long id = (long) tid;
    long other_id = id == 0 ? 1 : 0;
    while(true){
        flag[id] = true;
        turn = other_id;
        while(flag[other_id] && turn == other_id){
            usleep(50000);
        }
        /* start critical section */
        count++;
        if (count > 30) {
            count = 0;
            fprintf(stdout, "%c", '\n');
        } else {
            fprintf(stdout, "%c", printchar[id]);
            fflush(stdout);
        }
        /* end critical section */ 
        flag[id] = false;
    }
}

int main(int argc, char **argv)
{
    pthread_t threads[2];
    long ids; 
    int err;
    void *status;
    for(ids = 0; ids < 2; ids++){
        err = pthread_create(&threads[ids], NULL, t_func, (void *)ids);
        if (err) {
            printf("pthread_create error! %d\n", err);
            exit(-1);
        }
    }

    for(ids = 0; ids < 2; ids++){
        err = pthread_join(threads[ids], &status);
        if (err) {
            printf("pthread_join error! %d\n", err);
            exit(-1);
        }
        
    }

    pthread_exit(NULL);

}
