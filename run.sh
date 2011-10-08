#!/bin/bash

folder="T1_arquivos_teste"

gcc -Wall -pedantic -ansi -o main.c lex.c -o main.out
exit





gcc -Wall -pedantic -ansi -o $prog.out $prog.c

for i in $(ls $folder/entrada)
do
	$prog.out $folder/entrada/$i $i
	diff $i $folder/saida/$i
	rm $i
done
exit


