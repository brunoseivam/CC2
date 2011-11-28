/*
  dada uma variável, modificar seu conteúdo usando apenas seu endereço
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int valor; /* inteiro comum */
	int *ponteiro; /* ponteiro para inteiro */

	/* atribuicao de um valor inicial */
	valor = 100;

	/* determinacao do endereco da variavel */
	ponteiro = &valor;

	/* uso do ponteiro para mexer na memoria */
	*ponteiro = 200;

	/* verificacao do valor armazenado */
	printf("%d", valor); /* 200 */
	printf("\n");

	return 0;
}
