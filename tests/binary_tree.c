#include <stdlib.h>
#include "uprintf.h"

#define DEPTH 3

typedef struct _Node {
    struct _Node *left;
    struct _Node *right;
    int value;
} Node;

void populate_node(Node *node, int value, int depth) {
    node->value = value;
    if (depth >= DEPTH) {
        node->left = NULL;
        node->right = NULL;
        return;
    }

    node->left = malloc(sizeof(Node));
    populate_node(node->left, value + 1, depth + 1);

    node->right = malloc(sizeof(Node));
    populate_node(node->right, value * 2, depth + 1);
}

void free_node(Node *node) {
    if (node == NULL) return;
    free_node(node->left);
    free_node(node->right);
    free(node);
}

int main(void) {
    Node *root = malloc(sizeof(*root));
    populate_node(root, 3, 1);

    uprintf("Binary tree: %S\n", root);

    free_node(root);
    return _upf_test_status;
}
