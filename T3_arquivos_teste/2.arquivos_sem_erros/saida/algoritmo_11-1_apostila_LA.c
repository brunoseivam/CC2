/*
  obter, para uma relação de dados sobre vinhos, o vinho de maior venda
 e a lista de todos os vinhos que tenham venda abaixo da média geral do
 grupo
  Jander Moreira
  2010
*/

#include "basicos.h"

#define maximoElementos	300

typedef struct{
	char nome[STRSIZE], nomeFabricante[STRSIZE],
		nomeDistribuidor[STRSIZE], tipoVinho[STRSIZE];
	int anoSafra, anoEnvasamento;
	float preco;
	int unidades;
} tVinho;


/* leitura de um registro de vinho */
void leiaVinho(tVinho *vinho){

	printf("Nome do vinho: ");
	leiaLiteral(vinho->nome);
	printf("Nome do fabricante: ");
	leiaLiteral(vinho->nomeFabricante);
	printf("Nome do distribuidor: ");
	leiaLiteral(vinho->nomeDistribuidor);
	printf("Tipo do vinho (R, T, B): ");
	leiaLiteral(vinho->tipoVinho);
	printf("Ano da safra: ");
	scanf("%d", &vinho->anoSafra); limpa_entrada();
	printf("Ano do envasamento: ");
	scanf("%d", &vinho->anoEnvasamento); limpa_entrada();
	printf("Preco da garrafa: ");
	scanf("%f", &vinho->preco); limpa_entrada();
	printf("Numero de garrafas: ");
	scanf("%d", &vinho->unidades); limpa_entrada();
}

/* escrita de um registro de vinho */
void escrevaVinho(tVinho vinho){

	printf("%s", "Nome do vinho: ");
	printf("%s", vinho.nome);
	printf("\n");
	printf("%s", "Nome do fabricante: ");
	printf("%s", vinho.nomeFabricante);
	printf("\n");
	printf("%s", "Nome do distribuidor: ");
	printf("%s", vinho.nomeDistribuidor);
	printf("\n");
	printf("%s", "Tipo do vinho (R, T, B): ");
	printf("%s", vinho.tipoVinho);
	printf("\n");
	printf("%s", "Ano da safra: ");
	printf("%d", vinho.anoSafra);
	printf("\n");
	printf("%s", "Ano do envasamento: ");
	printf("%d", vinho.anoEnvasamento);
	printf("\n");
	printf("%s", "Preco da garrafa: ");
	printf("%f", vinho.preco);
	printf("\n");
	printf("%s", "Numero de garrafas: ");
	printf("%d", vinho.unidades);
	printf("\n");
	printf("\n");
}

/* retorna o produto do preco pelo numero de unidades vendidas */
float calculeVendas(tVinho vinho){

	return vinho.unidades * vinho.preco;
}


int main(){
	int i, numeroItens;
	float somaVendas, maiorVenda, mediaVinhos;
	tVinho vinho[maximoElementos];

	/* leitura dos dados e calculo da media e maior venda */
	printf("%s", "Digite o numero de itens: ");
	printf("\n");
	scanf("%d", &numeroItens); limpa_entrada();
	somaVendas = 0;
	maiorVenda = -1;
	for(i = 0; i <= numeroItens - 1; i++){
		leiaVinho(&vinho[i]);
		somaVendas = somaVendas + calculeVendas(vinho[i]);
		if(calculeVendas(vinho[i]) > maiorVenda){
			maiorVenda = calculeVendas(vinho[i]);
		}
	}
	mediaVinhos = somaVendas/numeroItens;

	/* apresentacao da melhor venda */
	printf("%s", "Vinhos com melhor venda: ");
	printf("\n");
	for(i = 0; i <= numeroItens - 1; i++){
		if(calculeVendas(vinho[i]) == maiorVenda){
			escrevaVinho(vinho[i]);
		}
	}

	/* apresentacao dos vinhos abaixo da media calculada */
	printf("%s", "Vinhos abaixo da media: ");
	printf("\n");
	for(i = 0; i <= numeroItens - 1; i++){
		if(calculeVendas(vinho[i]) < mediaVinhos){
			escrevaVinho(vinho[i]);
		}
	}


	return 0;
}
