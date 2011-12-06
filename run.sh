#!/bin/bash

folder2="T3_arquivos_teste/2.arquivos_sem_erros"
folder1="T3_arquivos_teste/1.arquivos_com_erros"

gcc -Wall common.c lex.c syntax.c semant.c generator.c datastruc/btree.c datastruc/stack.c datastruc/list.c main.c -o  compiler

./compiler entrada.txt saida.txt
exit


for i in $(ls $folder1/entrada)
do
	./compiler $folder1/entrada/$i $i
	echo $i
	diff $i $folder1/saida/$i
	rm $i
done

exit

for i in $(ls $folder2/entrada)
do
	./compiler $folder2/entrada/$i $i
	diff $i $folder2/saida/$i
	rm $i
done


