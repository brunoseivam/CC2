{ dados o nome, o preço e o tipo dos vinhos (indicados aqui por "T" para tinto, "B" para branco ou "R" para rosê, descrever o vinho mais caro; não são considerados vinhos de preços iguais; fim dos dados indicado por nome = "fim" }

algoritmo
	declare
		nome, nomeMaior,
		tipoVinho, tipoMaior: literal
	declare 
		preco, precoMaior: real

	{ repetição para leitura dos dados }
	precoMaior <- -1  { para forçar a troca na primeira verificação }
	faca
		{ dados }
		escreva("Nome: ")
		leia(nome)
		escreva("Preco: ")
		leia(preco)
		escreva("Tipo (R, T ou B): ")
		leia(tipoVinho)

		{ verificação do maior preço }
		se preco > precoMaior e nome <> "fim" entao
			nomeMaior <- nome
			precoMaior <- preco
			tipoMaior <- tipoVinho
		fim_se
	ate nome = "fim"

	{ apresentação do resultado }
	se precoMaior = -1 entao
		escreva("Nenhum vinho foi apresentado.")
	senao
		escreva(nomeMaior, precoMaior, tipoMaior)
	fim_se
fim_algoritmo
