/*
  Dados o nome, preco e tipo de vinhos, descrever o vinho mais caro,
  sem considerar precos iguais. O fim dos dados eh indicado por "fim"
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	char nome[80], nomeMaior[80], tipoVinho[80], tipoMaior[80];
	float preco, precoMaior;

	/* repeticao para leitura */
	precoMaior = -1; /* forca primeira troca */
	do{
		/* dados */
		printf("Nome: ");
		gets(nome); 
		printf("Preco: ");
		scanf("%f", &preco); limpa_entrada();
		printf("Tipo (R, T ou B): ");
		gets(tipoVinho); 

		/* verificacao do preco */
		if(preco > precoMaior && strcmp(nome, "fim") != 0){
			strcpy(nomeMaior, nome);
			precoMaior = preco;
			strcpy(tipoMaior, tipoVinho);
		}
	}while(!(strcmp(nome, "fim") == 0));

	/* resultado */
	if(precoMaior == -1){
		printf("Nenhum vinho foi apresentado.\n");
	}
	else{
		printf("%s, %f, %s\n", nomeMaior, precoMaior, tipoMaior);
	}

	return 0;
}
