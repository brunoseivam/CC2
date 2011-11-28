/*
  Calcular a media de tres provas para cada aluno de uma turma,
  dado o numero de alunos e as notas das provas
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	int i, numeroAlunos;
	float prova1, prova2, prova3, media;

	/* obtencao do numero de alunos */
	printf("Numero de alunos: ");
	scanf("%d", &numeroAlunos);

	/* calculo da media de cada aluno */
	for(i = 1; i <= numeroAlunos; i++){
		/* obtencao */
		printf("Nota da P1: ");
		scanf("%f", &prova1); limpa_entrada();
		printf("Nota da P2: ");
		scanf("%f", &prova2); limpa_entrada();
		printf("Nota da P3: ");
		scanf("%f", &prova3); limpa_entrada();

		/* calculo e resultado */
		media = (prova1 + prova2 + prova3)/3;
		printf("%f\n", media);
	}

	return 0;
}
