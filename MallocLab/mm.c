/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
    /* Team name */
    "I have no team",
    /* First member's full name */
    "Dillon Daudert",
    /* First member's email address */
    "dillon.g.daudert@wmich.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define INIT_SIZE 1024
#define INCR_SIZE 256

/* all of these macros are called with ptrs to the payload of a block */
#define header(ptr) ((void *)((char *)ptr - SIZE_T_SIZE))
#define footer(ptr) ((void *)((char *)header(ptr) + size(header(ptr)) - SIZE_T_SIZE))

/* all of these macros are called with ptrs to the block header*/
#define size(block) (*((size_t *)block) & ~0x7)
#define is_alloc(block) (*((size_t *)block) & 0x1)

#define prev(block) ( (void *)((char *)prev_header(block) + SIZE_T_SIZE) )
#define prev_header(block) ((void *)((char *)block - (*((size_t *)prev_footer(block)) & ~0x7)))
#define prev_footer(block) (header(block))
#define next_header(block) (void *)((char *)block + (*((size_t *)block) & ~0x7))

/* macros for getting the free list pointers of a block*/
#define next_ptr(block) (void *)((char *)block + SIZE_T_SIZE)
#define prev_ptr(block) (void *)((char *)block + 2*SIZE_T_SIZE)

/* pointer to the head of the free list*/
static void *FREE_HEAD = NULL;


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    /* Initialize the first block of the free list */
    FREE_HEAD = mem_sbrk(INIT_SIZE);
    //set header
    *((size_t *)FREE_HEAD) = INIT_SIZE;
    //set footer
    *((size_t *)((char *)FREE_HEAD + INIT_SIZE - SIZE_T_SIZE)) = INIT_SIZE;
    //set pointers for list
    (size_t *)next_ptr(FREE_HEAD) = NULL;
    (size_t *)next_ptr(FREE_HEAD) = NULL;
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    /* look for first-fit block in */
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}


/* Check the heap for errors */
int mm_check()
{
    size_t global_sum = 0;

    void *curr_block, *next_block;
    curr_block = mm_heap_lo();
    next_block = next_header(curr_block);
    while (next_block < mm_heap_hi()) {
        /* keep a running sum to check against mem_heapsize at the end */
        global_sum += size(curr_block);

        /* check for contiguous free blocks */
        if (!is_alloc(curr_block) && !is_alloc(next_block)) {
            return CONTIGUOUS_FREE;
        }
        curr_block = next_block;
        next_block = next_header(curr_block);
    }

    if (global_sum != mm_heapsize()) {
        return HEAPSIZE_MISMATCH;
    }

    return 0;
}
