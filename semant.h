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

#define  SEM_BTREE_ORDER            5


typedef enum sem_category
{
   variable = 1,
   type_def,
   procedure,
   function,
   constant

}sem_category;

typedef enum sem_pending_upd_type
{
   sem_upd_type = 1,
   sem_upd_is_pointer,
   sem_upd_num_param,
   sem_upd_more_info

}sem_pending_upd_type;

typedef struct sem_entry
{
   char*          string;
   sem_category   category;
   char*          type;
   int            num_param;
   int            is_pointer;
   void*          more_info;
}sem_entry;

typedef struct sem_table
{
   btree*      table;
   stack*      pending_stack;
   sem_entry*  pending_changes;
}sem_table;


sem_table*  sem_global_table;
sem_table*  sem_local_table;

sem_table*  sem_current_table;

void        sem_error(int error_code);
int         sem_compare_keys  (const void* key1, const void* key2);

void        sem_init          (void);

sem_entry*  sem_entry_get     (void);
void        sem_entry_dispose (sem_entry* entry);

sem_table*  sem_table_get     (void);

int         sem_table_insert  (sem_table* table, sem_entry* entry);
int         sem_table_remove  (sem_table* table, char* string);
int         sem_table_find    (sem_table* table, char* string);
void        sem_table_dispose (sem_table* table);


int         sem_pending_insert   (char* string, sem_category category);
void        sem_pending_update   (sem_pending_upd_type upd, void* value);
void        sem_pending_commit   (void);
void        sem_scope_change     (void);


#endif
