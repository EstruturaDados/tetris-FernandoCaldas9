#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5 // Capacidade máxima da fila (número fixo de peças futuras)

// ============================
// ESTRUTURAS DE DADOS
// ============================

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único da peça
} Peca;

// Estrutura que representa a fila circular
typedef struct {
    Peca pecas[MAX];
    int inicio;  // Índice do primeiro elemento
    int fim;     // Índice do último elemento
    int tamanho; // Número de elementos na fila
} Fila;

// ============================
// VARIÁVEIS GLOBAIS
// ============================
int contadorID = 0; // Controle incremental do ID das peças

// ============================
// FUNÇÕES DE FILA
// ============================

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Gera uma nova peça aleatória
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // Escolhe aleatoriamente entre I, O, T, L
    nova.id = contadorID++;        // Gera ID incremental
    return nova;
}

// Insere uma peça no final da fila (enqueue)
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) {
        printf("\nA fila de pecas futuras esta cheia!\n");
        return;
    }
    f->fim = (f->fim + 1) % MAX; // Movimento circular
    f->pecas[f->fim] = nova;
    f->tamanho++;
    printf("\nNova peca [%c %d] adicionada com sucesso!\n", nova.nome, nova.id);
}

// Remove uma peça do início da fila (dequeue)
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\nNao ha pecas para jogar! A fila esta vazia.\n");
        return;
    }
    Peca removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX; // Movimento circular
    f->tamanho--;
    printf("\nPeca jogada: [%c %d]\n", removida.nome, removida.id);
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("\nFila de pecas vazia!\n");
        return;
    }

    printf("\n--- Fila de Pecas Futuras ---\n");
    int i = f->inicio;
    for (int cont = 0; cont < f->tamanho; cont++) {
        printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
        i = (i + 1) % MAX;
    }
    printf("\n");
}

// ============================
// FUNÇÃO PRINCIPAL
// ============================

int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < MAX; i++) {
        Peca nova = gerarPeca();
        enfileirar(&fila, nova);
    }

    int opcao;

    do {
        printf("\n===== TETRIS STACK - FILA DE PECAS =====\n");
        exibirFila(&fila);
        printf("\nOpcoes:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2: {
                Peca nova = gerarPeca();
                enfileirar(&fila, nova);
                break;
            }
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

