#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Capacidade da fila de peças futuras
#define MAX_PILHA 3  // Capacidade da pilha de reserva

// ============================
// ESTRUTURAS
// ============================

// Struct que representa uma peça do jogo
typedef struct {
    char nome;  // Tipo da peça ('I','O','T','L')
    int id;     // Identificador único da peça
} Peca;

// Fila circular de peças futuras
typedef struct {
    Peca pecas[MAX_FILA];  // Array de peças
    int inicio;            // Índice da frente da fila
    int fim;               // Índice do final da fila
    int tamanho;           // Quantidade de elementos na fila
} Fila;

// Pilha de peças de reserva
typedef struct {
    Peca pecas[MAX_PILHA]; // Array de peças
    int topo;              // Índice do topo da pilha
} Pilha;

// ============================
// VARIÁVEIS GLOBAIS
// ============================

int contadorID = 0; // Contador global para gerar IDs únicos das peças

// ============================
// FUNÇÕES AUXILIARES
// ============================

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->tamanho = 0;
}

// Inicializa a pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) { return f->tamanho == MAX_FILA; }

// Verifica se a fila está vazia
int filaVazia(Fila *f) { return f->tamanho == 0; }

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA -1; }

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) { return p->topo == -1; }

// Gera uma peça aleatória ('I','O','T','L') com ID único
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I','O','T','L'};
    nova.nome = tipos[rand()%4];  // Escolhe tipo aleatório
    nova.id = contadorID++;       // Atribui ID único
    return nova;
}

// ============================
// FUNÇÕES DE FILA
// ============================

// Enfileira uma peça no final da fila circular
void enfileirar(Fila *f, Peca nova) {
    if(filaCheia(f)) return;  // Não adiciona se fila cheia
    f->fim = (f->fim +1)%MAX_FILA;  // Atualiza índice circular do fim
    f->pecas[f->fim] = nova;        // Insere peça
    f->tamanho++;
}

// Desenfileira peça da frente da fila
Peca desenfileirar(Fila *f) {
    Peca removida = {'?',-1};  // Valor padrão caso fila esteja vazia
    if(filaVazia(f)) return removida;
    removida = f->pecas[f->inicio];         // Remove peça da frente
    f->inicio = (f->inicio +1)%MAX_FILA;    // Atualiza índice circular
    f->tamanho--;
    return removida;
}

// ============================
// FUNÇÕES DE PILHA
// ============================

// Empilha peça no topo da pilha
void empilhar(Pilha *p, Peca nova) {
    if(pilhaCheia(p)) {
        printf("\nPilha cheia! Nao e possivel reservar.\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = nova; // Insere no topo
    printf("\nPeca [%c %d] reservada!\n", nova.nome, nova.id);
}

// Desempilha peça do topo da pilha
Peca desempilhar(Pilha *p) {
    Peca removida = {'?',-1};
    if(pilhaVazia(p)) {
        printf("\nPilha vazia! Nao ha peças para usar.\n");
        return removida;
    }
    removida = p->pecas[p->topo];
    p->topo--;
    printf("\nPeca [%c %d] usada da pilha.\n", removida.nome, removida.id);
    return removida;
}

// ============================
// FUNÇÕES DE TROCA
// ============================

// Troca a peça da frente da fila com o topo da pilha
void trocarFrentePilha(Fila *f, Pilha *p) {
    if(filaVazia(f) || pilhaVazia(p)) {
        printf("\nNao e possivel realizar a troca.\n");
        return;
    }
    Peca temp = f->pecas[f->inicio];
    f->pecas[f->inicio] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    printf("\nTroca realizada entre frente da fila e topo da pilha.\n");
}

// Troca múltipla: 3 primeiros da fila com 3 da pilha
void trocaTripla(Fila *f, Pilha *p) {
    if(f->tamanho<3 || p->topo<2) {
        printf("\nNao ha elementos suficientes para a troca tripla.\n");
        return;
    }
    for(int i=0;i<3;i++){
        int idxFila = (f->inicio + i)%MAX_FILA;
        Peca temp = f->pecas[idxFila];
        f->pecas[idxFila] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }
    printf("\nTroca realizada entre os 3 primeiros da fila e 3 da pilha.\n");
}

// ============================
// FUNÇÃO DE EXIBIÇÃO
// ============================

// Exibe estado atual da fila e da pilha
void exibirEstado(Fila *f, Pilha *p){
    printf("\n============================================\n");
    printf("ESTADO ATUAL DO JOGO\n");
    printf("--------------------------------------------\n");

    // Fila
    printf("Fila de peças futuras (%d/%d): ", f->tamanho, MAX_FILA);
    if(filaVazia(f)) printf("[vazia]");
    else{
        int i = f->inicio;
        for(int cont=0; cont<f->tamanho; cont++){
            printf("[%c %d] ", f->pecas[i].nome, f->pecas[i].id);
            i = (i+1)%MAX_FILA;
        }
    }
    printf("\n");

    // Pilha
    printf("Pilha de reserva (%d/%d) (Topo -> Base): ", p->topo+1, MAX_PILHA);
    if(pilhaVazia(p)) printf("[vazia]");
    else{
        for(int i=p->topo;i>=0;i--){
            printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
        }
    }
    printf("\n============================================\n");
}

// ============================
// FUNÇÃO PRINCIPAL
// ============================

int main(){
    srand(time(NULL));  // Inicializa gerador de números aleatórios

    Fila fila;
    Pilha pilha;

    // Inicializa estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial com peças aleatórias
    for(int i=0;i<MAX_FILA;i++){
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do{
        exibirEstado(&fila, &pilha);

        // Menu de ações
        printf("\nOpcoes:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca múltipla: 3 primeiros da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1: {
                // Jogar peça da frente da fila
                if(!filaVazia(&fila)){
                    Peca jogada = desenfileirar(&fila);
                    printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca()); // Mantém fila cheia
                }else{
                    printf("\nFila vazia!\n");
                }
                break;
            }
            case 2: {
                // Enviar peça da fila para a pilha
                if(!filaVazia(&fila) && !pilhaCheia(&pilha)){
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    enfileirar(&fila, gerarPeca()); // Mantém fila cheia
                }else if(pilhaCheia(&pilha)){
                    printf("\nPilha cheia!\n");
                }else{
                    printf("\nFila vazia!\n");
                }
                break;
            }
            case 3:
                // Usar peça da pilha
                desempilhar(&pilha);
                break;
            case 4:
                // Troca frente da fila com topo da pilha
                trocarFrentePilha(&fila,&pilha);
                break;
            case 5:
                // Troca múltipla
                trocaTripla(&fila,&pilha);
                break;
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }

    }while(opcao!=0);

    return 0;
}

