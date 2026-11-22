/*
 * Problema 1
 * Realizati un interpretor de comenzi propriu:
 *   - recunoaste comenzile DIR, TIME, DATE, COMANDA, ECOU;
 *   - DIR, TIME, DATE sunt transmise interpretorului sistemului (cmd);
 *   - COMANDA lanseaza interpretorul sistemului pana se tasteaza EXIT;
 *   - ECOU afiseaza toate argumentele primite;
 *   - orice alta comanda e lansata ca program (spawnvp: directorul
 *     curent, apoi PATH); daca nu reuseste se afiseaza "Comanda incorecta";
 *   - separatorul dintre cuvinte este doar spatiul.
 *
 * Utilizare: ex01
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(void)
{
    char linie[MAX_LINE];
    char copie_linie[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("Prompt> ");
        if (fgets(linie, sizeof(linie), stdin) == NULL) {
            break;
        }

        linie[strcspn(linie, "\r\n")] = '\0';

        if (strlen(linie) == 0) {
            continue;
        }

        strcpy(copie_linie, linie);

        int argc = 0;
        char *token = strtok(linie, " ");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        if (strcmp(args[0], "DIR") == 0 || 
            strcmp(args[0], "TIME") == 0 || 
            strcmp(args[0], "DATE") == 0) {
            
            system(copie_linie);
        } 
        else if (strcmp(args[0], "COMANDA") == 0) {
            system("cmd");
        } 
        else if (strcmp(args[0], "ECOU") == 0) {
            for (int i = 1; i < argc; i++) {
                printf("%s ", args[i]);
            }
            printf("\n");
        } 
        else {
            if (spawnvp(P_WAIT, args[0], (const char * const *)args) == -1) {
                printf("Comanda incorecta\n");
            }
        }
    }

    return 0;
}
