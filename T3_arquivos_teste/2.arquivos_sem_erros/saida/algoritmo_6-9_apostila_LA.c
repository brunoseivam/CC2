/*
  dada uma relação de preços e quantidade relativas a mídias de DVD
  virgens, calcular o valor total em estoque (em reais) e o preço médio
  da unidade de DVD; o fim dos dados é indicado por preço e quantidade
  iguais a zero
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int numeroItens, quantidade;
	float preco, valorTotal, somaUnitarios;

	/* valores iniciais dos contadores */
	numeroItens = 0;
	valorTotal = 0;
	somaUnitarios = 0;

	/* leitura dos dados */
	printf("%s", "Preco: ");
	printf("\n");
	scanf("%f", &preco); limpa_entrada();
	printf("%s", "Quantidade: ");
	printf("\n");
	scanf("%d", &quantidade); limpa_entrada();
	while(preco != 0 && quantidade != 0){
		/* conta e acumula */
		numeroItens = numeroItens + 1;
		valorTotal = valorTotal + preco * quantidade;
		somaUnitarios = somaUnitarios + preco;

		/* obtem o proximo item */
		printf("%s", "Preco: ");
		printf("\n");
		scanf("%f", &preco); limpa_entrada();
		printf("%s", "Quantidade: ");
		printf("\n");
		scanf("%d", &quantidade); limpa_entrada();
	}

	/* resultados */
	printf("%s", "Valor total: R$ ");
	printf("%f", valorTotal);
	printf("\n");
	if(numeroItens == 0){
		printf("%s", "Nao eh possivel calcular o preco medio unitario");
		printf("\n");
	}
	else{
		printf("Valor medio unitario: R$ ");
		printf("%f", 1.0 * somaUnitarios/numeroItens);
		printf("\n");
	}

	return 0;
}
