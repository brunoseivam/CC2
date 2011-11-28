/*
  Programa que apresenta saudacoes para alguns nomes
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	char nome[80];

	/* primeiro nome */
	printf("Digite um nome (ou 'fim' para terminar): ");
	gets(nome);

	/* repeticao ate quando o nome for "fim" */
	while(strcmp(nome, "fim") != 0){
		/* saudacao */
		printf("Ola, %s, como vai?\n", nome);

		/* proximo nome */
		printf("Digite um nome (ou 'fim' para terminar): ");
		gets(nome);
	}

	/* despedida */
	printf("Nao ha mais ninguem? Entao, tchau!\n");

	return 0;
}
