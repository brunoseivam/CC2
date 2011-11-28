/*
  dadas duas variáveis, uma inteira e outra literal, escrever os
endereços de memória na qual estão armazenadas
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float valorReal;
	char valorLiteral[STRSIZE];
	float *enderecoReal;
	char *enderecoLiteral;

	/* armazenamento dos enderecos */
	enderecoReal = &valorReal;
	enderecoLiteral = valorLiteral;

	/* escrita do resultado */
	printf("%s", "A variavel real esta no endereco: ");
	printf("%X", (unsigned int)enderecoReal);
	printf("\n");
	printf("%s", "A variavel literal esta no endereco: ");
	printf("%X", (unsigned int)enderecoLiteral);
	printf("\n");

	return 0;
}
