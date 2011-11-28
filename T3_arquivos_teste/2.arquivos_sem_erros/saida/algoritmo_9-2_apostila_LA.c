/*
  a partir de dois números racionais, calcular e apresentar a soma e a
multiplicação de ambos
  Jander Moreira
  2010
*/

#include "basicos.h"

/* definicao de tipo */
typedef struct{
	int numerador, denominador;
} tRacional;

void simplifiqueRacional(tRacional *racional){
	int valor1, valor2, resto;

	/* calculo do MDC */
	valor1 = racional->numerador;
	valor2 = racional->denominador;

	do{
		resto = valor1 % valor2;
		valor1 = valor2;
		valor2 = resto;
	}while(!(resto == 0)); 	/* resultado do MDC fica em valor1 */

	/* simplificacao da razao */
	racional->numerador = racional->numerador/valor1;
	racional->denominador = racional->denominador/valor1;
}
	
int main(){
	tRacional numero1, numero2, resultado;

	/* Obtencao dos numeros */
	printf("%s", "Numerador do primeiro racional: ");
	printf("\n");
	scanf("%d", &numero1.numerador); limpa_entrada();
	printf("%s", "Denominador do primeiro racional: ");
	printf("\n");
	scanf("%d", &numero1.denominador); limpa_entrada();
	printf("%s", "Numerador do segundo racional: ");
	printf("\n");
	scanf("%d", &numero2.numerador); limpa_entrada();
	printf("%s", "Denominador do segundo racional: ");
	printf("\n");
	scanf("%d", &numero2.denominador); limpa_entrada();

	/* calculo da soma */
	resultado.numerador = numero1.numerador * numero2.denominador +
				numero1.denominador * numero2.numerador;
	resultado.denominador = numero1.denominador * numero2.denominador;

	simplifiqueRacional(&resultado);

	/* escrita da soma */
	printf("%d", resultado.numerador);
	printf("%s", "/");
	printf("%d", resultado.denominador);
	printf("\n");

	/* calculo do produto */
	resultado.numerador = numero1.numerador * numero2.numerador;
	resultado.denominador = numero1.denominador * numero2.denominador;

	simplifiqueRacional(&resultado);

	/* escrita do produto */
	printf("%d", resultado.numerador);
	printf("%s", "/");
	printf("%d", resultado.denominador);
	printf("\n");

	return 0;
}
