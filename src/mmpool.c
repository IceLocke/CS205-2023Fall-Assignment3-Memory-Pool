#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct mmpool mmpool_t;
typedef struct mmpool_data mmpool_data_t;
typedef struct mmpool_large mmpool_large_t;

#define MMPOOL_MAX_FAILED 4
struct mmpool_data {
    uint8_t        *last;
    uint8_t        *end;
    mmpool_t       *next;
    size_t         failed;
};

struct mmpool_large {
    mmpool_large_t    *next;
    void              *alloc;
};

#define MMPOOL_MAX_ALLOC (0x1000 - sizeof(mmpool_t))
#define MMPOOL_SIZE      (0x1000 - sizeof(mmpool_t))
struct mmpool {
    mmpool_data_t   data;
    mmpool_t        *current;
    mmpool_large_t  *large;
};

/**
 * @brief Create a mmpool_t entry.
 * 
 * @return mmpool_t* the pointer of the memory pool.
 */
mmpool_t* mmpool_create();

/**
 * @brief Allocate a memory block from a memory pool.
 * 
 * @param pool the pointer of the memory pool.
 * @param size size of the memory block (in bytes).
 * 
 * @return void* pointer of the start address of the memory block.
 */
void* mmpool_alloc(mmpool_t *pool, size_t size);

/**
 * @brief Allocate a large memory block by `malloc()`.
 * This function must be called by `mmpool_alloc()`. Do not manually call it.
 * 
 * @param size size of the memory block (in bytes).
 * 
 * @return mmpool_large_t* pointer of the start address of the memory block.
 */
mmpool_large_t* mmpool_alloc_large(size_t size);

/**
 * @brief Free a large memory block from a memory pool.
 * 
 * @param pool the pointer of the memory pool.
 * @param p `*alloc` of the large memory block.
 */
void mmpool_free_large(mmpool_t *pool, void *p);

/**
 * @brief Destroy a memory pool. Free all related memory.
 * 
 * @param pool the pointer of the memory pool.
 */
void mmpool_destroy(mmpool_t *pool);

/**
 * @brief Reset a memory pool i.e. 
 * reset `*current` to the pool itself, `*last` and `failed`;
 * free all large memory blocks.
 * 
 * @param pool the pointer of the memory pool
 */
void mmpool_reset(mmpool_t *pool);

/* Functions for tests */
/* ==BEGIN== */
void prt_mmpool_details(mmpool_t *pool) {
    if (pool == NULL) return;
    puts("===MEMORY POOL DETAILS===");
    printf("POOL ADDRESS: %p\n", pool);
    printf("POOL SIZE (FOR EACH): %lu\n", MMPOOL_SIZE);
    printf("MAX ALLOCATION SIZE: %lu\n", MMPOOL_MAX_ALLOC);
    printf("MAX FAILED TIMES: %d\n", MMPOOL_MAX_FAILED);
    printf("CURRENT ACTIVE POOL ADDRESS: %p\n", pool->current);
    puts("\nSUB POOL DETAILS:");

    size_t count = 0;
    mmpool_t *cur = pool;
    while (cur != NULL) {
        printf("\t-> Sub pool #%lu, address=%p, free_space=%lu (bytes), failed=%lu\n",
                ++count, cur, (size_t)(cur->data.end - cur->data.last), cur->data.failed);
        cur = cur->data.next;
    }
    printf("SUB POOL COUNT: %lu\n", count);

    puts("\nLARGE BLOCK DETAILS: ");
    count = 0;
    mmpool_large_t *large = pool->large;
    while (large != NULL) {
        printf("\t-> Large block #%lu, address=%p\n", ++count, large);
        large = large->next;
    }
    printf("LARGE BLOCK COUNT: %lu\n", count);
    puts("===MEMORY POOL DETAILS END===\n");
}
/* ==END== */

mmpool_t* mmpool_create() {
    mmpool_t* p = NULL;

    p = (mmpool_t*) malloc(sizeof(mmpool_t) + MMPOOL_SIZE);
    if (p != NULL) {
        p->data.last = (uint8_t*) p + sizeof(mmpool_t);
        p->data.end = p->data.last + MMPOOL_SIZE;
        p->data.next = NULL;
        p->data.failed = 0;

        p->current = p;
        p->large = NULL;
    }

    return p;
}

/* WRITE YOUR CODE HERE */
/* ==BEGIN== */
void* mmpool_alloc(mmpool_t *pool, size_t size) {

}

mmpool_large_t* mmpool_alloc_large(size_t size) {

}

void mmpool_free_large(mmpool_t *pool, void *p) {

}

void mmpool_destroy(mmpool_t *pool) {

}

void mmpool_reset(mmpool_t *pool) {

}
/* ==END== */

#define MAX_POINTERS 100000

int main() {
    int Q, q, op;
    size_t size;

    /*
        op: operator
        1 - Create a memory pool
        2 - Allocate memory from memory pool
        3 - Free the latest allocated large memory
        4 - Reset the memory pool
        5 - Destroy a memory pool
    */

    mmpool_t *pool;
    void *small[MAX_POINTERS], *big[MAX_POINTERS];
    size_t scnt = 0, bcnt = 0;

    scanf("%d", &Q);
    for (q = 0; q < Q; ++q) {
        scanf("%d", &op);   
        printf("[DEBUG] Query: %d, Operator: %d\n", q, op);

        if (op == 1)
            pool = mmpool_create();
        else if (op == 2) {
            scanf("%lu", &size);
            if (size <= MMPOOL_MAX_ALLOC)
                small[scnt++] = mmpool_alloc(pool, size);
            else big[bcnt++] = mmpool_alloc(pool, size);
        }
        else if (op == 3)
            mmpool_free_large(pool, big[--bcnt]);
        else if (op == 4)
            mmpool_reset(pool);
        else if (op == 5) {
            mmpool_destroy(pool);
            pool = NULL;
        }
        else puts("UNKNOWN OPERATOR!");

        prt_mmpool_details(pool);
    } 

    return 0;
}
