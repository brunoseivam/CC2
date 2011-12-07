#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semant.h"

int gen_flag,tab;

typedef enum print_category
{
   pr_string = 1,
   pr_int,
   pr_float,
}print_category;

struct
{
	int is_type_def;
	int is_scanf;
	int waiting_exp;
	char* aux_ident;
	list* aux_list
}gen_gl_info;

void gen_init_info(void);
void gen_set_aux_ident(char* str);

void gen_print(print_category cat,int tabd,void* text);
void* gen_find(sem_table* table,char* key);

void generator_init();
void gen_main_begin();
void gen_main_end();
void gen_const_decl(sem_table* cur,char * value);
void gen_variable(sem_table* cur);
void gen_function(void);
void gen_printf(void);
void gen_if(void);
void gen_case(int begin,int end);
void gen_while(void);
void gen_do_while(void);



#endif
