#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct Node {
    int key;
    int color;
    struct Node *left, *right, *parent;
} Node;

// Criação de novo nó (inicia como vermelho)
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

// Rotação à esquerda
Node* rotateLeft(Node* root, Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
    return root;
}

// Rotação à direita
Node* rotateRight(Node* root, Node* y) {
    Node* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
    return root;
}

// Corrige a árvore após inserção
Node* fixInsert(Node* root, Node* z) {
    while (z->parent && z->parent->color == RED) {
        Node* gp = z->parent->parent;
        if (z->parent == gp->left) {
            Node* uncle = gp->right;
            if (uncle && uncle->color == RED) {
                // Caso 1: tio vermelho
                z->parent->color = BLACK;
                uncle->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->parent->right) {
                    // Caso 2: rotação esquerda
                    z = z->parent;
                    root = rotateLeft(root, z);
                }
                // Caso 3: rotação direita
                z->parent->color = BLACK;
                gp->color = RED;
                root = rotateRight(root, gp);
            }
        } else {
            Node* uncle = gp->left;
            if (uncle && uncle->color == RED) {
                // Caso 1 espelhado
                z->parent->color = BLACK;
                uncle->color = BLACK;
                gp->color = RED;
                z = gp;
            } else {
                if (z == z->parent->left) {
                    // Caso 2 espelhado
                    z = z->parent;
                    root = rotateRight(root, z);
                }
                // Caso 3 espelhado
                z->parent->color = BLACK;
                gp->color = RED;
                root = rotateLeft(root, gp);
            }
        }
    }
    root->color = BLACK;
    return root;
}

// Inserção padrão de BST com correção Rubro-Negra
Node* insert(Node* root, int key) {
    Node* z = newNode(key);
    Node* y = NULL;
    Node* x = root;

    // Inserção tipo BST
    while (x != NULL) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else if (z->key > x->key)
            x = x->right;
        else
            return root; // duplicado
    }

    z->parent = y;
    if (!y)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    // Corrige árvore Rubro-Negra
    return fixInsert(root, z);
}

// Impressão da árvore (pré-ordem, hierárquica)
void printTree(Node* root, int space) {
    if (!root) return;
    space += 5;
    printTree(root->right, space);
    for (int i = 5; i < space; i++) printf(" ");
    printf("%d(%s)\n", root->key, root->color == RED ? "R" : "B");
    printTree(root->left, space);
}

// Libera memória
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// Função principal
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
