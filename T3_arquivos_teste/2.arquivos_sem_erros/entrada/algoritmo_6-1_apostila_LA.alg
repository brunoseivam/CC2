{ algoritmo para gerar uma tabela de conversão Celsius -> Fahrenheit para valores de 0 a 40, de 1 em 1 }

algoritmo
	declare celsius: inteiro { sempre inteiro, neste caso }
	declare fahrenheit: real { real, pois é obtido pela conversão }

	{ repetição para os valores indicados }
	para celsius <- 0 ate 40 faca
		fahrenheit <- celsius * 1.8 + 32
		escreva(celsius, " --- ", fahrenheit)
	fim_para
fim_algoritmo
