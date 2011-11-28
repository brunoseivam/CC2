#ifndef SEMANT_H
#define SEMANT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "datastruc/btree.h"
#include "datastruc/stack.h"
#include "common.h"

#define  SEM_ENTRY_TYPE_STRING      "literal"
#define  SEM_ENTRY_TYPE_INTEGER     "inteiro"
#define  SEM_ENTRY_TYPE_REAL        "real"
#define  SEM_ENTRY_TYPE_LOGIC       "logico"

#define  SEM_UPD_POINTER            1
#define  SEM_UPD_TYPE               2


#define  BTREE_ORDER          5

typedef enum sem_entry_category
{
   variable = 1,
   type_def,
   procedure,
   function,
   constant,

}sem_entry_category;


btree*   sem_table;
char*    pending_vector[20];
int      pending_vector_size;


typedef struct sem_table_entry
{
   char*                string;
   sem_entry_category   category;
   char*                type;
   int                  num_param;
   short                is_pointer;
   void*                more_info;
}sem_table_entry;

void  sem_init_table(void);

int   sem_insert_pending(char* string, sem_entry_category category);



void sem_print_stack();
void sem_insert(char*, sem_entry_category, char*, int, short , void*);

#endif
