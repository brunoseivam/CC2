#!/bin/bash

folder1="T3_arquivos_teste/1.arquivos_com_erros"
folder2="T3_arquivos_teste/2.arquivos_sem_erros"
folder3="T3_arquivos_teste/3.arquivos_gera_codigo_C"


#gcc -Wall common.c lex.c syntax.c semant.c generator.c datastruc/btree.c datastruc/stack.c datastruc/list.c main.c -o  compiler
gcc -Wall common.c lex.c syntax.c semant.c datastruc/btree.c datastruc/stack.c datastruc/list.c main.c -o  compiler

#./compiler entrada.txt saida.txt
#exit


for i in $(ls $folder1/entrada)
do
	echo $i
	./compiler $folder1/entrada/$i $i	
	diff $i $folder1/saida/$i
	#diff $i saida_ok.txt
	rm $i
done

for i in $(ls $folder2/entrada)
do
	echo $i
	./compiler $folder2/entrada/$i $i	
	#diff $i $folder/saida/$i
	diff $i saida_ok.txt
	rm $i
done

for i in $(ls $folder3/entrada)
do
	echo $i
	./compiler $folder3/entrada/$i $i	
	#diff $i $folder/saida/$i
	diff $i saida_ok.txt
	rm $i
done

exit

for i in $(ls $folder1/entrada)
do
	./compiler $folder2/entrada/$i $i
	diff $i $folder2/saida/$i
	rm $i
done


