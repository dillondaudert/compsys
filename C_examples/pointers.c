#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void *my_malloc(unsigned int num_bytes);

int main(void)
{
    int i = 0;
    void *voi = &i;
    printf("Size of int: %lu\nSize of void*: %lu\n", sizeof i, sizeof voi);
    int *mem = my_malloc(2 * (unsigned int)sizeof(int));
    printf("Header: %d, Payload: %d, Footer: %d\n", *(mem-1), *mem, *(mem+2));
    free(mem-1);
    return 0;
}


/* Pass in number of bytes to be allocated
 * (ceil[num_bytes/intsize]) + 2 = block size in ints
 */
void *my_malloc(unsigned int num_bytes)
{
    //Size of block in ints
    unsigned int block_units = (int) ceil((double)num_bytes / sizeof(int)) + 2;
    void *block = calloc(block_units,sizeof(int));

    void *payload = (void *)((int *)block + 1);

    //Set header to block_size in bytes
    *(unsigned int *)block = block_units*sizeof(int);

    //Set footer to block_size in bytes
    *((unsigned int *)block + block_units - 1) = block_units*sizeof(int);
    return payload;
}
