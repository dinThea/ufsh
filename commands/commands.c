/*
@BRIEF: AÇÕES DOS COMANDOS QUE O INTERPRETADOR RECONHECE
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commands/commands.h"

const char *FUNCTIONS[NUM_OPS] = {"CT", "RT", "AT", "LT", "IR", "BR", "AR", "RR", "CI", "RI", "GI"};
const int NUM_PARAMS[NUM_OPS] = {3, 2, 2, 1, 3, 4, 2, 2, 4, 3, 3}; //quantidade mínima de parâmetros que cada comando deve ter(CT, RT, AT, LT, IR, BR, AR, RR)
const int NUM_FIELDS[NUM_OPS] = {2, 1, 0, 0, 1, 2, 1, 1, 0, 0, 0}; //quantidade mínima de campos que cada comando deve ter
const int (*functions[NUM_OPS]) (char *result, int num_args, char * argv[]) = {CT, RT, AT, LT, IR, BR, AR, RR, CI, RI, GI};

/*
@BRIEF: Apaga o arquivo relativo a tabela e remove os dados de sua metabase
@param  {result}    Resultado da operação
@param  {num_args}  numero de argumentos que o interpretador reconhece}
@param {argv}   armazenamento dos argumentes reconhecidos pelo interpretador
*/
int RT(char *result, int num_args, char *argv[]) {
    //RT nome_Da_Tabela
    printf ("Deletando tabela %s\n", argv[1]);

    int res = 0;

    // operation

    if (!res) {
        strcpy(result, "Deletado com sucesso");
    } else {
        strcpy(result, "Falha deletando a tabela ");            
    }

    return 0;

}
/*
@BRIEF: Cria um arquivo vazio
@param  {result}    Resultado da operação
@param  {num_args}  numero de argumentos que o interpretador reconhece}
@param {argv}   armazenamento dos argumentes reconhecidos pelo interpretador
*/
int CT(char *result, int num_args, char *argv[]) {
//  CT CLIENTES INT:CODIGO;STR:NOME;BIN:CETIF  -- exemplo de entrada 
//  comando nome_Da_Tabel campos
    printf ("Criando tabela: %s\n", argv[1]);
    printf ("Com os campos: \n");
   
    result_Fields(argv[2], result);

    return 0;
}
/*
@BRIEF: Apresenta um resumo dos metadados da tabela indicada: arquivos, campos e índices existentens.
@param  {result}    Resultado da operação
@param  {num_args}  numero de argumentos que o interpretador reconhece}
@param {argv}   armazenamento dos argumentes reconhecidos pelo interpretador
*/
int AT(char *result, int num_args, char * argv[]) {
    printf("RESUMO DOS METADADOS: ");
    // result_Fields(argv[1], result);

    return 0;
}
/*
@BRIEF: Lista o nome de todas as tabelas existentes na base
@param  {result}    Resultado da operação
@param  {num_args}  numero de argumentos que o interpretador reconhece}
@param {argv}   armazenamento dos argumentes reconhecidos pelo interpretador
*/
int LT(char *result, int num_args, char * argv[]) {

    printf("Lista das tabelas criadas: ");
    return 0;
}

/*
@BRIEF: Insere um registro no arquivo de uma tabela já criada
@param  {result}    Resultado da operação
@param  {num_args}  numero de argumentos que o interpretador reconhece}
@param {argv}   armazenamento dos argumentes reconhecidos pelo interpretador
*/
int IR(char *result, int num_args, char * argv[]) {

    //Exemplo: IR CLIENTES 10;JONATHAN;jonathan.crt
    printf("Inserindo Registro na tabela: %s \n", argv[1]);
    printf("Com os Registros: \n");

    result_Fields(argv[2], result);
    return 0;

}
int BR(char *result, int num_args, char * argv[]) {
    if(!strcmp(argv[1], "N"))
        printf("Registros da tabela %s", argv[2]); //BR N TABELA BUSCA
    else if(!strcmp(argv[1], "U"))
        printf("Busca em %s pelo critério %s ", argv[2], argv[3]); // Exemplo: BR U TABELA BUSCA
    else
        printf("syntax error");
    return 0;
}
int AR(char *result, int num_args, char * argv[]) {
    printf("Registros da última busca: ");
    return 0;
}
int RR(char *result, int num_args, char * argv[]) {
    printf("Tabela %s removida!", argv[1]);
    return 0;
}

int CI(char *result, int num_args, char * argv[]){
     if(!strcmp(argv[1], "A")) //CI H CLIENTES CODIGO
        printf("OK"); 
    else if(!strcmp(argv[1], "H"))
        printf("OK");
    else
        printf("syntax error");
    return 0;
}

int RI(char *result, int num_args, char * argv[]){
    //RI NOMETABELA CHAVE
    printf("Indice com chave %s da tabela %s removido!!", argv[2],argv[1]);
    return 0;
}
int GI(char *result, int num_args, char * argv[]){
    //GI NOMETABELA CHAVE
    printf("Indice com chave %s gerado!", argv[2]);
    return 0;
}
/*
@BRIEF  Verifica a quantidade de parâmetros, se forem válidos chama a função que correspode ao primeiro comando lido(CT, RT, AT ...)
@param      {resul}     Resultado da análise
@param      {num_args}  Número de argumentos lidos
@param      {argv}      Conteúdo de entrada
@param      {func_index}    indice da função 
*/
int interpreter(char *result, int num_args, char *argv[], int func_index) {
    
    char *last_param;
    if (num_args > NUM_PARAMS[func_index]) {
        
        int size = 0;
        for (int i = NUM_PARAMS[func_index]-1; i < num_args; i++) size+=strlen(argv[i]); 
        last_param = (char *) malloc(sizeof(char)*(size+1));
        for (int i = NUM_PARAMS[func_index]-1; i < num_args; i++) {
            strcat(last_param, argv[i]);
        }
        argv[NUM_PARAMS[func_index]-1] = (char *) malloc(sizeof(char)*(size+1));
        strcpy (argv[NUM_PARAMS[func_index]-1], last_param);
        free(last_param);
        printf ("\nLast param: %s\n", argv[NUM_PARAMS[func_index]-1]);
        strcpy(result, " ");

        (*functions[func_index]) (result, num_args, argv);
        
    } else if (num_args < NUM_PARAMS[func_index]) {
        strcpy(result, "número inválido de argumentos");
    } else if (!valid_Fields(argv[num_args - 1], NUM_FIELDS[func_index])) {
        strcpy(result,"syntax Error");
    } else {
        strcpy(result, " ");

        (*functions[func_index]) (result, num_args, argv);

    }

    return 0;

}

int check_functions(char *op) {

    for (int i = 0; i < NUM_OPS; i++) {
        if (!strcmp(op, FUNCTIONS[i])) {
            return i;    
        }
    }
    
    return -1;

}

/*
@BRIEF  Separa a string de entrada com o delimitador passado por parâmetro
@param  {argv}      string que vai ser interpretada e delimitada
@param  {fields}    alocação dos campos ou registros lidos em uma matriz de strings
@param  {delim}     delimitador da string
@return retorna o número de campos ou registros encontrados em {fields}
*/
int token(char *argv, char *fields[MAX_FIELDS], char *delim){
    
    char *pch;
    int field_index = 0; 

    /*
        CT CLIENTES INT:CODIGO;STR:NOME;BIN:CETIF  
        index 0 : CT
        index 1 : CLIENTES
        index 2 : INT:CODIGO;STR:NOME;BIN:CETIF
    */

    pch = strtok(argv, delim);
    
    while (pch != NULL) {

        fields[field_index] = malloc(sizeof(char)*strlen(pch));
        strcpy(fields[field_index], pch);
        field_index++;
        pch = strtok(NULL, delim);

    }  

    free(pch);

    return field_index;

}

/*
@BRIEF IMPRIME OS CAMPOS OU REGISTROS QUE FORAM VALIDADES PELO INTERPRETADOR 
@param      {argv}      string de campo ou registro lido
@param      {result}    resultado da validação feita
*/
void result_Fields(char argv[], char *result){
    char *fields[30];// armazenamento dos campos ou registros lidos
    char *delim = ";"; //delimitador da string de entrada
    int field_index = token(argv, fields, delim);

    for (int i = 0; i < field_index; i++) {
        printf ("%s", fields[i]);
        if( i < field_index - 1) 
            printf("\n");
    }

    // fputc('\n', stdout); // ?

    strcpy(result, " ");

}
/*
@BRIEF  verifa se a quantidade de campos ou registros é valida para determinado comando( EX: CT tabela campo -- CT  NOVATABELA NOME:JONATHAN)  o mínimo é que tenha 2 campos separados por ":"
@param      {argv}    string que vai ser verificada 
@param      {num_Index}     Número mínimo de indices para a determinada tarefa(EX: CT- min 2 ; RT - min 1)
*/
int valid_Fields(char * argv, int num_Index){
    char *fields[50];
    char str_Aux[50];
    strcpy(str_Aux, argv);

    int index = token(str_Aux, fields, ":");
    if (index < num_Index)
        return 0;
    return 1;
}
