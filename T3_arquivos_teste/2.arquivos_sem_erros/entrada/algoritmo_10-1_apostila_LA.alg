{ calcular, para um conjunto de 20 valores inteiros, quantos destes são maiores ou iguais à média do conjunto }

algoritmo
	{ declaração de variáveis }
	declare
		soma, contador, i,
		valor[20]: inteiro { valor armazena 20 inteiros }
	declare
		media: real

	{ obtenção dos dados e cálculo da média }
	soma <- 0
	para i <- 0 ate 19 faca
		escreva("Digite um valor: ")
		leia(valor[i])
		soma <- soma + valor[i]
	fim_para
	media <- soma/20.0

	{ contagem de quantos valores são maiores ou iguais à média }
	contador <- 0
	para i <- 0 ate 19 faca
		se valor[i] >= media entao
			contador <- contador + 1
		fim_se
	fim_para

	{ resultado }
	escreva("Ha", contador, "iten(s) maiore(s) que a media ", media)
fim_algoritmo
