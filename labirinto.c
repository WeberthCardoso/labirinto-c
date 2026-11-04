#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINHAS 10
#define MAX_COLUNAS 30
#define MAX_ESTRUTURA 300 // Limite seguro para Pilha e Fila

typedef struct {
    int linha;
    int coluna;
} Posicao;

// Estrutura e funções de pilha
typedef struct {
    Posicao dados[MAX_ESTRUTURA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) { p->topo = -1; }
bool pilhaVazia(Pilha *p) { return p->topo == -1; }
bool empilhar(Pilha *p, Posicao pos) {
    if (p->topo < MAX_ESTRUTURA - 1) {
        p->topo++;
        p->dados[p->topo] = pos;
        return true;
    }
    return false;
}
Posicao desempilhar(Pilha *p) {
    Posicao pos = {-1, -1};
    if (!pilhaVazia(p)) {
        pos = p->dados[p->topo];
        p->topo--;
    }
    return pos;
}

// Estrutura e funções da fila 

typedef struct {
    Posicao dados[MAX_ESTRUTURA];
    int frente;
    int tras;
} Fila;

void inicializarFila(Fila *f) { f->frente = 0; f->tras = 0; }
bool filaVazia(Fila *f) { return f->frente == f->tras; }
bool enfileirar(Fila *f, Posicao pos) {
    if (f->tras < MAX_ESTRUTURA) {
        f->dados[f->tras] = pos;
        f->tras++;
        return true;
    }
    return false;
}
Posicao desenfileirar(Fila *f) {
    Posicao pos = {-1, -1};
    if (!filaVazia(f)) {
        pos = f->dados[f->frente];
        f->frente++;
    }
    return pos;
}

// Algoritmos e busca

void resolverDFS(char mapa[MAX_LINHAS][MAX_COLUNAS], int altura, int largura, Posicao inicio, Posicao fim) {
    Pilha pilha;
    inicializarPilha(&pilha);
    bool visitado[MAX_LINHAS][MAX_COLUNAS] = {false};
    
    empilhar(&pilha, inicio);
    visitado[inicio.linha][inicio.coluna] = true;
    bool encontrou = false;
    
    int df[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    while (!pilhaVazia(&pilha)) {
        Posicao atual = pilha.dados[pilha.topo];
        
        if (atual.linha == fim.linha && atual.coluna == fim.coluna) {
            encontrou = true;
            break;
        }
        
        bool moveu = false;
        for (int i = 0; i < 4; i++) {
            int nl = atual.linha + df[i];
            int nc = atual.coluna + dc[i];
            
            if (nl >= 0 && nl < altura && nc >= 0 && nc < largura) {
                if ((mapa[nl][nc] == ' ' || mapa[nl][nc] == 'E') && !visitado[nl][nc]) {
                    visitado[nl][nc] = true;
                    empilhar(&pilha, (Posicao){nl, nc});
                    if (mapa[nl][nc] != 'E') mapa[nl][nc] = '.';
                    moveu = true;
                    break;
                }
            }
        }
        if (!moveu) {
            Posicao removida = desempilhar(&pilha);
            if (mapa[removida.linha][removida.coluna] == '.') mapa[removida.linha][removida.coluna] = ' ';
        }
    }
    
    if (encontrou) {
        printf("\n--- Labirinto Resolvido com DFS (Pilha) ---\n");
        for (int i = 0; i < altura; i++) printf("%s\n", mapa[i]);
    } else {
        printf("\nDFS: Não encontrou saída.\n");
    }
}

void resolverBFS(char mapa[MAX_LINHAS][MAX_COLUNAS], int altura, int largura, Posicao inicio, Posicao fim) {
    Fila fila;
    inicializarFila(&fila);
    bool visitado[MAX_LINHAS][MAX_COLUNAS] = {false};
    Posicao pai[MAX_LINHAS][MAX_COLUNAS];
    
    for(int i=0; i<altura; i++)
        for(int j=0; j<largura; j++) pai[i][j] = (Posicao){-1, -1};
        
    enfileirar(&fila, inicio);
    visitado[inicio.linha][inicio.coluna] = true;
    bool encontrou = false;
    
    int df[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    while (!filaVazia(&fila)) {
        Posicao atual = desenfileirar(&fila);
        
        if (atual.linha == fim.linha && atual.coluna == fim.coluna) {
            encontrou = true;
            break;
        }
        
        for (int i = 0; i < 4; i++) {
            int nl = atual.linha + df[i];
            int nc = atual.coluna + dc[i];
            
            if (nl >= 0 && nl < altura && nc >= 0 && nc < largura) {
                if ((mapa[nl][nc] == ' ' || mapa[nl][nc] == 'E') && !visitado[nl][nc]) {
                    visitado[nl][nc] = true;
                    pai[nl][nc] = atual;
                    enfileirar(&fila, (Posicao){nl, nc});
                }
            }
        }
    }
    
    if (encontrou) {
        Posicao passo = pai[fim.linha][fim.coluna];
        while (passo.linha != inicio.linha || passo.coluna != inicio.coluna) {
            mapa[passo.linha][passo.coluna] = '.';
            passo = pai[passo.linha][passo.coluna];
        }
        printf("\n--- Labirinto Resolvido com BFS (Fila - Caminho Curto) ---\n");
        for (int i = 0; i < altura; i++) printf("%s\n", mapa[i]);
    } else {
        printf("\nBFS: Não encontrou saída.\n");
    }
}


int main(void) {
    setlocale(LC_ALL, "Portuguese");
    
    char mapaOriginal[MAX_LINHAS][MAX_COLUNAS];
    char mapaTrabalho[MAX_LINHAS][MAX_COLUNAS];
    int altura = 0, largura = 0;
    
    FILE *arquivo = fopen("labirinto.txt", "r");
    if (arquivo == NULL) {
        printf("Não consegui abrir o labirinto.\n");
        return 1;
    }
    while (altura < MAX_LINHAS && fgets(mapaOriginal[altura], sizeof(mapaOriginal[altura]), arquivo) != NULL) {
        mapaOriginal[altura][strcspn(mapaOriginal[altura], "\n")] = '\0';
        altura++;
    }
    fclose(arquivo);
    if (altura > 0) largura = strlen(mapaOriginal[0]);

    Posicao inicio = {-1, -1}, fim = {-1, -1};
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (mapaOriginal[i][j] == 'S') inicio = (Posicao){i, j};
            if (mapaOriginal[i][j] == 'E') fim = (Posicao){i, j};
        }
    }
    if (fim.linha == -1 || inicio.linha == -1) {
        printf("\nErro: Entrada (S) ou Saída (E) não encontrada.\n");
        return 1;
    }

    int opcao;
    do {
        printf("\n========== MENU LABIRINTO ==========\n");
        printf("1. Resolver com DFS (Pilha - Profundidade)\n");
        printf("2. Resolver com BFS (Fila - Menor Caminho)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1 || opcao == 2) {
            // Copia o mapa original para o mapa de trabalho para limpar alterações anteriores
            for (int i = 0; i < altura; i++) strcpy(mapaTrabalho[i], mapaOriginal[i]);

            if (opcao == 1) {
                resolverDFS(mapaTrabalho, altura, largura, inicio, fim);
            } else {
                resolverBFS(mapaTrabalho, altura, largura, inicio, fim);
            }
        }
    } while (opcao != 0);

    return 0;
}

