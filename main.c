#include "commands/commands.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>

int main(){

    int control = 0;
    char line[500];
    char *params[5];
    char *delim = " ";

    while (control == 0) {
        printf("[SHELL >$]");
        fgets(line, 500, stdin); //Lê os comandos no terminalW
        int param_index = 0;
        
        char *pch;
        pch = strtok(line, delim); //Encontra o primeiro delimitador (" ") na string {line}

        while (pch != NULL) {

            if (pch[strlen(pch)-1] == '\n') 
                pch[strlen(pch)-1] = '\0';
            
            params[param_index] = malloc(sizeof(char)*(strlen(pch)+1)); // Alocação dinâmica dos parâmetros contidos na string {pch}
            params[param_index][strlen(pch)] = '\0'; 
            strcpy(params[param_index], pch);
            param_index++;
            pch = strtok(NULL, delim);
            
        }
        

        free(pch);

        if (param_index != 0) {

            char result[500];
            int func_index = check_functions(params[0]);

            if (!strcmp(params[0], "EB")){

                strcpy(result, "saindo");
                control = 1;

            } else if (func_index != -1) {

                interpreter(result, param_index, params, func_index);

            } else if (!strcmp(params[0], "")) {
                
                strcpy(result, "");

            } else {                
                strcpy(result, "syntax error");
            }

            printf ("%s\n", result);
            if (!strcmp(result, "syntax error")){

                control = 1;

            }

            // fgetc(stdin);

        }

    }

    return 0;

}