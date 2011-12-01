#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semant.h"

void generator_init();
void gen_main_begin();
void gen_main_end();
void gen_const(sem_table* cur,char * value);
void gen_variable(sem_table* cur);


#endif