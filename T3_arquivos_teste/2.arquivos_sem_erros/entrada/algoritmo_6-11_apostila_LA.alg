{ dado um numero inteiro, determinar se eh um numero perfeito, ou seja, se eh igual ah soma de seus divisores, exceto ele mesmo }

algoritmo
	declare
		i, valor, soma: inteiro

	{ leitura do numero }
	escreva("Digite um valor inteiro positivo: ")
	leia(valor)

	{ soma dos divisores de valor }
	soma <- 0
	para i <- 1 ate valor/2 faca
		se valor % i = 0 entao    { soma somente se for divisor }
			soma <- soma + i
		fim_se
	fim_para
 
	{ resultado }
	se soma = valor e valor <> 0 entao
		escreva(valor, "eh um numero perfeito")
	senao
		escreva(valor, "nao eh um numero perfeito")
	fim_se
fim_algoritmo
