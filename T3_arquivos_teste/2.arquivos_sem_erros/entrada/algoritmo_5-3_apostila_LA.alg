{ determinar, dados os comprimentos dos lados de um triângulo, se o triângulo pode ser formado (se existe) e qual o tipo dele (equilátero, isósceles ou escaleno) }

algoritmo
	declare
		lado1, lado2, lado3: real
	declare
		classificacao: literal

	{ leitura dos comprimentos dos lados }
	escreva("Lado 1: ")
	leia(lado1)
	escreva("Lado 2: ")
	leia(lado2)
	escreva("Lado 3 :")
	leia(lado3)

	se lado1 >= lado2 + lado3 ou lado2 >= lado1 + lado3 ou 
			lado3 >= lado1 + lado2 entao { existe? }		
		escreva("Os lados fornecidos nao formam um triangulo")
	senao
		se lado1 = lado2 e lado2 = lado3 entao { lados iguais? }
			classificacao <- "Equilatero"
		senao
			se lado1 = lado2 ou lado1 = lado3 ou
					lado2 = lado3 entao { isosceles? }
				classificacao <- "Isosceles"
			senao
				classificacao <- "Escaleno"
			fim_se
		fim_se

		{ escrita da classificação }
		escreva(classificacao)
	fim_se

fim_algoritmo
