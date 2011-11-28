/*
  Include basicos
  Rotinas basicas de adaptacao para C
  Jander
  2010
*/


#include "basicos.h"

char *subLiteral(char *str, int inicio, int comprimento){
        static char sub[80];
        int i;

        for(i = 0; i < comprimento; i++)
                sub[i] = *str++;
        sub[comprimento] = '\0';

        return sub;
}

void limpa_entrada(){
        char byte;
        while( (byte = fgetc(stdin)) != EOF && byte != '\n')
                ;
}

double sen(double angulo){

	return sin(angulo);
}

double pot(double base, double expoente){

	return pow(base, expoente);
}

int trunca(double valor){

	return (int)valor;
}

double frac(double valor){

	return valor - (int)valor;
}

