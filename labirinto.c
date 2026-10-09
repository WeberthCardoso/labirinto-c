#include <stdio.h>
#include <locale.h>
#include <string.h>

#define MAX_LINHAS 10
#define MAX_COLUNAS 30

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
	
	int linhaS = -1, colunaS = -1;
	int linhaE = -1, colunaE = -1;
	
	for(int i = 0; i < altura; i++){
		for(int j = 0; j < largura ; j++){
			
			if(mapa[i][j] == 'S'){
				linhaS = i;
				colunaS = j;
			}
			
			if(mapa[i][j] == 'E'){
				linhaE = i;
				colunaE = j;
			}
		}
	}
	
	if (linhaE == -1 || linhaS == -1) {
		printf("\nErro: Labirinto inválido! Entrada (S) ou Saída (E) não encontrada.\n");
		return 1; 
	}
	
	printf("\nAltura: %d\n", altura);
	printf("\nLargura: %d\n", largura);
	
	printf("Entrada (S) encontrada na linha %d, coluna %d\n", linhaS, colunaS);
	printf("Saída (E) encontrada na linha %d, coluna %d\n", linhaE, colunaE);
	
	return 0;
}
