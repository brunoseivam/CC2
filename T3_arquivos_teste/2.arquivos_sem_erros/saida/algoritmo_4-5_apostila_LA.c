/*
  Verificacao se um projetil, lancado em angulo colide ou nao com um
  objeto no caminho, sabendo-se a distancia do objeto, a espessura e
  altura, resultando em verdadeiro se ha colisao, falso caso contrario
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float angulo, velocidadeInicial, distancia, espessura, altura,
		momento1, momento2, alturaProjetil1, alturaProjetil2;
	unsigned short int colidiu;

	/* obtencao dos dados */
	printf("Angulo de lancamento: ");
	scanf("%f", &angulo); limpa_entrada();
	printf("Velocidade inicial (m/s): ");
	scanf("%f", &velocidadeInicial); limpa_entrada();
	printf("Distancia do objeto (m): ");
	scanf("%f", &distancia); limpa_entrada();
	printf("Espessura do objeto (m): ");
	scanf("%f", &espessura); limpa_entrada();
	printf("Altura do objeto (m): ");
	scanf("%f", &altura); limpa_entrada();

	/* calculo dos instantes em que o projetil passa pelos limites
	   do objeto */
	momento1 = distancia/(velocidadeInicial * cos(angulo * 3.1415/180));
	momento2 = (distancia + espessura)/
			(velocidadeInicial * cos(angulo * 3.1415/180));

	/* calculo da altura do projetil nos dois instantes */
	alturaProjetil1 = velocidadeInicial * sen(angulo * 3.1415/180)
				* momento1 - 9.8 * pot(momento1, 2)/2;
	alturaProjetil2 = velocidadeInicial * sen(angulo * 3.1415/180)
				* momento1 - 9.8 * pot(momento1, 2)/2;

	/* verificacao da colisao */
	colidiu = alturaProjetil1 <= altura && alturaProjetil1 > 0 ||
			alturaProjetil2 <= altura && alturaProjetil2 > 0;

	/* resultado */
	printf("Houve colisao? %d\n", colidiu);

	return 0;
}
