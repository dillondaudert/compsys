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

/* private static helper functions */
static int mm_check (void);
static void *_mm_incr_heap(size_t size);
static void _mm_insert_link(void *);
static void _mm_remove_link(void *);
static char *_mm_error(int);
static int _mm_check_freelist(void);
static int _mm_find_freeblock(void *);

/* error defines for mm_check clarity */
#define CONTIGUOUS_FREE 1
#define HEAPSIZE_MISMATCH 2
#define INVALID_BLOCK 3
#define NULL_PTR 4
#define UNLISTED_FREE 5
#define SHORT_FREE_LIST 6

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

/* mm_check debug flag */
#define DEBUG 0

#define DEBUG_PRINTF(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                            __LINE__, __func__, __VA_ARGS__); } while (0)

/* double word (8) alignment */
#define ALIGNMENT 8

/* minimize size: header (8) + 2 pointers (16) + footer (8) */
#define MIN_SIZE 4*ALIGNMENT

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define INIT_SIZE 256
#define INCR_SIZE 256

/* all of these macros are called with ptrs to the payload of a block */
#define header(ptr) ((void *)((char *)ptr - SIZE_T_SIZE))

/* all of these macros are called with ptrs to the block header*/
#define footer(block) ((void *)((char *)block + size(block) - SIZE_T_SIZE))
#define size(block) (*((size_t *)block) & ~0x7)
#define is_alloc(block) (*((size_t *)block) & 0x1)

#define prev(block) ( (void *)((char *)prev_header(block) + SIZE_T_SIZE) )
#define prev_header(block) ((void *)((char *)block - (*((size_t *)prev_footer(block)) & ~0x7)))
#define prev_footer(block) (header(block))
#define next_header(block) (void *)((char *)block + (*((size_t *)block) & ~0x7))

/* macros for getting the free list pointers of a block*/
#define next_ptr(block) (size_t **)((char *)block + SIZE_T_SIZE)
#define prev_ptr(block) (size_t **)((char *)block + 2*SIZE_T_SIZE)

/* pointer to the head of the free list*/
static void *FREE_HEAD;
static size_t FREE_LENGTH;


/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    FREE_HEAD = NULL;
    FREE_LENGTH = 0;
    /* Initialize the first block of the free list */
    void *newmem;
    newmem = mm_malloc(INIT_SIZE);

    if(newmem == NULL)
        return -1;

    mm_free(newmem);

    if (DEBUG) {
        int err = mm_check();
        DEBUG_PRINTF("%s\n", _mm_error(err));
        if (err != 0) exit(1);
    }
    return 0;
}

/* 
 * mm_malloc - Finds the first block of appropriate size in the free list and returns it
 * If the first block found is too large, the excess memory is split off and kept in the list
 * If no block is found, the heap is expanded by at least INCR_SIZE bytes
 */
void *mm_malloc(size_t size)
{
    int i;
    size_t newsize = ALIGN(size) + 2*SIZE_T_SIZE;
    if (newsize < MIN_SIZE)
        newsize = MIN_SIZE;
    void *curr_block = FREE_HEAD;
    void *new_payld;

    /* look for first-fit block in the free list */
    for (i = 0; i < FREE_LENGTH; i++) {
        if (size(curr_block) >= newsize) {

            /* split off excess if enough left over*/
            size_t excess = size(curr_block) - newsize;
            if (excess >= MIN_SIZE) {

                /* split off the end, curr_block pointers unmodified! */
                void *remain_block = curr_block;
                *(size_t *)remain_block = excess;
                *(size_t *)footer(remain_block) = excess;

                curr_block = next_header(remain_block);
                *(size_t *)curr_block = newsize;
                *(size_t *)footer(curr_block) = newsize;



            } else {
                /* remove the block from the list */
                _mm_remove_link(curr_block);
                FREE_LENGTH--;
            }

            DEBUG_PRINTF("MALLOC: block size %u | free length: %d\n", size(curr_block), FREE_LENGTH);

            /* mark removed block as allocated */
            *(size_t *)curr_block |= 0x1;
            new_payld = (void *)((char *)curr_block + SIZE_T_SIZE);
            *(size_t *)footer(curr_block) |= 0x1;

            if (DEBUG) {
                int err = mm_check();
                DEBUG_PRINTF("%s\n", _mm_error(err));
                if (err != 0) exit(1);
            }
            return new_payld;
        }
        /* not found yet, check next one */
        curr_block = (void *)(*next_ptr(curr_block));
    }
    
    void *new_mem, *excess_block;
    /* no item in list was found, incr heap */
    if (newsize+MIN_SIZE < INCR_SIZE) {

        excess_block = _mm_incr_heap(INCR_SIZE);

        size_t excess = INCR_SIZE - newsize;

        void *excess_ptr = (void *)((char *)excess_block + SIZE_T_SIZE);
        *(size_t *)excess_block = excess;
        *(size_t *)footer(excess_block) = excess;

        /* break off allocated chunk from end */
        new_mem = next_header(excess_block);
        new_payld = (void *)((char *)new_mem + SIZE_T_SIZE);
        *(size_t *)new_mem = newsize | 0x1;
        *(size_t *)footer(new_mem) = newsize | 0x1;

        /* free the preceeding excess */
        mm_free(excess_ptr);


    } else {
        new_mem = _mm_incr_heap(newsize);
        /* mark as allocated */
        *(size_t *)new_mem |= 0x1;
        *(size_t *)footer(new_mem) |= 0x1;
        new_payld = (void *)((char *)new_mem + SIZE_T_SIZE);
    }


    if (DEBUG) {
        int err = mm_check();
        DEBUG_PRINTF("%s\n", _mm_error(err));
        if (err != 0) exit(1);
    }

    return new_payld;
}


/*
 * mm_free - Free a previously allocated block. 
 * If the freed block is adjacent to any other free blocks, they are coalesced into a single block
 * Freed block are placed into a single linked list via the functions _mm_insert_link, _mm_remove_link
 */
void mm_free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }
    void *freed_block, *prev_block, *next_block;
    freed_block = header(ptr);

    DEBUG_PRINTF("FREE: block size %u | free length: %d\n", size(freed_block), FREE_LENGTH);

    prev_block = prev_header(freed_block);
    next_block = next_header(freed_block);
    size_t new_size;

    /* if both sides have free blocks */
    if ((!(prev_block < mem_heap_lo()) && !is_alloc(prev_block)) 
        && (!(next_block >= mem_heap_hi()) && !is_alloc(next_block))) {
        if(DEBUG) fprintf(stderr, "~ COALESCE previous and next ");

        /* remove all blocks from free list */
        _mm_remove_link(prev_block);
        /* because we are removing two links and replacing with one, decrement count */
        FREE_LENGTH--;
        _mm_remove_link(next_block);

        /* update sizes; header of prev, footer of next */
        new_size = size(prev_block) + size(freed_block) + size(next_block);
        *(size_t *)prev_block = new_size;
        *(size_t *)footer(next_block) = new_size;
        _mm_insert_link(prev_block);
        DEBUG_PRINTF("INSERT block size (header/footer): %u/%u, new free length: %d\n", size(prev_block), size(footer(prev_block)), FREE_LENGTH);


    }else if (!(prev_block < mem_heap_lo()) && !is_alloc(prev_block)) {
        /* coalesce with previous contiguous block */
        if(DEBUG) fprintf(stderr, "~ COALESCE previous ");

        _mm_remove_link(prev_block);

        new_size = size(freed_block) + size(prev_block);
        
        /* update prev header, freed_block footer with new size */
        *(size_t *)prev_block = new_size;
        *(size_t *)footer(freed_block) = new_size;

        DEBUG_PRINTF("INSERT block size (header/footer): %u/%u, new free length: %d\n", size(prev_block), size(footer(prev_block)), FREE_LENGTH);
        

        _mm_insert_link(prev_block);
        /* we coalesced, so do not increment or decrement count */

    } else if (!(next_block >= mem_heap_hi()) && !is_alloc(next_block)) {
        /* coalesce with next contiguous block */
        if(DEBUG) fprintf(stderr, "~ COALESCE next ");
        
        _mm_remove_link(next_block);

        new_size = size(freed_block) + size(next_block);
        /* update freed_block header, next_block footer with new size */
        *(size_t *)freed_block = new_size;
        *(size_t *)footer(next_block) = new_size;


        _mm_insert_link(freed_block);
        DEBUG_PRINTF("INSERT block size (header/footer): %u/%u, new free length: %d\n", size(freed_block), size(footer(freed_block)), FREE_LENGTH);

    /* if there was no coalescing, add to beginning of free list */ 
    } else {
        _mm_insert_link(freed_block);
        /* increment count */
        FREE_LENGTH++;
       DEBUG_PRINTF("~ INSERT block size (header, footer): %u/%u, new free length: %d\n", size(freed_block), size(footer(freed_block)), FREE_LENGTH);
        
    }

    if (DEBUG) {
        int err = mm_check();
        DEBUG_PRINTF("%s\n", _mm_error(err));
        if (err != 0) exit(1);
    }

    /* NULL the user pointer */
    ptr = NULL;
}

/*
 * _mm_remove_link - remove a free block from the linked list
 */
void _mm_remove_link(void *rem_block)
{
    /* if this was the last link, set FREE_HEAD to null */
    if ((rem_block == FREE_HEAD) && (FREE_LENGTH == 1)) {
        FREE_HEAD = NULL;
        if(DEBUG) fprintf(stderr, "~ FREE_HEAD is now NULL\n");
    } else if (*next_ptr(rem_block) == *prev_ptr(rem_block)) {
        /* two items in list, remove rem_block and set other to HEAD */
        FREE_HEAD = (void *)(*next_ptr(rem_block));
        *next_ptr(FREE_HEAD) = (size_t *)FREE_HEAD;
        *prev_ptr(FREE_HEAD) = (size_t *)FREE_HEAD;

    } else {
        void *next_link = (void *)(*next_ptr(rem_block));
        void *prev_link = (void *)(*prev_ptr(rem_block));
        /* if we are removing the current head, replace */
        if (rem_block == FREE_HEAD) {
            FREE_HEAD = next_link;
        }
        *next_ptr(prev_link) = (size_t *)next_link;
        *prev_ptr(next_link) = (size_t *)prev_link;
    }

    /* remove links */
    *next_ptr(rem_block) = 0x0;
    *prev_ptr(rem_block) = 0x0;

}

/*
 * _mm_insert_link - insert a link into the free list
 */
void _mm_insert_link(void *add_block) 
{
    /* set allocated flag to false */
    *(size_t *)add_block &= ~0x1;
    *(size_t *)footer(add_block) &= ~0x1;

    /* if the list is empty, set FREE_HEAD to new block */
    if (FREE_HEAD == NULL) {
        FREE_HEAD = add_block;
        /* next, prev pointers to itself */
        *next_ptr(FREE_HEAD) = (size_t *)FREE_HEAD;
        *prev_ptr(FREE_HEAD) = (size_t *)FREE_HEAD;

    } else if (((void *)*next_ptr(FREE_HEAD)) == FREE_HEAD) {
        /* only HEAD in the list, add new block */
        *next_ptr(FREE_HEAD) = (size_t *)add_block;
        *prev_ptr(FREE_HEAD) = (size_t *)add_block;
        *next_ptr(add_block) = (size_t *)FREE_HEAD;
        *prev_ptr(add_block) = (size_t *)FREE_HEAD;
    } else {
        /* three links to modify: last, curr, first */
        void *last_link = (void *)(*prev_ptr(FREE_HEAD));
        
        /* update new block pointers */
        *next_ptr(add_block) = (size_t *)FREE_HEAD;
        *prev_ptr(add_block) = (size_t *)last_link;

        /* update last link pointer */
        *next_ptr(last_link) = (size_t *)add_block;

        /* update first link pointer */
        *prev_ptr(FREE_HEAD) = (size_t *)add_block;
    }
}

/*
 * _mm_incr_heap - when a block of sufficient size is not found in the free list
 * in a call to mm_malloc, increment the size of the heap to accomodate
 * the request.
 */
void *_mm_incr_heap(size_t size)
{
    void *block = mem_sbrk(size);
    if (block == (void *)-1) {
        if(DEBUG) fprintf(stderr, "mem_sbrk returned -1, no further heap available!\n");
        return NULL;
    } else {
        *(size_t *)block = size;
        *(size_t *)footer(block) = size;
        return block;
    }

}
/*
 * mm_realloc - Reallocates a block pointed at by ptr to a new block of size bytes.
 * If the reallocation contracts the size of the block, the excess space is freed
 * Else if the reallocation expands the size of the block, a new block is malloc'd
 * and the data is copied over. The old block is freed.
 */
void *mm_realloc(void *ptr, size_t size)
{
    fprintf(stderr, "Realloc called\n");

    if (ptr == NULL) {
        return mm_malloc(size);
    } else if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    void *block = header(ptr);

    size_t newsize = ALIGN(size) + 2*SIZE_T_SIZE;
    if (newsize < MIN_SIZE)
        newsize = MIN_SIZE;

    if (size(block) >= newsize) {
        /* CONTRACT BLOCK */
        size_t excess_size = size(block) - newsize;
        if (excess_size >= MIN_SIZE) {
            /* split off and free excess */
            *(size_t *)block = newsize + 1;
            *(size_t *)footer(block) = newsize + 1;

            void *excess_block = next_header(block);
            *(size_t *)excess_block = excess_size;
            *(size_t *)footer(excess_block) = excess_size;
            void *excess_ptr = (void *)((char *)excess_block + SIZE_T_SIZE);
            mm_free(excess_ptr);
        }
        return ptr;
    } else {
        /* EXPAND BLOCK */
        void *newptr = mm_malloc(newsize);
        if (newptr == NULL)
          return NULL;

        /* size(block) < newsize */
        memcpy(newptr, ptr, size(block));
        mm_free(ptr);
        return newptr;
    
    }
}


/* Check the heap for errors */
int mm_check()
{
    void *curr_block, *next_block;
    /* check that each free block in the list has valid headers, footers, pointers */
    int retval;
    if ((retval = _mm_check_freelist()) != 0) {
        return retval;
    }

    /* check that the heap contains valid nodes, with no contiguous frees */
    size_t global_sum = 0;

    curr_block = mem_heap_lo();
    next_block = next_header(curr_block);
    
    while (curr_block < mem_heap_hi()) {

        /* check that headers, footers match up */
        if ( (is_alloc(curr_block) != is_alloc(footer(curr_block))) || (size(curr_block) != size(footer(curr_block))) ){
            fprintf(stderr, "Heap contains invalid block: [%u|%d/%u|%d]\n",
                size(curr_block), is_alloc(curr_block), size(footer(curr_block)), 
                is_alloc(footer(curr_block)));
            return INVALID_BLOCK;
        }

        /* keep a running sum to check against mem_heapsize at the end */
        global_sum += size(curr_block);

        /* if the current block is marked free */
        if (!is_alloc(curr_block)) {
            /* check in free list */
            if ((retval = _mm_find_freeblock(curr_block)) != 0){
                return retval;
            }

            /* check no contiguous free blocks*/
            if (next_block < mem_heap_hi() && !is_alloc(next_block)) {
                return CONTIGUOUS_FREE;
            }
        }

        curr_block = next_block;
        next_block = next_header(curr_block);
    }

    if (global_sum != mem_heapsize()) {
        return HEAPSIZE_MISMATCH;
    }

    return 0;
}

/*
 * Check that the free list contains FREE_LENGTH nodes that are all valid
 */
int _mm_check_freelist(void)
{
    int list_count = 0;
    void *curr_block, *next_block;
    next_block = FREE_HEAD;

    while (list_count < FREE_LENGTH) {
        list_count++;
        
        curr_block = next_block;
        next_block = (void *)(*next_ptr(curr_block));
        /* check that curr is valid */
        if ( is_alloc(curr_block) || is_alloc(footer(curr_block)) || (size(curr_block) != size(footer(curr_block))) ){
            fprintf(stderr, "Free list node points to invalid block: [%u|%d/%u|%d]\n",
                size(curr_block), is_alloc(curr_block), size(footer(curr_block)), 
                is_alloc(footer(curr_block)));
            return INVALID_BLOCK;
        }else if (*next_ptr(curr_block) == NULL || *prev_ptr(curr_block) == NULL) {
        
            fprintf(stderr, "Free block has NULL LINKS: [%u|%d/%u|%d]\n",
                size(curr_block), is_alloc(curr_block), size(footer(curr_block)), 
                is_alloc(footer(curr_block)));
            return NULL_PTR;
        }
        /* check that we do not loop back to the start before FREE_LENGTH is reached */
        if (list_count != 1 && curr_block == FREE_HEAD) {
            fprintf(stderr, "Free list looped back to FREE_HEAD after %d links; expected %d\n", list_count, FREE_LENGTH);
            return SHORT_FREE_LIST;
        }
    }

    return 0;
}

/*
 * Verify that a free block in the heap is in the free list
 */
int _mm_find_freeblock(void *block)
{
    int list_count = 0;
    void *curr_block = FREE_HEAD;
    while (list_count < FREE_LENGTH) {
        list_count++;
        if (curr_block == block) {
            return 0;
        }
        curr_block = (void *)(*next_ptr(curr_block));
    }
    return UNLISTED_FREE;
}

/* 
 * Return a string describing the error encountered
 */
char *_mm_error(int errval)
{
    char *retstring;
    switch(errval){
        case 0:
            retstring = "No errors.";
            break;
        case 1:
            retstring = "Contiguous free blocks found heap.";
            break;
        case 2:
            retstring = "Counted blocks in heap do not match heap size";
            break;
        case 3:
            retstring = "An invalid block was found";
            break;
        case 4:
            retstring = "A free block contained a null pointer";
            break;
        case 5:
            retstring = "A free block is unlisted";
            break;
        case 6:
            retstring = "The free list is missing a free block";
            break;
        default:
            retstring = "";
    }
    return retstring;
}

