{ classificação da faixa etária segundo um critério arbitrário }

algoritmo
	declare idade: inteiro

	{ leitura da idade }
	escreva("Digite uma idade: ")
	leia(idade)

	{ classificação }
	caso idade seja
		0: escreva("bebe")
		1..10: escreva("crianca")
		11..14: escreva("pre-adolescente")
		15..18: escreva("adolescente")
		19..120: escreva("adulto")
	senao
		escreva("Idade invalida ou sem classificacao definida")
	fim_caso
fim_algoritmo
