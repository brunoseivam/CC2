{ dadas duas variáveis, uma inteira e outra literal, escrever os endereços de memória na qual estão armazenadas }

algoritmo
	declare valorReal: real 
	declare valorLiteral: literal 
	declare enderecoReal: ^real { endereços de variáveis reais }
	declare enderecoLiteral: ^literal { endereços de variáveis literais }

	{ armazenamento dos endereços }
	enderecoReal <- &valorReal
	enderecoLiteral <- &valorLiteral

	{ escrita do resultado }
	escreva("A variavel real esta no endereco: ", enderecoReal)
	escreva("A variavel literal esta no endereco: ", enderecoLiteral)
fim_algoritmo
