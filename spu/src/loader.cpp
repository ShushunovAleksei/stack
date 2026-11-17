#include "spu.h"
#include<assem.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <calc.h>
#include<stdlib.h>

enum binary_error read_binary(struct program* prog, const char* filename){
    if(prog == NULL || filename == NULL){
        return binary_bad_argument;
    }

    prog->commands = NULL ;
    prog->count = 0;

    FILE* file =  fopen(filename, "rb");
    if(file == NULL){
        return binary_open_error ;
    }
    char signature[4] = {}; 

    if (fread(signature, sizeof(char), 4, file) != 4) {
        fclose(file);
        return binary_read_error;
    }

    if(memcmp(signature, "STKP", 4) != 0){
        printf("ошибка, указан не тот бинарный файл\n");
        return binary_bad_argument ;
    }

    uint32_t count = 0;
    if (fread(&count, sizeof(count), 1, file) != 1) {
        fclose(file);
        return binary_read_error;
    }
    if(count == 0){
        return binary_ok;
    }
    prog->commands = (struct instruction*)calloc(count, sizeof(struct instruction));
    if(prog->commands == NULL){
        free(prog->commands);
        return binary_mem_error ;
    }
    
    for(int i = 0; i < count; i++){
        int32_t code = 0;
        double argument = 0.0;
        if(fread(&code, sizeof(code), 1, file) == 1 && fread(&argument, sizeof(argument), 1, file) == 1){;
            prog->commands[i].code = (enum command_code)code;
            prog->commands[i].argument = argument;
        }else{
            free(prog->commands);
            prog->commands = NULL; ;
            fclose(file);
            return binary_read_error;
        }
    }
    fclose(file);
    prog->count = count ;
    return binary_ok;
}