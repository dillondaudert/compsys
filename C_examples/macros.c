#define header(ptr) ((void *)((char *)ptr - SIZE_T_SIZE))
#define size(ptr) (*((size_t *)header(ptr)) & ~0x7)
#define footer(ptr) ((void *)((char *)header(ptr) + size(ptr) - SIZE_T_SIZE))

#define prev(block) ( (void *)((char *)prev_header(block) + SIZE_T_SIZE) )
#define prev_header(block) ((void *)((char *)block - (*((size_t *)prev_footer(block)) & ~0x7)))
#define prev_footer(block) (header(block))
#define is_alloc(ptr) (*((size_t *)ptr) & 0x1)

#define next_header(block) (void *)((char *)block + (*((size_t *)block) & ~0x7))
int mm_check(void *, void *);

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
