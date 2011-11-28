/*
  Dados, separadamente, prenome e sobrenome, escreve-los em dois
  formatos: "Fulano Tal" e "Tal, F."
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	char prenome[80], sobrenome[80], formato1[80], formato2[80];

	/* obtencao dos nomes */
	printf("Primeiro nome: ");
	gets(prenome);
	printf("Sobrenome: ");
	gets(sobrenome);

	/* composicao dos nomes */
	char strtmp1[80];
	strtmp1[0] = '\0';
	strcat(strtmp1, prenome);
	strcat(strtmp1, " ");
	strcat(strtmp1, sobrenome);
	strcpy(formato1, strtmp1);

	char strtmp2[80];
	strtmp2[0] = '\0';
	strcat(strtmp2, sobrenome);
	strcat(strtmp2, ", ");
	strcat(strtmp2, subLiteral(prenome, 1, 1));
	strcat(strtmp2, ".");
	strcpy(formato2, strtmp2);

	/* resultados */
	printf("%s\n", formato1);
	printf("%s\n", formato2);

	return 0;
}
