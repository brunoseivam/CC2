/*
  Programa para gerar uma tabela de conversao Celsius/Fahrenheit
  para valores de 0 a 40, de 1 em 1
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int celsius;
	float fahrenheit;

	/* repeticao para os valores indicados */
	for(celsius = 0; celsius <= 40; celsius++){
		fahrenheit = celsius * 1.8 + 32;
		printf("%d --- %f\n", celsius, fahrenheit);
	}

	return 0;
}
