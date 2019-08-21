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
        
        fgets(line, 500, stdin);
        int param_index = 0;
        
        char *pch;  
        pch = strtok(line, delim);
        
        while (pch != NULL) {

            if (pch[strlen(pch)-1] == '\n') pch[strlen(pch)-1] = '\0';
            params[param_index] = malloc(sizeof(char)*(strlen(pch)+1));
            params[param_index][strlen(pch)] = '\0';
            strcpy(params[param_index], pch);
            param_index++;
            pch = strtok(NULL, delim);
        }


        free(pch);

        if (param_index != 0) {
            
            char result[500];

            if (!strcmp(params[0], "EB")){

                control = 1;

            } else if (!strcmp(params[0], "RT")) {

                RT(result, param_index, params);

            }
            else if (!strcmp(params[0], "CT")) {
                
                CT(result, param_index, params);

            } 
            else if (!strcmp(params[0], "IR")) {

                // IR(result, param_index, params);
                

            } else if (!strcmp(params[0], "RR")) {

                

            }

            printf ("%s\n", result);

            // fgetc(stdin);

        }

    }
    // printf ("%d\n", argc);
    // printf ("%s\n", argv[1]);
    return 0;

}