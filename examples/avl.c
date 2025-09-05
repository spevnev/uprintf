#include "avl.h"  // https://github.com/etherealvisage/avl
#include <stdint.h>
#include <stdlib.h>

#define UPRINTF_INDENTATION_WIDTH 2
#define UPRINTF_MAX_DEPTH 5
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

#define SIZE 10000

int cmp(void *k1, void *k2) {
    uint64_t ku1 = (uint64_t) k1;
    uint64_t ku2 = (uint64_t) k2;
    if (ku1 == ku2) return 0;
    else if (ku1 < ku2) return -1;
    return 1;
}

int main(void) {
    avl_tree_t tree;
    avl_initialize(&tree, cmp, NULL);

    for (int i = 0; i < SIZE; i++) {
        avl_insert(&tree, (void *) (long) i, (void *) (long) i);
    }

    uprintf("AVL tree: %S\n", &tree);

    for (int i = 0; i < SIZE; i++) {
        avl_remove(&tree, (void *) (long) i);
    }

    return EXIT_SUCCESS;
}
