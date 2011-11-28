/*
  Apresentar, para uma conta de lanchonete, o valor total da conta,
  o valor pago e o troco devolvido, dados o total consumido de bebidas e
  alimentos, alem de se saber que a porcentagem do garcom vale 10%
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float totalBebidas, totalAlimentos, valorPago, troco,
		porcentagemGarcom;

	/* obtencao dos dados necessarios */
	printf("Total de bebidas: ");
	scanf("%f", &totalBebidas); limpa_entrada();
	printf("Total de alimentos: ");
	scanf("%f", &totalAlimentos); limpa_entrada();
	printf("Total pago: ");
	scanf("%f", &valorPago); limpa_entrada();

	/* calculos necessarios */
	porcentagemGarcom = (totalBebidas + totalAlimentos) * 10/100.0; // 10%
	troco = valorPago - (totalBebidas + totalAlimentos + porcentagemGarcom);

	/* apresentacao dos resultados */
	printf("Valor da conta: %f\n",
		totalBebidas + totalAlimentos + porcentagemGarcom);
	printf("Valor pago: %f\n", valorPago);
	printf("Troco: %f\n", troco);

	return 0;
}
