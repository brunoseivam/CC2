/*
  exemplo de modificação de variáveis diferentes usando o mesmo conjunto
de comandos
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int i, numeroPassos, valor1, valor2;
	int *ponteiro;

	/* atribuicoes iniciais */
	valor1 = 0;
	valor2 = 0;

	/* repeticao */
	printf("%s", "Numero de passos: ");
	printf("\n");
	scanf("%d", &numeroPassos); limpa_entrada();

	for(i = 1; i <= numeroPassos; i++){
		/* escolha da variavel */
		if(i % 5 == 0){
			ponteiro = &valor1;
		}
		else{
			ponteiro = &valor2;
		}

		/* manipulacao da variavel escolhida */
		*ponteiro = *ponteiro + i;
		if(*ponteiro % 2 == 1){
			*ponteiro = *ponteiro - 1;
		}
	}

	/* escritas */
	printf("%d ", valor1);
	printf("%d", valor2);
	printf("\n");

	return 0;
}
