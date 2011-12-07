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


/*
 * O módulo 'semant' define as estruturas e operadores para
 *  armazenar e consultar informações semânticas sendo processadas.
 *
 * A principal estrutura semântica é a sem_entry, que é a menor unidade
 *  de armazenamento semântico. A tabela de símbolos, sem_table, é a
 *  estrutura mais importante do analisador semântico. É nessas tabelas
 *  que são armazenadas todas as informações semânticas relevantes de
 *  um contexto específico.
 *
 */



/*
 * Definição de novos tipos utilizados no analisador semântico
 *
 * Nota: alguns enum's começam com o valor 1 pois o gcc, em alguns
 *  casos, inicializa variáveis com 0. Caso nós negligenciemos a
 *  mudança deste 0 para um valor de enum válido, o gcc nos avisará
 *
 */


/*
   Este tipo define os valores que o campo "categoria" (category)
    da entrada semântica pode assumir.
*/
typedef enum sem_category
{
   variable = 1,
   type_def,
   procedure,
   function,
   constant

}sem_category;

/*
   Este tipo é usado como seletor para a função sem_pending_update().
   Esta função pode realizar várias ações, explicadas no seu código,
    dependendo do valor selecionado pelo tipo abaixo.
*/
typedef enum sem_pending_upd_type
{
   sem_upd_type = 1,    /* Atualização de tipo de variável                             */
   sem_upd_pointer,     /* Modifica a informação sobre ponteiro na entrada semântica   */
   sem_upd_more_info,   /* Atualiza o campo more_info da entrada semântica             */
   sem_upd_param_insert,/* Inserção de parâmetro de função/procedimento                */
   sem_upd_param_update /* Atualização do tipo do parâmetro de função/procedimento     */

}sem_pending_upd_type;

/*
   O tipo abaixo funciona como um seletor de ação para a função
    sem_context_change().
*/

typedef enum sem_scope_chg_type /* Semantic context change type */
{
   sem_scope_global_to_local = 1,   /* Muda o contexto de global para local                     */
   sem_scope_local_to_global,       /*                    local para global                     */
   sem_scope_register_insert,       /* Modifica o contexto para se inserir um registro          */
   sem_scope_register_query,        /* Modifica o contexto para se consultar campos de registro */
   sem_scope_register_end,          /* Retorna o contexto de registro (consulta/inserção) para o
                                          contexto anterior                                     */
   sem_scope_allows_return,         /* Indica que o contexto atual permite o uso de 'retorne'   */
   sem_scope_forbids_return         /* Indica que o contexto atual não permite o 'retorne'      */
}sem_scope_chg_type;

/*
   Seletor de tipo de erro, de acordo com a especificação da professora.
*/

typedef enum sem_error_type
{
   sem_error_ident_ja_declarado = 1,
   sem_error_tipo_nao_declarado,
   sem_error_ident_nao_declarado,
   sem_error_incomp_de_parametros,
   sem_error_atrib_nao_compativel,
   sem_error_retorne_nao_permitido
}sem_error_type;

/*
   Este enum determina se a entrada em questão tem associada os valores
   'var' e '^'. A ordem destes valores importa:

                  valor     binário
      none          0          00
      pointer       1          01
      var           2          10
      pointer_var   3          11

   Faz-se um OR bit a bit para atribuir um valor desses a uma variável.
   Assim, se a variável era none (00), e ela tem o atributo 'var' (10), faz-se:

      00 OR 10     resultando em     10 (var)

   Caso, posteriormente, descubra-se que ela tem '^' também:

      10 OR 01     resultando em     11 (var e ^)
*/

typedef enum sem_pt_type
{
   sem_pt_type_none = 0,
   sem_pt_type_pointer,
   sem_pt_type_var,
   sem_pt_type_pointer_var
}sem_pt_type;

/*
   Seletor de ação para a função sem_check(), que realiza algumas checagens
    pertinentes
*/
typedef enum sem_check_type
{
   sem_check_type_declared = 1,  /* Checa se o tipo já foi declarado       */
   sem_check_variable_declared,  /* Checa se a variável já foi declarada   */
   sem_check_var_const_declared, /* Checa se uma variável ou uma constante
                                     com o nome dado já foi declarada      */
   sem_check_proc_func_declared, /* Checa se há uma função ou procedimento
                                     já foi declarado */
   sem_check_any_declared,       /* Checa se há qualquer coisa declarada com
                                     o identificador especificado          */
   sem_check_return_allowed,     /* Checa se o contexto atual permite o uso
                                     do comando 'retorne'                  */
   sem_check_attr                /* Checa se o tipo de atribuição ou passagem
                                     de parâmetros está correto            */

}sem_check_type;

/*
   Principal unidade do analisador semântico. Define uma entrada da tabela de símbolos
*/
typedef struct sem_entry
{
   char*          string;        /* Identificador da entrada                           */
   sem_category   category;      /* Categoria (variável, procedimento, registro...     */
   char*          type;          /* Tipo da entrada (inteiro, real, literal...         */
   sem_pt_type    pointer;       /* Armazena informações sobre se é '^' e/ou 'var'     */
   list*          param_list;    /* Lista de parâmetros de função/procedimento         */
   void*          more_info;     /* Ponteiro para uma tabela semântica, contendo mais
                                     informações para campos de registro.              */

   /*
      Nota: more_info somente aponta para tabelas semânticas. Em versões futuras, deve-se
        substituir o tipo void* por sem_table*
   */
}sem_entry;

/*
   Principal estrutura do analisador semântico. Esta tabela de símbolos tem informações de
    um contexto específico.

   Ela tem três partes:

      table:            É uma árvore B que contém todas as entradas do seu contexto. Esta
                         é a estrutura de armazenamento principal dos elementos semânticos.

      pending_stack:    Pilha de entradas pendentes. Estas entradas têm identificador e categoria,
                         mas carecem de tipo. Assim, elas vão sendo empilhadas até que se encontre
                         o seu tipo. Posteriormente, ao encontrar o tipo, elas são desempilhadas.

                        O mecanismo da tabela é explicado em semant.c

      pending_changes:  Armazena as mudanças pendentes, que devem ser aplicadas à pilha de entradas
                         pendentes.

*/
typedef struct sem_table
{
   btree*      table;
   stack*      pending_stack;
   sem_entry*  pending_changes;
}sem_table;

/*
   Aqui é declarada a estrutura que é acessível globalmente. Seus campos são explicados:
*/
struct
{
   sem_table*  global_table;        /* Tabela semântica global                                     */
   sem_table*  local_table;         /* Tabela semântica local                                      */

   sem_table*  current_table;       /* Ponteiro para o contexto atual                              */

   stack*      context_stack;       /* Pilha de contexto, usada para entrar e sair do
                                        contexto dos registros                                     */

   char*       register_ident;      /* Esta string armazena a junção de vários tokens,
                                        por exemplo, "^a.b.c". Ela é usada para se imprimir
                                        corretamente os erros                                      */

   char*       context_return_type; /* Este campo tem propósito duplo: se estiver vazio (NULL)
                                        indica que o contexto atual não permite 'retorne'. Caso
                                        contrário, armazena o tipo que será retornado pelo bloco
                                        através do 'retorne'.                                      */

   stack*      attrib_stack;        /* Pilha utilizada para checagem de tipos (atribuição/passagem
                                        de parâmetros. Seu uso é descrito em semant.c.             */

   char*       attrib_temp;         /* String que armazena temporariamente um tipo, que será inserido
                                        na attrib_stack. Ele é usado para quando se entra num
                                        registro. Por exemplo, quando tenta-se determinar o tipo de:

                                        a.b.c

                                       Primeiro coloca-se o tipo de 'a' em attrib_temp. Então
                                        sobrescreve-se attrib_temp com o tipo de 'b'. Por fim,
                                        sobrescreve-se novamente attrib_temp para o tipo de c, que
                                        é, de fato, o tipo da expressão 'a.b.c'.

                                       Depois, pode-se empilhar attrib_temp em attrib_stack        */


}sem_gl_info;


/* Imprime o erro que ocorreu.

      Entradas:   error          Tipo do erro ocorrido (especificados pela professora)
                  str            Informação sobre o erro, como, por exemplo o token relacionado

      Saída:      Imprime a mensagem de erro.
*/

void        sem_error            (sem_error_type error, char* str);

/* Compara duas chaves de tabela. Esta função é passada para a árvore B,
    que aceita qualquer tipo de chave

   Entradas:      key1, key2     Chaves a serem comparadas

   Saída:         <0 caso key1 <  key2
                   0 caso key1 == key2
                  >0 caso key1 >  key2

*/
int         sem_compare_keys     (const void* key1, const void* key2);

/* Inicializa as estruturas globais do analisador semântico                   */
void        sem_init             (void);

/* Insere funções básicas na tabela semântica global (sen, cos, subLiteral...)
    É chamada por sem_init()                                                  */
void        sem_insert_basic_functions    (void);
/*
 * Conjunto de funções que manipulam uma estrutura do tipo sem_entry
 *
 */

/* Aloca e retorna uma nova sem_entry, com seus valores setados para o padrão */
sem_entry*  sem_entry_get        (void);

/* Aloca uma nova sem_entry e copia os valores de uma entrada dada            */
sem_entry*  sem_entry_clone      (sem_entry* entry);

/* Desaloca uma sem_entry passada. Esta função é passada para a árvore B      */
void        sem_entry_dispose    (void* e);

/*
 * Conjunto de funções que manipula uma estrutura sem_table
 *
 */

/* Aloca e retorna uma nova tabela de símbolos                                */
sem_table*  sem_table_get        (void);

/* Insere uma entrada na tabela de símbolos                                   */
int         sem_table_insert     (sem_table* table, sem_entry* entry);

/* Remove uma entrada da tabela de símbolos através da chave passada.         */
int         sem_table_remove     (sem_table* table, char* string);

/* Desaloca uma tabela de símbolos                                            */
void        sem_table_dispose    (sem_table* table);

/*
 * Conjunto de funções que manipula a pilha dentro da tabela de uma tabela de
 *  símbolos (pending_stack)
 *
 */

/* Insere uma entrada incompleta na pilha da tabela de símbolos.              */
int         sem_pending_insert   (char* string, sem_category category);

/* Atualiza um valor das entradas incompletas na pilha da tabela de símbolos  */
void        sem_pending_update   (sem_pending_upd_type upd, void* value);

/* Aplica todas as alterações feitas previamente, desempilhando as entradas   */
void        sem_pending_commit   (void);

/* Muda de contexto. São realizadas várias ações, exlpicadas em semant.c      */
void        sem_context_change   (sem_scope_chg_type type);



/* Realiza uma checagem específica em uma entrada da tabela de símbolos. São
    possíveis várias checagens, explicadas na implementação de sem_check, no
    arquivo semant.c                                                          */
int         sem_check            (sem_check_type check, char* key);

/*
 * Conjunto de funções que manipulam a string de armazenamento temporário
 *  register_ident. Esta variável global serve para armazenar o identificador
 *  completo de uma entrada, por exemplo, ^a.b.c[4].
 *
 */

/* Sobrescreve o valor anterior de register_ident com o valor passado por str */
void        sem_register_ident_set     (char* str);

/* Concatena à variavel register_ident o valor passado por str                */
void        sem_register_ident_append  (char* str);

/* Limpa o valor de register_ident:   register_ident = ""                     */
void        sem_register_ident_clear   (void);



/*
 * Conjunto de funções que manipula a pilha global de tipos. Esta pilha serve
 *  para avaliar expressões, em que os tipos de suas partes são empilhados para
 *  armazenagem e desempilhados para checagem de consistência.
 *
 */


/* sem-attrib_set

   Sobrescreve o valor da variável global attrib_temp. Isto é feito ao se
    'entrar' dentro de registros, por exemplo:

      a.b.c

   Neste caso, sem_attrib_set é chamado três vezes. Na primeira vez:

      attrib_temp = type(a)

   Depois:

      attrib_temp = type(b)    sobrescreve

   Por fim:

      attrib_temp = type(c)    sobrescreve

   No final, attrib_temp conterá o tipo do c, que é o tipo verdadeiro da
    expressão a.b.c

   Depois deste procedimento, este valor poderá ser empilhado com
    sem_attrib_push().
*/
void        sem_attrib_set       (char* value);

/* Empilha o tipo passado na pilha attrib_stack                               */
void        sem_attrib_push      (char* type);

/* Desempilha e retorna o tipo no topo da pilha attrib_stack                  */
char*       sem_attrib_pop       (void);

/* Retorna o tipo no topo da pilha attrib_stack                               */
char*       sem_attrib_peek      (void);

/* Força o tipo do topo da pilha de tipos (attrib_stack) para conter o tipo
    passado em 'type'. Caso o topo seja igual à 'type', não faz nada.
    Caso contrário, muda o topo para 'tipo_indefinido'.
*/
void        sem_attrib_enforce_top_type   (char* type);


/* Esta função desempilha dois valores da pilha de tipos, [pop]
   checa se os dois tipos são compatíveis e               [check]
   empilha o resultado da comparação.                     [push]

   Se os dois tipos desempilhados são compatíveis, empilha um deles de volta.
   Caso contrário, empilha 'tipo_indefinido'
*/
int         sem_pop_check_push            (char* type);

/* Função auxiliar que busca uma entrada nas tabelas semânticas apropriadas
    para o contexto corrente e retorna o tipo desta entrada                   */
char*       sem_type_of          (char* key);

/* Função auxiliar que busca uma entrada na tabela semântica global e retorna
    a lista de parâmetros desta entrada (procedimento/função                  */
list*       sem_list_of          (char* key);

#endif
