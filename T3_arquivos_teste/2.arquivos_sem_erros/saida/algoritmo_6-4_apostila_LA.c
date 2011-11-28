/*
  Geracao de uma tabela de conversao Celsius/Fahrenheit de 0 a 10, de
  0,5 em 0,5
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float celsius, fahrenheit;

	celsius = 0; /* inicial */
	do{
		/* calcula o valor */
		fahrenheit = celsius * 1.8 + 32;
		printf("%f --- %f\n", celsius, fahrenheit);

		/* proximo */
		celsius = celsius + 0.5;
	}while(!(celsius > 10));

	return 0;
}
