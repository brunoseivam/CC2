/*
  a partir de dois números racionais, calcular e apresentar a soma e a
multiplicação de ambos
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	typedef struct{
		int numerador, denominador;
	} tRacional;

	tRacional numero1, numero2, resultado;
	int valor1, valor2, resto;

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

	/* calculo do MDC */
	valor1 = resultado.numerador;
	valor2 = resultado.denominador;

	do{
		resto = valor1 % valor2;
		valor1 = valor2;
		valor2 = resto;
	}while(!(resto == 0)); 	/* resultado do MDC fica em valor1 */

	/* simplificacao da razao */
	resultado.numerador = resultado.numerador/valor1;
	resultado.denominador = resultado.denominador/valor1;

	/* escrita da soma */
	printf("%d", resultado.numerador);
	printf("%s", "/");
	printf("%d", resultado.denominador);
	printf("\n");

	/* calculo do produto */
	resultado.numerador = numero1.numerador * numero2.numerador;
	resultado.denominador = numero1.denominador * numero2.denominador;

	/* calculo do MDC */
	valor1 = resultado.numerador;
	valor2 = resultado.denominador;

	do{
		resto = valor1 % valor2;
		valor1 = valor2;
		valor2 = resto;
	}while(!(resto == 0)); 	/* resultado do MDC fica em valor1 */

	/* simplificacao da razao */
	resultado.numerador = resultado.numerador/valor1;
	resultado.denominador = resultado.denominador/valor1;

	/* escrita do produto */
	printf("%d", resultado.numerador);
	printf("%s", "/");
	printf("%d", resultado.denominador);
	printf("\n");

	return 0;
}
