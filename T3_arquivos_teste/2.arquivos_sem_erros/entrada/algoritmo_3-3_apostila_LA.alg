{ determinar, dados os comprimentos dos lados de um triângulo, se o triângulo pode ser formado (se existe) e qual o tipo dele (equilátero, isósceles ou escaleno) }

algoritmo
	declare
		lado1, lado2, lado3: real
	declare
		existeTriangulo,
		ehEquilatero, ehIsosceles, ehEscaleno: logico

	{ leitura dos comprimentos dos lados }
	escreva("Lado 1: ")
	leia(lado1)
	escreva("Lado 2: ")
	leia(lado2)
	escreva("Lado 3: ")
	leia(lado3)

	{ verificações necessárias }
	existeTriangulo <- lado1 < lado2 + lado3 e
				lado2 < lado1 + lado3 e
				lado3 < lado1 + lado2
	
	ehEquilatero <- existeTriangulo e lado1 = lado2 e lado2 = lado3

	ehIsosceles <- existeTriangulo e nao ehEquilatero e
			(lado1 = lado2 ou lado2 = lado3 ou lado1 = lado3)

	ehEscaleno <- existeTriangulo e nao ehEquilatero e nao ehIsosceles

	{ resultados }
	escreva("Triangulo existe? ", existeTriangulo)
	escreva("Eh equilatero? ", ehEquilatero)
	escreva("Eh isosceles? ", ehIsosceles)
	escreva("Eh escaleno? ", ehEscaleno)

fim_algoritmo
