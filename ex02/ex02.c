/*
 * Problema 2
 * Imbunatatiti interpretorul de la problema 1 astfel incat sa recunoasca
 * < si > ca redirectari ale intrarii si iesirii, realizand efectiv
 * redirectarile.
 *   Exemple:  ECOU salut > a.txt
 *             sort < nume.txt
 *
 * Utilizare: ex02
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>


#define MAX_LINE 1024
#define MAX_ARGS 64

int main(void)
{
    /* Scrieti solutia aici */
    char linie[MAX_LINE];
    char cmd_curata[MAX_LINE];
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

        
        char *raw_args[MAX_ARGS];
        int raw_argc = 0;
        char *token = strtok(linie, " ");
        while (token != NULL && raw_argc < MAX_ARGS - 1) {
            raw_args[raw_argc++] = token;
            token = strtok(NULL, " ");
        }

        if (raw_argc == 0) {
            continue;
        }

        
        char *fisier_in = NULL;
        char *fisier_out = NULL;
        int argc = 0;
        int eroare_sintaxa = 0;

        for (int i = 0; i < raw_argc; i++) {
            if (strcmp(raw_args[i], "<") == 0) {
                if (i + 1 < raw_argc) {
                    fisier_in = raw_args[++i]; 
                } else {
                    printf("Eroare: lipseste fisierul pentru redirectarea intrarii (<)\n");
                    eroare_sintaxa = 1;
                    break;
                }
            } 
            else if (strcmp(raw_args[i], ">") == 0) {
                if (i + 1 < raw_argc) {
                    fisier_out = raw_args[++i]; 
                } else {
                    printf("Eroare: lipseste fisierul pentru redirectarea iesirii (>)\n");
                    eroare_sintaxa = 1;
                    break;
                }
            } 
            else {
                args[argc++] = raw_args[i];
            }
        }
        args[argc] = NULL;

        if (eroare_sintaxa || argc == 0) {
            continue;
        }

    
        int saved_stdin = _dup(0);
        int saved_stdout = _dup(1);
        FILE *f_in = NULL;
        FILE *f_out = NULL;
        int redirect_ok = 1;

        
        if (fisier_in != NULL) {
            f_in = fopen(fisier_in, "r");
            if (f_in == NULL) {
                printf("Eroare la deschiderea fisierului de intrare: %s\n", fisier_in);
                redirect_ok = 0;
            } else {
                _dup2(_fileno(f_in), 0);
            }
        }

        
        if (redirect_ok && fisier_out != NULL) {
            f_out = fopen(fisier_out, "w");
            if (f_out == NULL) {
                printf("Eroare la deschiderea fisierului de iesire: %s\n", fisier_out);
                redirect_ok = 0;
            } else {
                fflush(stdout); 
                _dup2(_fileno(f_out), 1); 
            }
        }

        
        if (redirect_ok) {
            if (strcmp(args[0], "DIR") == 0 || 
                strcmp(args[0], "TIME") == 0 || 
                strcmp(args[0], "DATE") == 0) {
                
                
                cmd_curata[0] = '\0';
                for (int i = 0; i < argc; i++) {
                    strcat(cmd_curata, args[i]);
                    if (i < argc - 1) strcat(cmd_curata, " ");
                }
                system(cmd_curata);
            } 
            else if (strcmp(args[0], "COMANDA") == 0) {
                system("cmd");
            } 
            else if (strcmp(args[0], "ECOU") == 0) {
                for (int i = 1; i < argc; i++) {
                    printf("%s", args[i]);
                    if (i < argc - 1) printf(" ");
                }
                printf("\n");
                fflush(stdout); 
            } 
            else {
                if (spawnvp(P_WAIT, args[0], (const char * const *)args) == -1) {
                    printf("Comanda incorecta\n");
                }
            }
        }

        
        fflush(stdout);
        if (f_in != NULL) {
            _dup2(saved_stdin, 0);
            fclose(f_in);
        }
        if (f_out != NULL) {
            _dup2(saved_stdout, 1);
            fclose(f_out);
        }
        _close(saved_stdin);
        _close(saved_stdout);
    }

    return 0;
}
