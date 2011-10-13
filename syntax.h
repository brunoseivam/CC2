#ifndef SYNTAX_H
#define SYNTAX_H

#include "common.h"

const firsts programa_firsts;
const firsts declaracao_local_firsts;
const firsts declaracao_global_firsts;
/*
...
*/
const firsts outros_fatores_logicos_firsts;


int programa();
int declaracao_local();
int declaracao_global();
/*
...
*/
int outros_fatores_logicos();

#endif
