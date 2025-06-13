#include <stdio.h>
#include <stdlib.h>
//
typedef struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

int height(Node* n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

Node* insert(Node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; 

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)         // LL
        return rotateRight(node);

    if (balance < -1 && key > node->right->key)       // RR
        return rotateLeft(node);

    if (balance > 1 && key > node->left->key) {       // LR
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && key < node->right->key) {     // RL
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void printTree(Node* root, int space) {
    if (!root) return;
    space += 5;
    printTree(root->right, space);
    for (int i = 5; i < space; i++) printf(" ");
    printf("%d(h=%d,b=%d)\n", root->key, root->height, getBalance(root));
    printTree(root->left, space);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    int seq[] = {40, 20, 10, 25, 30, 50, 60, 70, 65};
    int n = sizeof(seq) / sizeof(seq[0]);
    Node* root = NULL;

    for (int i = 0; i < n; i++) {
        printf("---------------------\n");
        printf("Inserindo %d:\n", seq[i]);
        root = insert(root, seq[i]);
        printTree(root, 0);
    }

    freeTree(root);
    return 0;
}
