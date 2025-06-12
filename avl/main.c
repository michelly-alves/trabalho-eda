#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó AVL
typedef struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
} Node;

// Função para obter a altura
int height(Node* n) {
    return n ? n->height : 0;
}

// Função para obter o maior entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Cria um novo nó
Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// Rotação à direita (LL)
Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Rotação à esquerda (RR)
Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Fator de balanceamento
int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

// Inserção com balanceamento AVL
Node* insert(Node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // chaves iguais não são permitidas

    // Atualiza altura
    node->height = 1 + max(height(node->left), height(node->right));

    // Verifica balanceamento
    int balance = getBalance(node);

    // Casos de rotação
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

// Impressão da árvore com altura e balanceamento
void printTree(Node* root, int space) {
    if (!root) return;
    space += 5;
    printTree(root->right, space);
    for (int i = 5; i < space; i++) printf(" ");
    printf("%d(h=%d,b=%d)\n", root->key, root->height, getBalance(root));
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
