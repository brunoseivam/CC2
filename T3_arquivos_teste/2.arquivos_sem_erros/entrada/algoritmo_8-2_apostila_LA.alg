{ determinar, em R3, qual de um conjunto de pontos é o mais próximo da origem, sabendo-se o número de elementos do conjunto }

algoritmo
	declare
		ponto, pontoMaisProximo: registro
						x, y, z: real
					fim_registro
	declare distancia, menorDistancia: real
	declare i, numeroPontos: inteiro

	{ obtenção do número de pontos }
	escreva("Numero de pontos: ")
	leia(numeroPontos)

	se numeroPontos <= 0 entao
		escreva("Nao ha pontos a serem processados ou valor invalido")
	senao
		{ leitura das coordenadas do primeiro ponto}
		escreva("Digite as coordenadas do ponto: ")
		leia(ponto.x, ponto.y, ponto.z)
		pontoMaisProximo <- ponto { o primeiro é o mais próximo }
		menorDistancia <- raiz(pot(ponto.x, 2) + pot(ponto.y, 2) + pot(ponto.z, 2))

		{ processa os demais pontos }
		para i <- 2 ate numeroPontos faca
			escreva("Digite as coordenadas do ponto: ")
			leia(ponto.x, ponto.y, ponto.z)
			distancia <- raiz(pot(ponto.x, 2) + pot(ponto.y, 2) + pot(ponto.z, 2))
			{ comparação }
			se distancia < menorDistancia entao
				menorDistancia <- distancia
				pontoMaisProximo <- ponto
			fim_se
		fim_para

		{ resultado }
		escreva("O ponto mais proximo:", pontoMaisProximo.x, pontoMaisProximo.y, pontoMaisProximo.z)
	fim_se
fim_algoritmo
