#pragma once

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

bool MESSAGES = true;

void initialize_comand_line(char *comand_line, int size, char *str_arq){    
    comand_line[0] = 'g';
    comand_line[1] = 'c';
    comand_line[2] = 'c';
    comand_line[3] = ' ';

    int str_n = (int) strlen(str_arq);

    for(int i = 0; i < str_n; i++){
        comand_line[i + 4] = str_arq[i];
    }

    comand_line[str_n + 4] = ' ';
    comand_line[str_n + 5] = '-';
    comand_line[str_n + 6] = 'o';
    comand_line[str_n + 7] = ' ';

    for(int i = (str_n + 7 + 1), j = 0; i < size; i++, j++)
        comand_line[i] = str_arq[j];

    comand_line[size - 1] = ' ';

}

bool arq_ext(char *str_arq){
    FILE *arq;
    arq = fopen(str_arq, "r");
    if(arq == NULL){
        if(MESSAGES)
            printf("Arquivo de entrada invalido\n");
        // fclose(arq);
        return false;
    }
    else{
        fclose(arq);
        return true;
    }
}


char * find_input_file(char **argv, int *argc){
    for(int i = 0; i < *argc; i++){
        int n = (int) strlen(argv[i]);
        if( argv[i][n - 4] == '.' && argv[i][n - 3] == 't' && argv[i][n - 2] == 'x' && argv[i][n - 1] == 't'){
            if(arq_ext(argv[i]))
                return argv[i];
        }
    }

    return NULL;
}

bool search_command(char **comands, int num_comands, char str[]){
    for(int i = 0; i < num_comands; i++)
        if(strcmp(comands[i], str) == 0)
            return true;
    return false;
}

void print_comand_line(char *comand_line, int size){
    printf("%d - ", size);
    for(int i = 0; i < size; i++)
        printf("%c", comand_line[i]);
    printf("|\n");
}

char * insert_comands(char **comands, int *num_comands, char *comand_line, int *comand_size, char **argv, int *argc){

    if(search_command(comands, *num_comands, "--warn")){
        char warn[] = "-g -Wall -Wextra ";
        int warn_size = (int) strlen(warn);
        
        comand_line = (char *) realloc(comand_line, *comand_size + warn_size * sizeof(char));
        
        for(int i = 0; i < warn_size; i++){
            comand_line[ *comand_size + i ] = warn[i];
        }

        *comand_size = *comand_size +  warn_size;        
    }
    if(search_command(comands, *num_comands, "--exec")){
        char exec[] = "&& ./";
        int exec_size = (int) strlen(exec);

        comand_line = (char *) realloc(comand_line, *comand_size + exec_size * sizeof(char));

        for(int i = 0; i < exec_size; i++){
            comand_line[ *comand_size + i ] = exec[i];
        }

        *comand_size += exec_size;

        int str_file_size = (int) strlen( argv[1] ) - 2;

        comand_line = (char *) realloc(comand_line, (*comand_size + str_file_size + 1) * sizeof(char));

        for(int i = 0; i < str_file_size; i++)
            comand_line[ *comand_size + i ] = argv[1][i];

        *comand_size += str_file_size + 1;

        comand_line[ *comand_size - 1 ] = ' ';
    }
    if(search_command(comands, *num_comands, "--input")){
        char *str_input_file;
        str_input_file = find_input_file(argv, argc);
        
        if(str_input_file != NULL){
            int size_input_file = (int) strlen(str_input_file);
            char input[] = "< ";
            
            int input_size = (int) strlen(input);

            comand_line = (char *) realloc(comand_line, *comand_size + input_size * sizeof(char));

            for(int i = 0; i < input_size; i++)
                comand_line[ *comand_size + i ] = input[i];    
            
            *comand_size += input_size;

            comand_line = (char *) realloc(comand_line, *comand_size + size_input_file * sizeof(char));

            for(int i = 0; i < size_input_file; i++)
                comand_line[ *comand_size + i ] = str_input_file[i];
            
            *comand_size += size_input_file;
        }
    }
    return comand_line;
}

bool is_comand(char *comand){
    char *p;
    
    if(comand[0] == '-' && comand[1] == '-'){
    
        p = (char *) malloc(sizeof(char) * (strlen(comand) - 2));
    
        for(int i = 0; i < (int) strlen(comand); i++)
            p[i] = comand[i + 2];
        
        if(strcmp(p, "warn") == 0){
            free(p);
            return true;
        }
        else if(strcmp(p, "exec") == 0){
            free(p);
            return true;
        }
        else if(strcmp(p, "input") == 0){
            free(p);
            return true;
        }
        else{
            free(p);
            return false;
        }
    }else
        return false;
}

void filter_comands(char **comands, int *cn, char ** argv, int argc){
    for(int i = 2; i < argc; i++){
        if(is_comand(argv[i])){
            *cn += 1;
            comands = (char **) realloc(comands, *cn * sizeof(char *));
            // comands[*cn - 1] = (char *) malloc(sizeof(char) * strlen(argv[i]));
            comands[*cn - 1] = argv[i];
            // for(int j = 0; j < strlen(argv[i]); j++)
            //     comands[ *cn - 1 ][j] = argv[i][j];
        }
    }
}



void print_comands(char **comands, int n){
    for(int i = 0; i < n; i++){
        if(is_comand(comands[i]))
            puts(comands[i]);
    }
}