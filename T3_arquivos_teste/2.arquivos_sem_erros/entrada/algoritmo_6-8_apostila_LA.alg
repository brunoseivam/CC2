{ determinar, dadas uma quantidade conhecida de pessoas e suas idades, quantas são maiores de 18 (maior ou igual) e, dentre estas, a porcentagem de pessoas acima de 60 anos }

algoritmo
	declare
		i, numeroPessoas, idade,
		conta18, conta60: inteiro

	{ obtenção da quantidade de pessoas }
	escreva("Numero de pessoas: ")
	leia(numeroPessoas)

	{ contagem de maiores de idade e de acima de 60 anos }
	conta18 <- 0
	conta60 <- 0
	para i <- 1 ate numeroPessoas faca
		{ leitura da idade }
		escreva("Idade: ")
		leia(idade)

		{ analisa a idade }
		se idade >= 18 entao
			conta18 <- conta18 + 1 { conta mais 1 }
			se idade > 60 entao
				conta60 <- conta60 + 1
			fim_se
		fim_se
	fim_para

	{ resultados }
	se conta18 = 0 entao
		escreva("Nao ha maiores de idade nem sexagenarios")
	senao
		escreva("Ha", conta18, "pessoas maiores de idade")
		escreva("Destas,", 100.0 * conta60/conta18, "% tem mais de 60 anos")
	fim_se
fim_algoritmo
