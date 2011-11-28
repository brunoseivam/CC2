{ calcular a média de três provas para cada aluno de uma turma, dado o número de alunos e as notas das provas }

algoritmo
	declare
		i, numeroAlunos: inteiro
	declare
		prova1, prova2, prova3, media: real

	{ obtenção do número de alunos }
	escreva("Numero de alunos: ")
	leia(numeroAlunos)

	{ cálculo da média para cada aluno }
	para i <- 1 ate numeroAlunos faca
		{ obtenção das notas }
		escreva("Nota da P1: ")
		leia(prova1)
		escreva("Nota da P2: ")
		leia(prova2)
		escreva("Nota da P3: ")
		leia(prova3)

		{ cálculo e escrita dos resultados }
		media <- (prova1 + prova2 + prova3)/3
		escreva(media)
	fim_para
fim_algoritmo
