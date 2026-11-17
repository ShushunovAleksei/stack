#include "assem.h"

#include <string.h>
#include <stdlib.h>

enum command_code find_command(const char* name){
    const struct command_entry command[] = {
        {"HLT",  cmd_hlt},
        {"PUSH", cmd_push},
        {"ADD",  cmd_add},
        {"SUB",  cmd_sub},
        {"MUL",  cmd_mul},
        {"DIV",  cmd_div},
        {"OUT",  cmd_out},
        {"JMP",  cmd_jmp},
        {"JA",   cmd_ja},
        {"JB",   cmd_jb},
        {"JE",   cmd_je}
    };
    size_t count = sizeof(command) / sizeof(command[0]);
    for (int i = 0; i < (int)count; i++) {
        if (strcmp(name, command[i].name) == 0) {
            return command[i].code;
        }
    }
    return cmd_unknown;
}

enum text_error get_command(const struct text_command* text, struct program* prog){
    if (text == NULL || prog == NULL) {
        return text_null_ptr;
    }

    prog->count = 0;
    prog->commands = NULL;

    if (text->nlines == 0) {
        return text_ok;
    }

    prog->commands = (struct instruction*)calloc(text->nlines, sizeof(struct instruction));

    if (prog->commands == NULL) {
        return text_malloc_error;
    }

    for (int i = 0; i < text->nlines; i++) {
        char name[32] = {};
        double argument = 0.0;

        int fields = sscanf(text->lines[i], "%31s %lf", name, &argument);

        if (fields < 1) {
            continue;
        }

        enum command_code cmd = find_command(name);

        bool needs_argument = (cmd == cmd_push || cmd == cmd_jmp || cmd == cmd_ja || cmd == cmd_jb || cmd == cmd_je);

        if (cmd == cmd_unknown || (needs_argument && fields != 2)) {
            free(prog->commands);
            prog->commands = NULL;
            prog->count = 0;
            return text_read_error;
        }

        prog->commands[prog->count].code = cmd;
        prog->commands[prog->count].argument = argument;
        prog->count++;
    }

    return text_ok;
}


enum binary_error write_binary(const char* filename, const struct program* prog){
    if (filename == NULL || prog == NULL || prog->count < 0) {
        return binary_bad_argument;
    }

    if (prog->count > 0 && prog->commands == NULL) {
        return binary_bad_argument;
    }

    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return binary_open_error;
    }
    
    uint32_t count = (uint32_t)prog->count;

    const char signature[4] = {'S', 'T', 'K', 'P'};
    if(fwrite(&signature, sizeof(char), 4, file) != 4 || fwrite(&count, sizeof(count), 1, file) != 1){
        fclose(file);
        return binary_write_error;
    }
    for(int i = 0; i < prog->count; i++){
        int32_t code = (int32_t)prog->commands[i].code;
        double argument = prog->commands[i].argument;
        if(fwrite(&code, sizeof(code), 1, file) != 1 || fwrite(&argument, sizeof(argument), 1, file) != 1){
            fclose(file);
            return binary_write_error;
        }
    }
    if(fclose(file) != 0){
        return binary_close_error;
    }
    return binary_ok ;
}

enum text_error text_code_writer(const struct program* prog, const char* filename){
    if (filename == NULL || prog == NULL || prog->count < 0) {
        return text_bad_argument;
    }

    if (prog->count > 0 && prog->commands == NULL) {
        return text_bad_argument;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return text_open_error;
    }
    fprintf(file, "INDEX CODE NAME ARGUMENT\n");
    
    for(int i = 0; i < prog->count; i++){
        const char* name = get_command_name(prog->commands[i].code);
        if(fprintf(file, "%d %d %s %g\n", i, prog->commands[i].code, name, prog->commands[i].argument) < 0){
            fclose(file);
            return text_write_error;
        }
    }

    fclose(file);
    return text_ok;
}

const char* get_command_name(enum command_code code){
    const struct command_entry command[] = {
        {"HLT",  cmd_hlt},
        {"PUSH", cmd_push},
        {"ADD",  cmd_add},
        {"SUB",  cmd_sub},
        {"MUL",  cmd_mul},
        {"DIV",  cmd_div},
        {"OUT",  cmd_out},
        {"JMP",  cmd_jmp},
        {"JA",   cmd_ja},
        {"JB",   cmd_jb},
        {"JE",   cmd_je}
    };
    size_t count = sizeof(command) / sizeof(command[0]);

    for(int i = 0; i < (int)count ; i++){
        if (command[i].code == code){
            return command[i].name;
        }
    }
    return "UNKNOWN" ;
}