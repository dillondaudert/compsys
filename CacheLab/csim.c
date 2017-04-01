//Date: 2/13/2017
//Class: CS 5541
//Assignment: Assignment 3 CacheLab
//Author: Dillon Daudert


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

//Typedef declarations

typedef struct Line_s Line;
struct Line_s{
    unsigned char valid;
    unsigned long int tag;
    Line *nxt;
};

typedef struct {
    Line *head;
} Set;

typedef struct {
    Set * sets;
    int s, E, b;
    int hits, misses, evicts;
    int h_flag, m_flag, e_flag;
} Cache;


//Function literals
void printSummary(int hits,  /* number of  hits */
                  int misses, /* number of misses */
                  int evictions); /* number of evictions */
void printUsage();
Cache *_createCache(int s, int E, int b);
void _destroyCache(Cache *my_cache, int s);
void simCache(Cache *my_cache, char *tracefile);
void _cacheRead(Cache *my_cache, unsigned long int addr, int size);
void _cacheWrite(Cache *my_cache, unsigned long int addr, int size);


//Global variables
int verbose = 0;

/*
 * simCache - simulate the behavior of a cache iterating over a tracefile
 */
void simCache(Cache *my_cache, char *tracefile)
{
    //open tracefile
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char instr;
    unsigned long int addr;
    int size;

    fp = fopen(tracefile, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);


    //for line in file
    while ((read = getline(&line, &len, fp)) != -1){
        my_cache->h_flag = my_cache->m_flag = my_cache->e_flag = 0;
        sscanf(line, " %c %lx, %d", &instr, &addr, &size);
        //switch on instruction
        switch(instr){
        case 'I':
            break;
        case 'L':
            _cacheRead(my_cache, addr, size);
            if(verbose){
                printf("%c %8lx,%d ", instr, addr, size);
                if(my_cache->h_flag)
                    printf("hit ");
                if(my_cache->m_flag)
                    printf("miss ");
                if(my_cache->e_flag)
                    printf("eviction ");
                printf("\n");
            }
            break;
        case 'S':
            _cacheWrite(my_cache, addr, size);
            if(verbose){
                printf("%c %8lx,%d ", instr, addr, size);
                if(my_cache->h_flag)
                    printf("hit ");
                if(my_cache->m_flag)
                    printf("miss ");
                if(my_cache->e_flag)
                    printf("eviction ");
                printf("\n");
            }
            break;
        case 'M':
            _cacheRead(my_cache, addr, size);
            if(verbose){
                printf("%c %8lx,%d ", instr, addr, size);
                if(my_cache->h_flag)
                    printf("hit ");
                if(my_cache->m_flag)
                    printf("miss ");
                if(my_cache->e_flag)
                    printf("eviction ");
            }

            my_cache->h_flag = my_cache->m_flag = my_cache->e_flag = 0;
            _cacheWrite(my_cache, addr, size);
            if(verbose){
                if(my_cache->h_flag)
                    printf("hit ");
                if(my_cache->m_flag)
                    printf("miss ");
                if(my_cache->e_flag)
                    printf("eviction ");
                printf("\n");
            }
            break;
        }

    }

    fclose(fp);
    if(line)
        free(line);

}

void _cacheWrite(Cache *my_cache, unsigned long int addr, int size)
{
    //The masks are calculated from the following:
    //addr = [tag | s bits | b bits]
    unsigned int t_mask = ~((1 << (my_cache->s + my_cache->b)) - 1);
    unsigned int tag = addr & t_mask;
    unsigned int b_mask = ((1 << my_cache->b) - 1);
    unsigned int s_mask = ~(t_mask|b_mask);
    int set_index = (addr & s_mask)>>my_cache->b;

    //For the given set, iterate over all lines until valid+tagmatch, or end
    Line *curr, *prev;
    prev = curr = my_cache->sets[set_index].head;
    
    while(curr->nxt != NULL){
        if(curr->valid && (curr->tag == tag)){
            //Hit, write-through
            my_cache->h_flag = 1;
            my_cache->hits++;
            //If more than one lin, and this is not the first one
            if(prev != curr){
                //Cache location accessed, move to front of LL
                prev->nxt = curr->nxt;
                curr->nxt = my_cache->sets[set_index].head;
                my_cache->sets[set_index].head = curr;
            }
            break;
        }else if(curr->valid == 0){
            //Cold miss
            my_cache->m_flag = 1;
            my_cache->misses++;

            if(prev == curr){
                //Empty set, do not move pointers
                curr->tag = tag;
                curr->valid = 1;
            }else{
                //Move the loaded data to front of set list
                curr->valid = 1;
                curr->tag = tag;
                prev->nxt = curr->nxt;
                curr->nxt = my_cache->sets[set_index].head;
                my_cache->sets[set_index].head = curr;
            }
            break;
        }
        prev = curr;
        curr = curr->nxt;
    }
    
    if(curr->nxt == NULL){
        my_cache->m_flag = 1;
        my_cache->misses++;
        my_cache->e_flag = 1;
        my_cache->evicts++;
        //Evict last data, load new, move to head
        prev->nxt = NULL;
        curr->valid = 1;
        curr->tag = tag;
        curr->nxt = my_cache->sets[set_index].head;
        my_cache->sets[set_index].head = curr;

    }
}

void _cacheRead(Cache *my_cache, unsigned long int addr, int size)
{
    //The masks are calculated from the following:
    //addr = [tag | s bits | b bits]
    unsigned int t_mask = ~((1 << (my_cache->s + my_cache->b)) - 1);
    unsigned int tag = addr & t_mask;
    unsigned int b_mask = ((1 << my_cache->b) - 1);
    unsigned int s_mask = ~(t_mask|b_mask);
    int set_index = (addr & s_mask)>>my_cache->b;

    //For the given set, iterate over all lines until valid+tagmatch, or end
    Line *curr, *prev;
    prev = curr = my_cache->sets[set_index].head;
    
    while(curr->nxt != NULL){
        if(curr->valid && (curr->tag == tag)){
            //Hit
            my_cache->h_flag = 1;
            my_cache->hits++;
            //If more than one lin, and this is not the first one
            if(prev != curr){
                //Cache location accessed, move to front of LL
                prev->nxt = curr->nxt;
                curr->nxt = my_cache->sets[set_index].head;
                my_cache->sets[set_index].head = curr;
            }
            break;
        }else if(curr->valid == 0){
            //Cold miss
            my_cache->m_flag = 1;
            my_cache->misses++;

            if(prev == curr){
                //Empty set, do not move pointers
                curr->tag = tag;
                curr->valid = 1;
            }else{
                //Move the loaded data to front of set list
                curr->valid = 1;
                curr->tag = tag;
                prev->nxt = curr->nxt;
                curr->nxt = my_cache->sets[set_index].head;
                my_cache->sets[set_index].head = curr;
            }
            break;
        }
        prev = curr;
        curr = curr->nxt;
    }
    
    if(curr->nxt == NULL){
        my_cache->m_flag = 1;
        my_cache->misses++;
        my_cache->e_flag = 1;
        my_cache->evicts++;
        //Evict last data, load new, move to head
        prev->nxt = NULL;
        curr->valid = 1;
//        printf("Evicting cache location with tag: %8lx\n", curr->tag);
        curr->tag = tag;
        curr->nxt = my_cache->sets[set_index].head;
        my_cache->sets[set_index].head = curr;

    }
    
}


/* 
 * printSummary - Summarize the cache simulation statistics. Student cache simulators
 *                must call this function in order to be properly autograded. 
 */
void printSummary(int hits, int misses, int evictions)
{
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
    FILE* output_fp = fopen(".csim_results", "w");
    assert(output_fp);
    fprintf(output_fp, "%d %d %d\n", hits, misses, evictions);
    fclose(output_fp);
}

void printUsage()
{
    fprintf(stderr, "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
    return;
}

int main(int arc, char **argv)
{
    Cache *my_cache;

    int s_bits = 0, E_lines = 0, b_bits = 0;
    char *tracefile;

    /*
     * Handle command line options
     */
    int c;
    int sflg = 0, eflg = 0, bflg = 0, tflg = 0, errflg = 0;
    while ((c = getopt(arc, argv, ":hvs:E:b:t:")) != -1){
        switch(c){
        case 'h':
            errflg++;
            break;
        case 'v':
            verbose = 1;
            break;
        case 's':
            s_bits = atoi(optarg);
            sflg++;
            break;
        case 'E':
            E_lines = atoi(optarg);
            eflg++;
            break;
        case 'b':
            b_bits = atoi(optarg);
            bflg++;
            break;
        case 't':
            tracefile = optarg;
            tflg++;
            break;
        case ':':
            //option missing argument, print usage
            fprintf(stderr, "Option %s requires an argument\n", optarg);
            errflg++;
            break;
        case '?':
            //invalid option, print usage
            fprintf(stderr, "Unrecognized option: -%s\n", optarg);
            errflg++;
            break;
        }
    }
    if(errflg){
        printUsage();
        exit(2);
    }
    if(!(sflg && eflg && bflg && tflg)){
        fprintf(stderr, "Missing option!\n");
        printUsage();
        exit(2);
    }

    //Build cache
    //printf("s: %d, E: %d, b: %d\n", s_bits, E_lines, b_bits);
    my_cache = _createCache(s_bits, E_lines, b_bits);

    //Iterate over file here
    simCache(my_cache, tracefile);

    printSummary(my_cache->hits, my_cache->misses, my_cache->evicts);


    //Teardown
    _destroyCache(my_cache, s_bits);
    return 0;
}


/*
 * _createCache - allocate memory for the my_cache struct to simulate the cache
 *              - pass in the # of sets, # of lines/set, # of data bits in address
 */
Cache *_createCache(int s, int E, int b)
{
    //Address: tag | set bits | data bits
    //Cache: 2^s sets, E lines/set, 2^b data bytes/block
    int num_sets = (1<<s);

    Cache *my_cache = calloc(1, sizeof(Cache));
    my_cache->s = s;
    my_cache->E = E;
    my_cache->b = b;
    my_cache->hits = my_cache->misses = my_cache->evicts = 0;

    //Create an array of 2^s sets
    my_cache->sets = calloc(num_sets, sizeof(Set));

    //For each set, create E lines (linked list)
    int i, j;
    for(i = 0; i < num_sets; i++){
        Set *curr_set = &my_cache->sets[i];
        curr_set->head = calloc(1, sizeof(Line));
        Line *curr_line = curr_set->head;
        //Create linked list of lines
        for(j = 0; j < E; j++){
            curr_line->nxt = calloc(1, sizeof(Line));
            curr_line = curr_line->nxt;
        }
    }
    return my_cache;
}

/*
 * _destroyCache - deallcoate memory for the cache to avoid mem leaks :)
 */
void _destroyCache(Cache *my_cache, int s)
{
    int num_sets = (1<<s);
    int i;
    for(i = 0; i < num_sets; i++){
        Line *head = my_cache->sets[i].head;
        Line *tmp;
        while(head != NULL){
            tmp = head;
            head = head->nxt;
            free(tmp);
        }
    }
    //free(my_cache->sets);
    free(my_cache);
}
