#include<stdio.h>
#include<stdlib.h>
#include "mylib.h"

int main(int argc, char **argv){
    
    if(!arq_ext(argv[1]))
        return 0;
    
    int num_comands = 0;
    char **comands = (char **) malloc(0);
    
    filter_comands(comands, &num_comands, argv, argc);
    
    int comand_size = 9 + strlen(argv[1]) + (strlen(argv[1]) - 2);
    char *comand_line = (char *) malloc(sizeof(char) * comand_size);
    
    initialize_comand_line(comand_line, comand_size, argv[1]);
    
    comand_line = insert_comands(comands, &num_comands, comand_line, &comand_size, argv, &argc);
    
    system(comand_line);

    free(comands);
    
    free(comand_line);
}