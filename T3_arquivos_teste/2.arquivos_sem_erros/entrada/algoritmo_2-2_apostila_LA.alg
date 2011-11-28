{ leitura de nome e idade com escrita de mensagem usando estes dados }

algoritmo
	declare
		nome: literal
	declare
		idade: inteiro

	{ leitura de nome e idade do teclado }
	escreva("Nome: ")
	leia(nome)
	escreva("Idade: ")
	leia(idade)

	{ saída da mensagem na tela }
	escreva(nome, " tem ", idade, " anos.")

fim_algoritmo
