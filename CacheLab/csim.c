#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//Function literals
void printSummary(int hits,  /* number of  hits */
                  int misses, /* number of misses */
                  int evictions); /* number of evictions */

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

int main()
{
    printSummary(0, 0, 0);
    return 0;
}
