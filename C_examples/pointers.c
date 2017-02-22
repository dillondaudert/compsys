#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

//Note: Align must be 8 if size_t is used for headers and footers
#define ALIGN 8

void *my_malloc(size_t num_bytes);
size_t *header(void *mem);
size_t *footer(void *mem);
size_t size(void *mem);

int main(void)
{
    int *mem = my_malloc(3 * sizeof(int));
    printf("Header: %lu, Size: %lu, Footer: %lu\n", *header(mem), 
                                                    size(mem), *footer(mem));
    mem[0] = 10;
    mem[1] = 20;
    printf("mem[0]: %d, mem[1]: %d\n",mem[0], mem[1]);
    free(header(mem));
    return 0;
}


/* Pass in number of bytes to be allocated
 * Add 8 bytes for header+footer (2 ints)
 * Align block according to ALIGN
 *     (ceil[(num_bytes + 8)/ALIGN])  = block size in ALIGNed memory
 */
void *my_malloc(size_t num_bytes)
{
    //Size of block in ALIGN count and bytes
    size_t block_units = (int) ceil((double)(num_bytes + 2*sizeof(size_t)) / ALIGN);
    size_t block_size = block_units*ALIGN;

    //Get pointer to very start of memory, here would do search for free block
    //of size block_size to allocate
    void *block = calloc(block_units,ALIGN);

    //Set header to block_size in bytes, add one to mark as allocated
    *(size_t *)block = block_size + 1;

    //Get pointer to payload
    void *payload = (void *)((size_t *)block + 1);

    //Divide block into units of size_t, then jump to last location in block
    //Set footer to block_size in bytes, add one to mark as allocated
    *((size_t *)block + (block_size/sizeof(size_t)) - 1) = block_size + 1;

    return payload;
}

/*
 * Return the total block size in bytes
 */
size_t size(void *mem)
{
    size_t *head = header(mem);
    //ALIGN tells the alignment, subtracting 1 gives: 11111000 if ALIGN=8
    return *head & ~(ALIGN-1);
}

/*
 * Return the header of a payload in memory
 */
size_t *header(void *mem)
{
    return (size_t *)mem - 1;
}

size_t *footer(void *mem)
{
    size_t block_size = size(mem);
    size_t *head = header(mem);
    size_t *foot = head + (block_size/sizeof(size_t) - 1);
    return foot;
}
