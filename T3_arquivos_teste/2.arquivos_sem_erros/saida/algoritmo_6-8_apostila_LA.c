/*
  determinar, dadas uma quantidade conhecida de pessoas e suas idades,
  quantas são maiores de 18 (maior ou igual) e, dentre estas, a
  porcentagem de pessoas acima de 60 anos
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int i, numeroPessoas, idade, conta18, conta60;

	/* obtencao da quantidade de pessoas */
	printf("%s", "Numero de pessoas: ");
	printf("\n");
	scanf("%d", &numeroPessoas); limpa_entrada();

	/* contagem de maiores de idade e acima de 60 anos */
	conta18 = 0;
	conta60 = 0;
	for(i = 1; i <= numeroPessoas; i++){
		/* leitura da idade */
		printf("%s", "Idade: ");
		printf("\n");
		scanf("%d", &idade); limpa_entrada();

		/* analisa idade */
		if(idade >= 18){
			conta18 = conta18 + 1; /* conta mais um */
			if(idade > 60){
				conta60 = conta60 + 1;
			}
		}
	}

	/* resultados */
	if(conta18 == 0){
		printf("%s", "Nao ha maiores de idade nem sexagenarios");
		printf("\n");
	}
	else{
		printf("%s", "Ha ");
		printf("%d", conta18);
		printf("%s", " pessoas maiores de idade");
		printf("\n");
		printf("%s", "Destas, ");
		printf("%f", 100.0 * conta60/conta18);
		printf("%s", "% tem mais de 60 anos");
		printf("\n");
	}

	return 0;
}
