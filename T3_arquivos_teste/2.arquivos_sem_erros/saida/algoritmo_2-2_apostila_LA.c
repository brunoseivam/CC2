/*
  Leitura de nome e idade com escrita de mensagem usando estes dados
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	char nome[80]; // tamanho arbitrario
	int idade;

	/* leitura de nome e idade do teclado */
	printf("Nome: ");
	gets(nome);  // para literais
	printf("Idade: ");
	scanf("%d", &idade); limpa_entrada();

	/* saida da mensagem na tela */
	printf("%s tem %d anos.", nome, idade);

	return 0;
}
