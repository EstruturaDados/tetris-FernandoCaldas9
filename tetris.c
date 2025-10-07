#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Capacidade máxima da fila circular (peças futuras)
#define MAX_PILHA 3  // Capacidade máxima da pilha de reserva

// =========================================
// ESTRUTURAS DE DADOS
// =========================================

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único incremental
} Peca;

// Estrutura de Fila Circular
typedef struct {
    Peca pecas[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// Estrutura de Pilha Linear
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

// =========================================
// VARIÁVEIS GLOBAIS
// =========================================
int contadorID = 0; // Gera IDs únicos para cada peça

// =========================================
// FUNÇÕES DE FILA
// =========================================

// Inicializa a fila circular
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->tamanho == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

// Gera uma nova peça aleatória
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];  // Escolhe tipo aleatório
    nova.id = contadorID++;         // Gera ID incremental
    return nova;
}

// Adiciona uma peça ao final da fila
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->pecas[f->fim] = nova;
    f->tamanho++;
}

// Remove a peça do início da fila
Peca desenfileirar(Fila *f) {
    Peca removida = {'?', -1};
    if (filaVazia(f)) return removida;
    removida = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
    return removida;
}

// =========================================
// FUNÇÕES DE PILHA
// =========================================

// Inicializa a pilha de reserva
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha uma peça no topo da pilha
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("\nA pilha de reserva esta cheia!\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = nova;
    printf("\nPeca [%c %d] reservada com sucesso!\n", nova.nome, nova.id);
}

// Desempilha a peça do topo da pilha
Peca desempilhar(Pilha *p) {
    Peca removida = {'?', -1};
    if (pilhaVazia(p)) {
        printf("\nNao ha pecas reservadas para usar.\n");
        return removida;
    }
    removida = p->pecas[p->topo];
    p->topo--;
    printf("\nPeca [%c %d] usada da reserva.\n", removida.nome, removida.id);
    return removida;
}

// =========================================
// EXIBIÇÃO DO ESTADO ATUAL
// =========================================
void exibirEstado(Fila *f, Pilha *p) {
    printf("\n============================================\n");
    printf("ESTADO ATUAL DO JOGO\n");
    printf("--------------------------------------------\n");

    // Exibe fila
    printf("Fila de pecas futuras (%d/%d): ", f->tamanho, MAX_FILA);
    if (filaVazia(f)) {
        printf("[vazia]");
    } else {
        int i = f->inicio;
        for (int cont = 0; cont < f->tamanho; cont++) {
            printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
            i = (i + 1) % MAX_FILA;
        }
    }
    printf("\n");

    // Exibe pilha
    printf("Pilha de reserva (%d/%d) (Topo -> Base): ", p->topo + 1, MAX_PILHA);
    if (pilhaVazia(p)) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }

    printf("\n============================================\n");
}

// =========================================
// FUNÇÃO PRINCIPAL
// =========================================
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (remove da fila)\n");
        printf("2 - Reservar peca (move fila -> pilha)\n");
        printf("3 - Usar peca reservada (remove pilha)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Jogar uma peça da fila
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
                    // Gera nova peça para o fim da fila
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("\nFila vazia!\n");
                }
                break;
            }

            case 2: {
                // Reservar a peça da frente da fila
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    enfileirar(&fila, gerarPeca());
                } else if (pilhaCheia(&pilha)) {
                    printf("\nNao e possivel reservar: pilha cheia.\n");
                } else {
                    printf("\nFila vazia!\n");
                }
                break;
            }

            case 3:
                // Usar a peça reservada
                desempilhar(&pilha);
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

