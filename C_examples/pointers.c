#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define header(ptr) ((void *)((char *)ptr - SIZE_T_SIZE))
#define size(ptr) (*((size_t *)header(ptr)) & ~0x7)
#define footer(ptr) ((void *)((char *)header(ptr) + size(ptr) - SIZE_T_SIZE))

#define prev(block) ( (void *)((char *)prev_header(block) + SIZE_T_SIZE) )
#define prev_header(block) ((void *)((char *)block - (*((size_t *)prev_footer(block)) & ~0x7)))
#define prev_footer(block) (header(block))
#define is_alloc(ptr) (*((size_t *)ptr) & 0x1)

#define next_header(block) (void *)((char *)block + (*((size_t *)block) & ~0x7))

void *my_malloc(size_t num_bytes);
int mm_check(void *, void *);

int main(void)
{
    /* create two stacks, one with contiguous free blocks*/
    void *my_stack = calloc(10, SIZE_T_SIZE);
    *((size_t *)my_stack) = 4*SIZE_T_SIZE + 1;
    void *mem1 = (void *)((char *)my_stack + SIZE_T_SIZE);
    *((size_t *)((char *)my_stack + 3*SIZE_T_SIZE)) = 4*SIZE_T_SIZE + 1;

    printf("Header: %p,%lu; Footer: %p,%lu\n", header(mem1), *((size_t *)header(mem1)), 
        footer(mem1), *((size_t *)footer(mem1)));

    *((size_t *)((char *)my_stack + 4*SIZE_T_SIZE)) = 6*SIZE_T_SIZE + 1;
    void *mem2 = (void *)((char *)my_stack + 5*SIZE_T_SIZE);
    *((size_t *)((char *)my_stack + 9*SIZE_T_SIZE)) = 6*SIZE_T_SIZE + 1;

    printf("Header2: %p,%lu; Footer2: %p,%lu\n", header(mem2), *((size_t *)header(mem2)), 
        footer(mem2), *((size_t *)footer(mem2)));

    printf("Previous of Header2: %p,%lu\n", prev_header(header(mem2)), *((size_t *)prev_header(header(mem2))));
    
    /* stack 2 */
    /* create two stacks, one with contiguous free blocks*/
    void *my_stack2 = calloc(14, SIZE_T_SIZE);
    void *end_stack2 = (void *)((char *)my_stack2 + 14*SIZE_T_SIZE);
    *((size_t *)my_stack2) = 4*SIZE_T_SIZE + 1;
    *((size_t *)((char *)my_stack2 + 3*SIZE_T_SIZE)) = 4*SIZE_T_SIZE + 1;

    *((size_t *)((char *)my_stack2 + 4*SIZE_T_SIZE)) = 6*SIZE_T_SIZE;
    *((size_t *)((char *)my_stack2 + 9*SIZE_T_SIZE)) = 6*SIZE_T_SIZE;

    *((size_t *)((char *)my_stack2 + 10*SIZE_T_SIZE)) = 4*SIZE_T_SIZE;
    *((size_t *)((char *)my_stack2 + 13*SIZE_T_SIZE)) = 4*SIZE_T_SIZE;

    mm_check(my_stack2, end_stack2);

    free(my_stack);
    free(my_stack2);
    return 0;
}

/* Check the heap for errors */
int mm_check(void *heap_lo, void *heap_hi)
{
    int i = 1;
    void *curr_block, *next_block;
    curr_block = heap_lo;
    next_block = next_header(curr_block);
    while (next_block < heap_hi) {
        if (!is_alloc(curr_block) && !is_alloc(next_block)) {
            printf("Contiguous free blocks %d and %d found\n", i, i+1);
        }
        curr_block = next_block;
        next_block = next_header(curr_block);
        i++;
    }
    return 0;
}


/* Pass in number of bytes to be allocated
 * Round up to a multiple of Align, add 2*Align for header, footer
 */
void *my_malloc(size_t num_bytes)
{
    //Size of block in ALIGN count and bytes
    size_t block_units = (int) ceil((double)(num_bytes + 2*sizeof(size_t)) / ALIGNMENT);
    size_t block_size = block_units*ALIGNMENT;

    //Get pointer to very start of memory, here would do search for free block
    //of size block_size to allocate
    void *block = calloc(block_units,ALIGNMENT);

    //Set header to block_size in bytes, add one to mark as allocated
    *(size_t *)block = block_size + 1;

    //Get pointer to payload
    void *payload = (void *)((size_t *)block + 1);

    //Divide block into units of size_t, then jump to last location in block
    //Set footer to block_size in bytes, add one to mark as allocated
    *((size_t *)block + (block_size/sizeof(size_t)) - 1) = block_size + 1;

    return payload;
}
