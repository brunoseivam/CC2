/*
  determinar a menor temperatura de um conjunto, sabendo-se a quantidade
  de valores disponíveis
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int i, numeroEntradas;
	float temperatura, menorTemperatura;

	/* obtencao da quantidade de itens */
	printf("%s", "Quantidade de temperaturas: ");
	printf("\n");
	scanf("%d", &numeroEntradas); limpa_entrada();

	if(numeroEntradas <= 0){
		printf("%s", "Nao ha dados; portanto nao ha menor temperatura");
		printf("\n");
	}
	else{
		/* leitura da primeira temperatura */
		printf("%s", "Temperatura: ");
		printf("\n");
		scanf("%f", &temperatura); limpa_entrada();
		menorTemperatura = temperatura; /* primeira eh menor */

		/* repeticao para leitura das outras temperaturas */
		for(i = 2; i <= numeroEntradas; i++){
			/* leitura */
			printf("%s", "Temperatura: ");
			printf("\n");
			scanf("%f", &temperatura); limpa_entrada();

			/* verificacao da maior temperatura */
			if(temperatura < menorTemperatura){
				menorTemperatura = temperatura;
			}
		}

		/* resultado */
		printf("%s", "Menor temperatura: ");
		printf("%f", menorTemperatura);
		printf("\n");
	}

	return 0;
}
