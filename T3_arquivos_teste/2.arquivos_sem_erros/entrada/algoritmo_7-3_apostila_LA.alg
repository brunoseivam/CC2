{ exemplo de modificação de variáveis diferentes usando o mesmo conjunto de comandos }

algoritmo
	declare
		i, numeroPassos,
		valor1, valor2: inteiro
	declare 
		ponteiro: ^inteiro

	{ atribuições iniciais }
	valor1 <- 0
	valor2 <- 0

	{ repetição }
	escreva("Numero de passos: ")
	leia(numeroPassos)

	para i <- 1 ate numeroPassos faca
		{ escolha da variável }
		se i % 5 = 0 entao
			ponteiro <- &valor1
		senao
			ponteiro <- &valor2
		fim_se

		{ comandos de manipulação da variável escolhida }
		^ponteiro <- ^ponteiro + i
		se ^ponteiro % 2 = 1 entao
			^ponteiro <- ^ponteiro - 1
		fim_se
	fim_para

	{ escritas }
	escreva(valor1, valor2)
fim_algoritmo
