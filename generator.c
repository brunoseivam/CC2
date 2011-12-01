#include "generator.h"


/*Includes*/
void generator_init()
{

}

/*Comeco da main */
void gen_main_begin()
{
    printf("int main( )\
            \n{\n");

    return;
}

/*Fim da main*/
void gen_main_end()
{
    printf("\n}");
}

/*Declaracão de constante*/
void gen_const(sem_table* cur,char * value)
{

    printf("\tconst ");

    if(!strcmp(cur->pending_changes->type,"literal"))
        printf("char* ");
    else if(!strcmp(cur->pending_changes->type,"real"))
        printf("float ");
    else
        printf("int ");

    printf("%s = ", ((sem_entry*) stack_peek(cur->pending_stack)) -> string);

    printf("%s; \n",value);

    return;

}

/*Declaracao de variavel(is)*/
void gen_variable(sem_table* cur)
{

    stack* temp_stack = stack_get();
    sem_entry* temp_sem_entry;

    if(!strcmp(cur->pending_changes->type,"literal"))
        printf("\tchar* ");
    else if(!strcmp(cur->pending_changes->type,"real"))
        printf("\tfloat ");
    else if(!strcmp(cur->pending_changes->type,"inteiro") || !strcmp(cur->pending_changes->type,"logico"))
        printf("\tint ");
    else if(strcmp(cur->pending_changes->type,"registro"))
        printf("\t%s ",cur->pending_changes->type);
    else
    {
        /*Caso em que a:registro
                            --
                      fim_registro */

    }

    /* Empilha em uma pilha auxiliar temp_stack para ler os nomes das variáveis */
    while((temp_sem_entry = (sem_entry*)stack_pop(cur->pending_stack)))
      stack_push(temp_stack,temp_sem_entry);

    /*Imprime os nomes das variáveis */
    temp_sem_entry = (sem_entry*)stack_pop(temp_stack);
    printf("%s",temp_sem_entry -> string);
    stack_push(cur->pending_stack,temp_sem_entry);
    temp_sem_entry = (sem_entry*)stack_pop(temp_stack);
    while(temp_sem_entry)
    {
        stack_push(cur->pending_stack,temp_sem_entry);
        printf(", %s",temp_sem_entry -> string);
        temp_sem_entry = (sem_entry*)stack_pop(temp_stack);
    }

    printf(";\n");

     /* Libera stack temporaria*/
     stack_dispose(temp_stack);
 

}