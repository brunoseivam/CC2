/*
  determinar, em R3, qual de um conjunto de pontos é o mais próximo
da origem, sabendo-se o número de elementos do conjunto 
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	struct{
		float x, y, z;
	} ponto, pontoMaisProximo;
	float distancia, menorDistancia;
	int i, numeroPontos;


	/* obtencao do numero de pontos */
	printf("%s", "Numero de pontos: ");
	printf("\n");
	scanf("%d", &numeroPontos); limpa_entrada();
	
	if(numeroPontos == 0){
		printf("%s", "Nao ha pontos a serem processados ou valor invalido");
		printf("\n");
	}
	else{
		/* leitura das coordenadas do primeiro ponto */
		printf("%s", "Digite as coordenadas do ponto: ");
		printf("\n");
		scanf("%f", &ponto.x); limpa_entrada();
		scanf("%f", &ponto.y); limpa_entrada();
		scanf("%f", &ponto.z); limpa_entrada();
		pontoMaisProximo = ponto; /* primeiro */
		menorDistancia = raiz(pot(ponto.x, 2) +
			pot(ponto.y, 2) + pot(ponto.z, 2));

		/* processa demais pontos */
		for(i = 2; i <= numeroPontos; i++){
			printf("%s", "Digite as coordenadas do ponto: ");
			printf("\n");
			scanf("%f", &ponto.x); limpa_entrada();
			scanf("%f", &ponto.y); limpa_entrada();
			scanf("%f", &ponto.z); limpa_entrada();

			/* comparacao */
			if(distancia < menorDistancia){
				menorDistancia = distancia;
				pontoMaisProximo = ponto;
			}
		}

		/* resultado */
		printf("%s", "O ponto mais proximo: ");
		printf("%f ", pontoMaisProximo.x);
		printf("%f ", pontoMaisProximo.y);
		printf("%f ", pontoMaisProximo.z);
		printf("\n");
	}

	return 0;
}
