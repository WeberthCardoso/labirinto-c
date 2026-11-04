#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h> // Para usar o Sleep() na animação

#define MAX_LINHAS 10
#define MAX_COLUNAS 30
#define MAX_ESTRUTURA 300 

typedef struct {
    int linha;
    int coluna;
} Posicao;

// =================================================================
// ESTRUTURA E FUNÇÕES DA PILHA (Para DFS)
// =================================================================
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

// =================================================================
// ESTRUTURA E FUNÇÕES DA FILA (Para BFS)
// =================================================================
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

// =================================================================
// FUNÇÃO AUXILIAR DE ANIMAÇÃO
// =================================================================
void desenharMapaAnimado(char mapa[MAX_LINHAS][MAX_COLUNAS], int altura) {
    system("cls"); // Limpa a tela do terminal
    printf("========== ANIMACAO DA BUSCA ==========\n\n");
    for (int i = 0; i < altura; i++) {
        printf("%s\n", mapa[i]);
    }
    Sleep(100); // Pausa de 100 milissegundos para gerar efeito visual
}

// =================================================================
// ALGORITMOS DE BUSCA ANIMADOS
// =================================================================

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
                    
                    desenharMapaAnimado(mapa, altura); 
                    
                    moveu = true;
                    break;
                }
            }
        }
        if (!moveu) {
            Posicao removida = desempilhar(&pilha);
            if (mapa[removida.linha][removida.coluna] == '.') {
                mapa[removida.linha][removida.coluna] = ' ';
                desenharMapaAnimado(mapa, altura); 
            }
        }
    }
    
    if (encontrou) {
        printf("\n--- Labirinto Resolvido com DFS (Pilha) ---\n");
    } else {
        printf("\nDFS: Não encontrou saída.\n");
    }
}

void resolverBFS(char mapa[MAX_LINHAS][MAX_COLUNAS], int altura, int largura, Posicao inicio, Posicao fim) {
    Fila fila;
    inicializarFila(&fila);
    bool visitado[MAX_LINHAS][MAX_COLUNAS] = {false};
    Posicao pai[MAX_LINHAS][MAX_COLUNAS];
    
    for(int i=0; i<altura; i++) {
        for(int j=0; j<largura; j++) {
            pai[i][j] = (Posicao){-1, -1};
        }
    }
        
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
                    
                    if (mapa[nl][nc] != 'E') {
                        mapa[nl][nc] = 'o'; 
                        desenharMapaAnimado(mapa, altura);
                    }
                }
            }
        }
    }
    
    if (encontrou) {
        for(int i=0; i<altura; i++) {
            for(int j=0; j<largura; j++) {
                if(mapa[i][j] == 'o') mapa[i][j] = ' ';
            }
        }
        
        Posicao passo = pai[fim.linha][fim.coluna];
        while (passo.linha != inicio.linha || passo.coluna != inicio.coluna) {
            mapa[passo.linha][passo.coluna] = '.';
            desenharMapaAnimado(mapa, altura);
            passo = pai[passo.linha][passo.coluna];
        }
        printf("\n--- Labirinto Resolvido com BFS (Fila - Caminho Curto) ---\n");
    } else {
        printf("\nBFS: Não encontrou saída.\n");
    }
}

// =================================================================
// FUNÇÃO PRINCIPAL
// =================================================================
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
        mapaOriginal[altura][strcspn(mapaOriginal[altura], "\r\n")] = '\0';
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
        printf("1. Resolver com DFS (Pilha - Profundidade Animado)\n");
        printf("2. Resolver com BFS (Fila - Menor Caminho Animado)\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            break;
        }

        if (opcao == 1 || opcao == 2) {
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

