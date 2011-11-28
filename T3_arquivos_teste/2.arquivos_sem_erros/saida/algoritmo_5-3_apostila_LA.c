/*
  Determinar, dados os comprimentos dos lados de um triangulo, se
  ele existe e qual seu tipo (equilatero, isosceles ou escaleno)
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float lado1, lado2, lado3;
	char classificacao[80];

	/* leitura dos lados */
	printf("Lado 1: ");
	scanf("%f", &lado1); limpa_entrada();
	printf("Lado 2: ");
	scanf("%f", &lado2); limpa_entrada();
	printf("Lado 3: ");
	scanf("%f", &lado3); limpa_entrada();

	if (lado1 >= lado2 + lado3 || lado2 > lado1 + lado3 || lado3 >= lado1 + lado2) {
		printf("Os lados fornecidos nao formam um triangulo\n");
	}
	else {
		if (lado1 == lado2 && lado2 == lado3) { /* lados iguais */
			strcpy(classificacao, "Equilatero");
		}
		else {
			if (lado1 == lado2 || lado1 == lado3 || lado2 == lado3) { 
				strcpy(classificacao, "Isosceles");
			}
			else {
				strcpy(classificacao, "Escaleno");
			}
		}
		/* resultado */
		printf("%s\n", classificacao);		
	}

	return 0;
}
