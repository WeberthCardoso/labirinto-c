#include <stdio.h>
#include <locale.h>
#include <string.h>

#define max_linhas 10
#define max_colunas 30

int main(void){
	setlocale(LC_ALL,"Portuguese");
	
	char mapa[max_linhas][max_colunas];
	
	int altura = 0;
	int largura = 0;
	
	FILE *arquivo;
	
	arquivo = fopen("labirinto.txt", "r");
	
	if (arquivo == NULL){
		printf("Não consegui abrir o labirinto.\n");
		
		return 1;
	}
	while (altura < max_linhas && fgets(mapa[altura], sizeof(mapa[altura]), arquivo)!= NULL){
		
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
	printf("\nAltura: %d\n", altura);
	printf("\nLargura: %d\n", largura);
	
	return 0;
}
