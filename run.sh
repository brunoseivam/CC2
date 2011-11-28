#!/bin/bash

folder1="T2_arquivos_teste/arquivos_sem_erros"
folder2="T2_arquivos_teste/arquivos_com_1_erro"

gcc -Wall common.c lex.c syntax.c semant.c btree.c main.c -o compiler
./compiler entrada.txt saida.txt

exit

for i in $(ls $folder1/entrada)
do
	./parser $folder1/entrada/$i $i
	diff $i $folder1/saida/$i
	rm $i
done

for i in $(ls $folder2/entrada)
do
	./parser $folder2/entrada/$i $i
	diff $i $folder2/saida/$i
	rm $i
done

exit


