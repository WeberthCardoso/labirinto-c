#include <stdio.h>
#include <locale.h>

int main(void){
	setlocale(LC_ALL,"Portuguese");
	
	FILE *arquivo;
	
	arquivo = fopen("labirinto.txt", "r");
	
	if (arquivo == NULL){
		printf("Não conseguir abrir o labirinto.\n");
		
		return 1;
	}
	printf("Labirinto aberto com sucesso!\n");
	
	fclose(arquivo);
	
	return 0;
}
