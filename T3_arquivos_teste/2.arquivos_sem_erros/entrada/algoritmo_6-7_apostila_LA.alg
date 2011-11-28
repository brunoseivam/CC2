{ determinar a menor temperatura de um conjunto, sabendo-se a quantidade de valores disponíveis }

algoritmo
	declare
		i, numeroEntradas: inteiro
	declare
		temperatura, menorTemperatura: real

	{ obtenção da quantidade de itens }
	escreva("Quantidade de temperaturas: ")
	leia(numeroEntradas)
	
	se numeroEntradas <= 0 entao
		escreva("Nao ha dados; portanto nao ha menor temperatura")
	senao
		{ leitura da primeira temperatura, em separado }
		escreva("Temperatura: ")
		leia(temperatura)
		menorTemperatura <- temperatura { a primeira é a menor }

		{ repetição para leitura das outras temperaturas}
		para i <- 2 ate numeroEntradas faca
			{ leitura}
			escreva("Temperatura: ")
			leia(temperatura)

			{ verificação da menor temperatura }
			se temperatura < menorTemperatura entao
				menorTemperatura <- temperatura
			fim_se
		fim_para

		{ resultado }
		escreva("Menor temperatura: ", menorTemperatura)
	fim_se
fim_algoritmo
