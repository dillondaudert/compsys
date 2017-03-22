/* Date: 22/03/2016
 * Class: CS5540
 * Assignment: Peterson's Algorithm
 * Author: Dillon Daudert
 */

/* define _DEFAULT_SOURCE for usleep compatibility */
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CHARS 1200

/* global variables */
bool flag[2] = {false, false};
int turn;
int count = 0;
char printchar[2] = {'a', 'b'};


void *t_func(void *tid)
{
    long id = (long) tid;
    long other_id = id == 0 ? 1 : 0;
    while(count < MAX_CHARS){

        flag[id] = true;
        turn = other_id;
        while(flag[other_id] && turn == other_id){
            usleep(50000);
        }
        /* start critical section */
        if (count % 30 == 0) {
            fprintf(stdout, "%c", '\n');
        } else {
            fprintf(stdout, "%c", printchar[id]);
            fflush(stdout);
        }
        count++;
        /* end critical section */ 
        flag[id] = false;

    }

    return NULL;
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
