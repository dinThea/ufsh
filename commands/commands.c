#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../commands/commands.h"

int RT(char *result, int num_args, char *argv[]) {

    if (num_args > 2) {
        strcpy(result, "muitos argumentos");
    } else if (num_args < 2) {
        strcpy(result, "faltando argumentos");
    } else {

        printf ("Deletando tabela %s", argv[1]);

        int res = 0;

        // operation

        if (!res) {
            strcpy(result, "Deletado com sucesso");
        } else {
            strcpy(result, "Falha deletando a tabela ");            
        }
    }

    return 0;

}

int CT(char *result, int num_args, char *argv[]) {

    if (num_args > 3) {
        strcpy(result, "muitos argumentos");
    } else if (num_args < 3) {
        strcpy(result, "faltando argumentos");
    } else {

        int res = 0;

        // operation
        printf ("Criando tabela %s\n", argv[1]);
        char *delim = ";";
        char *field[30];
        char *pch;
        int field_index;  

        /*  index 0 : CT
            index 1 nome da tabela
            index 2 
        */

        pch = strtok(argv[2], delim);
        
        while (pch != NULL) {

            field[field_index] = malloc(sizeof(char)*strlen(pch));
            strcpy(field[field_index], pch);
            field_index++;
            pch = strtok(NULL, delim);

        }  

        free(pch);

        printf ("Com os campos: \n");

        for (int i = 0; i < field_index; i++) {
            printf ("%s ", field[i]);
        }

        fputc('\n', stdout);

        strcpy(result, " ");

    }

    return 0;

}