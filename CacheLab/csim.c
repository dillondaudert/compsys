#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>

//Function literals
void printSummary(int hits,  /* number of  hits */
                  int misses, /* number of misses */
                  int evictions); /* number of evictions */
void printUsage();

//Global variables
int verbose = 0;
int s_bits = 0, E_lines = 0, b_bits = 0;
char *tracefile;

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
    fprintf(stderr, "Usage: \n");
    return;
}

int main(int arc, char **argv)
{
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
    
    //printSummary(0, 0, 0);
    return 0;
}
