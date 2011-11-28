/*
  Calculo da media final de um aluno, pela media de duas provas,
  escrevendo mensagem indicando aprovacao ou reprovacao
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float notaProva1, notaProva2, media;

	/* leitura das notas */
	printf("Nota da P1: ");
	scanf("%f", &notaProva1); limpa_entrada();
	printf("Nota da P2: ");
	scanf("%f", &notaProva2); limpa_entrada();

	/* calculo */
	media = (notaProva1 + notaProva2)/2;

	/* resultados */
	printf("%f\n", media);
	if(media >= 6){
		printf("Aprovado :)...\n");
	}
	else{
		printf("Reprovado :(...\n");
	}

	return 0;
}
