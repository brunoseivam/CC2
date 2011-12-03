#ifndef SEMANT_H
#define SEMANT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "datastruc/btree.h"
#include "datastruc/stack.h"
#include "datastruc/list.h"
#include "common.h"

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
   sem_upd_pointer,
   sem_upd_more_info,
   sem_upd_param_insert,
   sem_upd_param_update

}sem_pending_upd_type;

typedef enum sem_scope_chg_type /* Semantic context change type */
{
   sem_scope_global_to_local = 1,
   sem_scope_local_to_global,
   sem_scope_register,
   sem_scope_register_end
}sem_scope_chg_type;

typedef enum sem_error_type
{
   sem_error_ident_ja_declarado = 1,
   sem_error_tipo_nao_declarado,
   sem_error_ident_nao_declarado,
   sem_error_incomp_de_parametros,
   sem_error_atrib_nao_compativel,
   sem_error_retorne_nao_permitido
}sem_error_type;

typedef enum sem_pt_type
{
   sem_pt_type_none = 0,
   sem_pt_type_pointer,
   sem_pt_type_var,
   sem_pt_type_pointer_var
}sem_pt_type;

typedef struct sem_entry
{
   char*          string;
   sem_category   category;
   char*          type;
   sem_pt_type    pointer;
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

stack*      sem_context_stack;



void        sem_error            (sem_error_type error);
int         sem_compare_keys     (const void* key1, const void* key2);

void        sem_init             (void);

sem_entry*  sem_entry_get        (void);
sem_entry*  sem_entry_clone      (sem_entry* entry);
void        sem_entry_dispose    (sem_entry* entry);

sem_table*  sem_table_get        (void);

int         sem_table_insert     (sem_table* table, sem_entry* entry);
int         sem_table_remove     (sem_table* table, char* string);
int         sem_table_find       (sem_table* table, char* string);
void        sem_table_dispose    (sem_table* table);


int         sem_pending_insert   (char* string, sem_category category);
void        sem_pending_update   (sem_pending_upd_type upd, void* value);
void        sem_pending_commit   (void);
void        sem_context_change   (sem_scope_chg_type type);


#endif
