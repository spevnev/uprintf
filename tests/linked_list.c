#include <stdlib.h>
#include "uprintf.h"

#define NODES 5

typedef struct Node {
    int value;
    struct Node *next;
} Node;

int main(void) {
    Node head = {
        .value = 0,
        .next = NULL,
    };

    Node *cur = &head;
    for (int i = 1; i < NODES; i++) {
        Node *node = (Node *) malloc(sizeof(*node));
        if (node == NULL) return 1;
        node->value = i * 2;
        node->next = NULL;

        cur->next = node;
        cur = node;
    }

    uprintf("Linked list: %S\n", &head);

    cur = head.next;
    while (cur != NULL) {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }

    return _upf_test_status;
}
