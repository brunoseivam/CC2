/*
  Calculo do consumo medio de um veiculo, dados a quilometragem total
  percorrida e o numero de litros utilizado nos dois unicos abastecimentos
  realizados
  Jander Moreira
  2010
*/

#include "basicos.h"

int main(){
	float quilometragem, abastecimento1, abastecimento2;

	/* obtencao da quilometragem e litros consumidos */
	printf("Quilometragem total: ");
	scanf("%f", &quilometragem); limpa_entrada();
	printf("Primeiro abastecimento (litros): ");
	scanf("%f", &abastecimento1); limpa_entrada();
	printf("Segundo abastecimento (litros): ");
	scanf("%f", &abastecimento2); limpa_entrada();

	/* escrita do resultado do consumo */
	printf("%f km/l\n", quilometragem/(abastecimento1 + abastecimento2));

	return 0;
}
