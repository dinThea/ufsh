#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commands/commands.h"

const char *FUNCTIONS[NUM_OPS] = {"CT", "RT", "AT", "LT", "IR", "BR", "AR", "RR"};
const int NUM_PARAMS[NUM_OPS] = {3, 2, 2, 1, 3, 4, 4, 2};
const int (*functions[NUM_OPS]) (char *result, int num_args, char * argv[]) = {CT, RT, AT, LT, IR, BR, AR, RR};


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

int CT(char *result, int num_args, char *argv[]) {
//  CT CLIENTES INT:CODIGO;STR:NOME;BIN:CETIF  -- exemplo de entrada 
//  comando nome_Da_Tabel campos
    printf ("Criando tabela: %s\n", argv[1]);
    printf ("Com os campos: \n");
   
    result_Fields(argv[2], result);

}

int AT(char *result, int num_args, char * argv[]) {
    
    return 0;
}
int LT(char *result, int num_args, char * argv[]) {
    return 0;
}

//Manipulaçao de resgistros nas tabelas
int IR(char *result, int num_args, char * argv[]) {
    //Exemplo: IR CLIENTES 10;JONATHAN;jonathan.crt
    printf("Inserindo Registro na tabela: %s \n", argv[1]);

    char *registros[30];

    char *delim = ";";
    int registro_index = token(argv[2], registros, delim);


    printf("Com os Registros: \n");

    for(int i = 0; i < registro_index; i++){
        printf("%s", registros[i]);
        if( i < registro_index - 1)
            printf("\n");
    }


    
    return 0;
}
int BR(char *result, int num_args, char * argv[]) {
    return 0;
}
int AR(char *result, int num_args, char * argv[]) {
    return 0;
}
int RR(char *result, int num_args, char * argv[]) {
    return 0;
}

int interpreter(char *result, int num_args, char *argv[], int func_index) {

    if (num_args > NUM_PARAMS[func_index]) {
        strcpy(result, "muitos argumentos");
    } else if (num_args < NUM_PARAMS[func_index]) {
        strcpy(result, "poucos argumentos");
    } else {
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


int token(char *argv, char *fields[MAX_FIELDS], char *delim){
    
    // char *delim = ";"; // delimitar a string
    char *pch;
    int field_index = 0; 

    /*  index 0 : CT
        index 1 nome da tabelaCT CLIENTES INT:CODIGO;STR:NOME;BIN:CETIF
        index 2 
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


void result_Fields(char argv[], char *result){
    char *fields[30];
    char *delim = ";";
    int field_index = token(argv, fields, delim);

    for (int i = 0; i < field_index; i++) {
        printf ("%s", fields[i]);
        if( i < field_index - 1) 
            printf("\n");
    }

    // fputc('\n', stdout); // ?

    strcpy(result, " ");

}