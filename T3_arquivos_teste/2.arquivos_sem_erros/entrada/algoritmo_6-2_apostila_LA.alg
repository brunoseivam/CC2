{ algoritmo que apresenta uma saudação para alguns nomes }

algoritmo
	declare nome: literal

	{ primeiro nome a ser lido }
	escreva("Digite um nome (ou 'fim' para terminar): ")
	leia(nome)

	{ repetição que termina quando o nome for "fim" }
	enquanto nome <> "fim" faca
		{ escreve a saudação }
		escreva("Ola,", nome, ", como vai?")

		{ leitura do próximo nome }
		escreva("Digite um nome (ou 'fim' para terminar): ")
		leia(nome)
	fim_enquanto

	{ despedida }
	escreva("Nao ha mais ninguem? Entao, tchau!")

fim_algoritmo
