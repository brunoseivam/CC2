{ cálculo do consumo médio de um veículo, dados a quilometragem total percorrida e o número de litros utilizados nos dois únicos abastecimentos realizados }

algoritmo
	declare
		quilometragem, consumo,
		abastecimento1, abastecimento2: real

	{ obtenção da quilometragem e litros consumidos }
	escreva("Quilometragem total: ")
	leia(quilometragem)
	escreva("Primeiro abastecimento (litros): ")
	leia(abastecimento1)
	escreva("Segundo abastecimento (litros): ")
	leia(abastecimento2)

	{ cálculo do consumo }
	consumo <- quilometragem/(abastecimento1 + abastecimento2)

	{ saída do resultado calculado }
	escreva(consumo, " km/l")

fim_algoritmo
