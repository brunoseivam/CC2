#!/bin/bash
#
# compila todos os programas

echo Objetos...
for i in algoritmo_*.c basicos.c; do
	echo $i
	gcc -c $i
done

for i in algoritmo_*.o; do
	echo $i
	gcc -lm -o $i.out $i basicos.o
done

rm -f *.o
