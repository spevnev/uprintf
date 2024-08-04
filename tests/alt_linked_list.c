#include <stdlib.h>
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

struct NodeB;

typedef struct NodeA {
    int value;
    struct NodeB *next;
} NodeA;

typedef struct NodeB {
    float value;
    struct NodeA *next;
} NodeB;

int main(void) {
    NodeA head = {
        .value = 0,
        .next = NULL,
    };

    void *_cur = &head;
    for (int i = 1; i <= 5; i++) {
        if (i % 2 == 0) {
            NodeB *cur = (NodeB *) _cur;
            NodeA *node = (NodeA *) malloc(sizeof(*node));
            if (node == NULL) return 1;
            node->value = i * 2;
            node->next = NULL;

            cur->next = node;
            _cur = node;
        } else {
            NodeA *cur = (NodeA *) _cur;
            NodeB *node = (NodeB *) malloc(sizeof(*node));
            if (node == NULL) return 1;
            node->value = i * -1.23;
            node->next = NULL;

            cur->next = node;
            _cur = node;
        }
    }

    uprintf("Alternating linked list: %S\n", &head);

    _cur = head.next;
    while (_cur != NULL) {
        NodeA *next = ((NodeB *) _cur)->next;
        free(_cur);
        _cur = next;
    }

    return _upf_test_status;
}
