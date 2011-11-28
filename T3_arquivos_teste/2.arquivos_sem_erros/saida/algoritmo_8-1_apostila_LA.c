/*
  determinar, em R3, qual de dois pontos é o mais próximo da origem
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	struct{
		float x, y, z;
	} ponto1, ponto2;
	float distancia1, distancia2;

	/* leitura das coordenadas dos pontos */
	printf("%s", "Digite as coordenadas do primeiro ponto: ");
	printf("\n");
	scanf("%f", &ponto1.x); limpa_entrada();
	scanf("%f", &ponto1.y); limpa_entrada();
	scanf("%f", &ponto1.z); limpa_entrada();
	printf("%s", "Digite as coordenadas do segundo ponto: ");
	printf("\n");
	scanf("%f", &ponto2.x); limpa_entrada();
	scanf("%f", &ponto2.y); limpa_entrada();
	scanf("%f", &ponto2.z); limpa_entrada();

	/* calculo das distancias */
	distancia1 = raiz(pot(ponto1.x, 2) + pot(ponto1.y, 2) +
			pot(ponto1.z, 2));
	distancia2 = raiz(pot(ponto2.x, 2) + pot(ponto2.y, 2) +
			pot(ponto2.z, 2));

	/* comparacao e resultado */
	if(distancia1 < distancia2){
		printf("%s", "O primeiro ponto eh o mais proximo");
		printf("\n");
	}
	else{
		if(distancia2 < distancia1){
			printf("%s", "O segundo ponto eh o mais proximo");
			printf("\n");
		}
		else{
			printf("%s", "Os pontos equidistam da origem");
			printf("\n");
		}
	}

	return 0;
}
