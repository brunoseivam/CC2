{ dado o comprimento de um arco, calcular seu cosseno pela soma cos(x) = S x^i/i! para um dado numero de termos, para valores de x proximos a zero }

algoritmo
	declare i, baseFatorial, numeroTermos: inteiro
	declare fatorial, termo, cosseno, angulo: real

	{ leitura do arco e do numero de termos }
	escreva("Angulo (rad): ")
	leia(angulo) { angulo em radianos }
	escreva("Numero de termos da soma: ")
	leia(numeroTermos)

	{ calculo da aproximacao do cosseno }
	cosseno <- 0   { acumulador do resultado }
	baseFatorial <- 1
	fatorial <- 1
	termo <- 1
	para i <- 1 ate numeroTermos faca
		{ faz o somatorio }
		se i % 2 = 1 entao
			cosseno <- cosseno + termo   { soma termos impares }
		senao
			cosseno <- cosseno - termo   { subtrai termos pares }
		fim_se

		{ calcula o proximo termo }
		fatorial <- fatorial * baseFatorial * (baseFatorial + 1)
		baseFatorial <- baseFatorial + 2
		termo <- pot(angulo, i + 1)/fatorial
	fim_para

	{ resultado calculado }
	escreva("cos(", angulo, ") = ", cosseno)
fim_algoritmo
