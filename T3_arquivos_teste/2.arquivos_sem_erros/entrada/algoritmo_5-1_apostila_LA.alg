{ cálculo da média final de um aluno, feita pela média aritmética de duas provas, e escrever mensagem indicando aprovação ou reprovação }

algoritmo
	declare notaProva1, notaProva2, media: real

	{ leitura das notas }
	escreva("Nota da P1: ")
	leia(notaProva1)
	escreva("Nota da P2: ")
	leia(notaProva2)

	{ cálculo da média }
	media <- (notaProva1 + notaProva2)/2

	{ resultados }
	escreva(media)

	se media >= 6 entao
		escreva("Aprovado :)...")
	senao
		escreva("Reprovado :(...")
	fim_se
fim_algoritmo
