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


#define  SEM_TYPE_NUMBER            "number"
#define  SEM_TYPE_UNDEFINED         "tipo_indefinido"


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
   sem_scope_register_insert,
   sem_scope_register_query,
   sem_scope_register_end,
   sem_scope_allows_return,
   sem_scope_forbids_return
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

typedef enum sem_check_type
{
   sem_check_type_declared = 1,
   sem_check_variable_declared,
   sem_check_var_const_declared,
   sem_check_proc_func_declared,
   sem_check_any_declared,
   sem_check_return_allowed,
   sem_check_attr

}sem_check_type;

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

/* Global semantic information */
struct
{
   sem_table*  global_table;
   sem_table*  local_table;

   sem_table*  current_table;

   stack*      context_stack;

   char*       register_ident;

   char*       context_return_type;

   stack*      attrib_stack;
   char*       attrib_temp;

   char*       attrib_expected;
   char*       attrib_got;
}sem_gl_info;



void        sem_error            (sem_error_type error, char* str);
int         sem_compare_keys     (const void* key1, const void* key2);

void        sem_init             (void);

sem_entry*  sem_entry_get        (void);
sem_entry*  sem_entry_clone      (sem_entry* entry);
void        sem_entry_dispose    (void* e);

sem_table*  sem_table_get        (void);

int         sem_table_insert     (sem_table* table, sem_entry* entry);
int         sem_table_remove     (sem_table* table, char* string);
void        sem_table_dispose    (sem_table* table);


int         sem_pending_insert   (char* string, sem_category category);
void        sem_pending_update   (sem_pending_upd_type upd, void* value);
void        sem_pending_commit   (void);
void        sem_context_change   (sem_scope_chg_type type);

int         sem_check            (sem_check_type check, char* key);

void        sem_register_ident_set     (char* str);
void        sem_register_ident_append  (char* str);
void        sem_register_ident_clear   (void);

void        sem_attrib_set       (char* value);
void        sem_attrib_push      (char* type);
char*       sem_attrib_pop       (void);
char*       sem_attrib_peek      (void);

int         sem_pop_check_push            (char* type);
void        sem_attrib_enforce_top_type   (char* type);

char*       sem_type_of          (char* key);
list*       sem_list_of          (char* key);

/* ?? */
void        sem_attrib_update    (char* value);


#endif
