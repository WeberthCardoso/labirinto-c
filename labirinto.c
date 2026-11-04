#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINHAS 10
#define MAX_COLUNAS 30
#define MAX_PILHA 300

typedef struct {
	int linha;
	int coluna;
} Posicao;

typedef struct {
	Posicao dados[MAX_PILHA];
	int topo;
}Pilha;

void inicializarPilha(Pilha *p){
	p->topo = -1;
}

bool pilhaVazia(Pilha *p){
	return p->topo == -1;
}

bool empilhar(Pilha *p, Posicao pos){
	if(p->topo < MAX_PILHA -1){
		 p->topo++;
        p->dados[p->topo] = pos;
        return true;
	}
	return false;
}

Posicao desempilhar(Pilha *p){
	Posicao pos = {-1, -1};
	if(!pilhaVazia(p)){ 
		pos = p->dados[p->topo];
		p->topo--;
	}
	return pos;
}

int main(void){
	setlocale(LC_ALL,"Portuguese");
	
	char mapa[MAX_LINHAS][MAX_COLUNAS];
	
	int altura = 0;
	int largura = 0;
	
	FILE *arquivo;
	
	arquivo = fopen("labirinto.txt", "r");
	
	if (arquivo == NULL){
		printf("Não consegui abrir o labirinto.\n");
		
		return 1;
	}
	while (altura < MAX_LINHAS && fgets(mapa[altura], sizeof(mapa[altura]), arquivo)!= NULL){
		
		mapa[altura][strcspn(mapa[altura],"\n")]= '\0';
		
		altura++;
	}
	fclose(arquivo);
	
	if (altura > 0){
		largura = strlen(mapa[0]);
	}
	
	for(int i = 0; i < altura; i++){
		printf("%s\n", mapa[i]);
	}
	
	Posicao inicio = {-1, -1};
	Posicao fim = {-1, -1};
	
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < largura ; j++){
			
			if(mapa[i][j] == 'S'){
				inicio.linha = i;
				inicio.coluna = j;
			}
			
			if(mapa[i][j] == 'E'){
				fim.linha = i;
				fim.coluna = j;
			}
		}
	}
	
	if (fim.linha == -1 || inicio.linha == -1) {
		printf("\nErro: Labirinto inválido! Entrada (S) ou Saída (E) não encontrada.\n");
		return 1; 
	}
	
	printf("\nAltura: %d | Largura: %d\n", altura, largura);
    printf("Entrada (S) na linha %d, coluna %d\n", inicio.linha, inicio.coluna);
    printf("Saída (E) na linha %d, coluna %d\n\n", fim.linha, fim.coluna);
	
	
	 Pilha caminho;
    inicializarPilha(&caminho);
    
    
    bool visitado[MAX_LINHAS][MAX_COLUNAS] = {false};
    
    
    Posicao atual = inicio;
    empilhar(&caminho, atual);
    visitado[atual.linha][atual.coluna] = true;
    
    bool encontrouSaida = false;

    
    int df[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!pilhaVazia(&caminho)) {
        atual = caminho.dados[caminho.topo]; 

        
        if (atual.linha == fim.linha && atual.coluna == fim.coluna) {
            encontrouSaida = true;
            break;
        }

        bool moveu = false;

        
        for (int i = 0; i < 4; i++) {
            int novaLinha = atual.linha + df[i];
            int novaColuna = atual.coluna + dc[i];

            
            if (novaLinha >= 0 && novaLinha < altura && novaColuna >= 0 && novaColuna < largura) {
                
                if ((mapa[novaLinha][novaColuna] == ' ' || mapa[novaLinha][novaColuna] == 'E') && !visitado[novaLinha][novaColuna]) {
                    
                    Posicao proxima = {novaLinha, novaColuna};
                    visitado[novaLinha][novaColuna] = true;
                    empilhar(&caminho, proxima);
                    
                    
                    if (mapa[novaLinha][novaColuna] != 'E') {
                        mapa[novaLinha][novaColuna] = '.'; 
                    }
                    
                    moveu = true;
                    break; 
                }
            }
        }

        
        if (!moveu) {
            Posicao removida = desempilhar(&caminho);
            
            if (mapa[removida.linha][removida.coluna] == '.') {
                mapa[removida.linha][removida.coluna] = ' '; 
            }
        }
    }

    
    if (encontrouSaida) {
        printf("--- Labirinto Resolvido! ---\n");
        for (int i = 0; i < altura; i++) {
            printf("%s\n", mapa[i]);
        }
    } else {
        printf("Não existe um caminho possível para a saída.\n");
    }
	    
	
	return 0;
}
