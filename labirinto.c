#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAX_LINHAS 10
#define MAX_COLUNAS 30

typedef struct {
	int linha;
	int coluna;
} Posicao;
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
	
	printf("\nAltura: %d\n", altura);
	printf("\nLargura: %d\n", largura);
	
	printf("Entrada (S) encontrada na linha %d, coluna %d\n", inicio.linha, inicio.coluna);
	printf("Saída (E) encontrada na linha %d, coluna %d\n", fim.linha, fim.coluna);
	
	return 0;
}
