/*
  calcular, para um conjunto de 20 valores inteiros, quantos destes
sao maiores ou iguais a media do conjunto
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int soma, contador, i, valor[20];
	float media;

	/* obtencao dos dados */
	soma = 0;
	for(i = 0; i <= 19; i++){
		printf("%s", "Digite um valor: ");
		scanf("%d", &valor[i]); limpa_entrada();
		soma = soma + valor[i];
	}
	media = soma/20.0;

	/* contagem de quantos valores sao maiores ou iguais a media */
	contador = 0;
	for(i = 0; i <= 19; i++){
		if(valor[i] >= media){
			contador = contador + 1;
		}
	}

	/* resultado */
	printf("%s", "Ha ");
	printf("%d", contador);
	printf("%s", " iten(s) maiore(s) que a media ");
	printf("%f", media);
	printf("\n");


	return 0;
}
