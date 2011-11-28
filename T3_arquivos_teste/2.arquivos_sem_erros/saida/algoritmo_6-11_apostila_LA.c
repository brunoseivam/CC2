/*
  dado um numero inteiro, determinar se eh um numero perfeito, ou seja,
  se eh igual ah soma de seus divisores, exceto ele mesmo
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int i, valor, soma;

	/* leitura do numero */
	printf("%s", "Digite um valor inteiro positivo: ");
	printf("\n");
	scanf("%d", &valor); limpa_entrada();

	/* soma dos divisores do valor */
	soma = 0;
	for(i = 1; i <= valor/2; i++){
		if(valor % i == 0){
			soma = soma + i;
		}
	}

	/* resultado */
	if(soma == valor && valor != 0){
		printf("%d", valor);
		printf("%s", " eh um numero perfeito");
		printf("\n");
	}
	else{
		printf("%d", valor);
		printf("%s", " nao eh um numero perfeito");
		printf("\n");
	}

	return 0;
}
