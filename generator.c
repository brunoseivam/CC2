#include "generator.h"

/*Função para inicializar a estrutura gen_gl_info */
void gen_init_info(void)
{
	gen_gl_info.is_type_def = 0;
	gen_gl_info.is_scanf = 0;
	gen_gl_info.waiting_exp = 0;
	gen_gl_info.aux_ident = NULL;
	gen_gl_info.aux_list = NULL;
}

/* Função para colocar uma string em gen_gl_info.aux_ident */
void gen_set_aux_ident(char* str)
{
	gen_gl_info.aux_ident = (char*)malloc((1+strlen(str))*sizeof(char));
	strcpy(gen_gl_info.aux_ident,str);
}

void* gen_find(sem_table* table,char* key);
{
    sem_entry* aux_entry;
	if((aux_entry = sem_table_find(sem_gl_info.current_table, key)) == NULL)
		aux_entry = sem_table_find(sem_gl_info.global_table, key)
	return gen_aux_entry;
}

/* Funcao para imprimir a tabulação no arquivo	*/
void gen_print_tab()
{
	int i;
	for(i=0;i<tab;i++)
	{
		printf("\t");
	}
}

/* Função para impressão genérica de código */
void gen_print(print_category cat,int tabd,void* text)
{

	if(gen_flag)
	{
		if(tabd)
			gen_print_tab();

		if(cat == pr_string)
        	    	printf("%s",(char*)text);
        	else if(cat == pr_float)
        		printf("%f",*((float*)text));
	        else if(cat == pr_int)
        		printf("%d",*((int*)text));
	}
}

/*Includes*/
void generator_init()
{

}

/*Declaracão de constante*/
void gen_const_decl(sem_table* cur,char * value)
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

void gen_function(void)
{
	int i;
	sem_entry* temp_entry = (sem_entry*)stack_peek(sem_gl_info->current_table->pending_stack);

	if(temp_entry->type == SEM_TYPE_UNDEFINED)
		printf("void %s(",temp_entry->string);
	else
		printf("%s %s(",temp_entry->type,temp_entry->string);

	for(i = 0;i<(gen_gl_info.aux_list->size);++i)
	{
		temp_entry = (sem_entry*)list_elem_at(gen_gl_info.aux_list, i);
		printf("%s %s,",temp_entry->type,temp_entry->string);
	}

	temp_entry = (sem_entry*)list_elem_at(gen_gl_info.aux_list, end);
	printf("%s %s)",temp_entry->type,temp_entry->string);
}

void gen_scanf(void)
{
	int i,id_begin = 1;
	int bitmap = 0
	int marker = 1;
	list* temp_list = gen_gl_info.aux_list;
	sem_entry* temp_entry;

	printf("scanf(\"");
	for(i=1;i<temp_list->size;++i)
	{
		temp_entry = list_element_at(temp_list,i);
		if(strcmp(temp_entry->type,"mark"))
		{
			temp_entry = list_element_at(temp_list,i-1);

			if(temp_entry->is_pointer)
				bitmap = bitmap | marker;
			if(!strcmp(temp_entry->type,"literal"))
			{
				printf("%%s ");
				bitmap = bitmap | marker;
			}
			else if(!strcmp(temp_entry->type,"inteiro") || !strcmp(temp_entry->type,"logico"))
				printf("%%d ");
			else
				printf("%%f ");

			marker = marker << 1;
		}
	}

	marker = 1;
	printf("\",");
	for(i=0;i<temp_list->size;++i)
	{
		temp_entry = list_element_at(temp_list,i);
		if(id_begin)
		{
			id_begin = 0;
			if(!(marker & bitmap))
				printf("&(");

		}

		if(strcmp(temp_entry->type,"mark"))
		{
			id_begin = 1;
			if(!(marker & bitmap))
			{
				if(i != (temp_list->size - 1))
					printf("),");
				else
					printf(")");
			}
			marker = marker << 1;
		}
		else
		{
			printf("%s",temp_entry->string);
			if(strcmp((list_element_at(temp_list,i))->type,"mark"))
			{
				if(tem_entry->is_pointer)
					printf("->");
				else
					printf(".");
			}
		}
	}

	printf(");\n");
}
