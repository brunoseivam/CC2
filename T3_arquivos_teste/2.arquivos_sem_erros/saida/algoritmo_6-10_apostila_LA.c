/*
  dado o comprimento de um arco, calcular seu cosseno pela soma 
  cos(x) = Sum x^i/i! para um dado numero de termos
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int i, baseFatorial, numeroTermos;
	float termo, cosseno, fatorial, angulo;

	/* leitura do arco e numero de termos */
	printf("%s", "Angulo (rad): ");
	printf("\n");
	scanf("%f", &angulo); limpa_entrada();
	printf("%s", "Numero de termos da soma: ");
	printf("\n");
	scanf("%d", &numeroTermos); limpa_entrada();

	/* calculo da aproximacao do cosseno */
	cosseno = 0;
	baseFatorial = 1;
	fatorial = 1;
	termo = 1;
	for(i = 1; i <= numeroTermos; i++){
		/* faz o somatorio */
		if(i % 2 == 1){
			cosseno = cosseno + termo; /* soma impares */
		}
		else{
			cosseno = cosseno - termo; /* subtrai pares */
		}

		/* calcula proximo termo */
		fatorial = fatorial * baseFatorial * (baseFatorial + 1);
		baseFatorial = baseFatorial + 2;
		termo = pow(angulo, i + 1)/fatorial;
	}

	/* resultado calculado */
	printf("%s", "cos(");
	printf("%f", angulo);
	printf("%s", ") = ");
	printf("%f", cosseno);
	printf("\n");

	return 0;
}
