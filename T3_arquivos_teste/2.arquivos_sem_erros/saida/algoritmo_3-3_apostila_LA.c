/*
  Determinar, dados os comprimentos dos lados de um triangulo, se
  ele pode ser formado (se existe) e qual seu tipo (equilatero,
  isosceles ou escaleno
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float lado1, lado2, lado3;
	unsigned short int existeTriangulo,
		ehEquilatero, ehIsosceles, ehEscaleno;

	/* leitura dos comprimentos dos lados */
	printf("Lado 1: ");
	scanf("%f", &lado1); limpa_entrada();
	printf("Lado 2: ");
	scanf("%f", &lado2); limpa_entrada();
	printf("Lado 3: ");
	scanf("%f", &lado3); limpa_entrada();

	/* verificacoes necessarias */
	existeTriangulo = lado1 < lado2 + lado3 &&
				lado2 < lado1 + lado3 &&
				lado3 < lado1 + lado2;

	ehEquilatero = existeTriangulo && lado1 == lado2 && lado2 == lado3;
	ehIsosceles = existeTriangulo && ! ehEquilatero &&
			(lado1 == lado2 || lado2 == lado3 || lado1 == lado3);
	ehEscaleno = existeTriangulo && ! ehEquilatero && ! ehIsosceles;

	/* resultados */
	printf("Triangulo existe? %d\n", existeTriangulo);
	printf("Equilatero? %d\n", ehEquilatero);
	printf("Isosceles? %d\n", ehIsosceles);
	printf("Escaleno? %d\n", ehEscaleno);

	return 0;
}
