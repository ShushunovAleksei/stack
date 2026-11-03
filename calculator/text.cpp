#include "text.h"
#include<string.h>
#include <stdlib.h>
#include<math.h>

enum text_error open_program_file(const char* filename, FILE** out_file, long* file_size){
    if (filename == NULL || out_file == NULL || file_size == NULL) {
        return text_null_ptr;
    }

    *out_file = NULL;
    *file_size = 0;

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return text_open_error;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return text_seek_error;
    }

    long size = ftell(file);
    if (size == -1L) {
        fclose(file);
        return text_tell_error;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return text_seek_error;
    }

    *file_size = size;
    *out_file = file;
    return text_ok;
}

enum text_error text_to_buffer(struct text_command* text, FILE* fp, long size){
    if (text == NULL || fp == NULL) {
        return text_null_ptr;
    }

    text->buffer = (char*)malloc((size_t)size + 1);
    if (text->buffer == NULL) {
        return text_malloc_error;
    }

    size_t read_count = fread(text->buffer, sizeof(char), (size_t)size, fp);
    text->buffer[size] = '\0';

    if (read_count != (size_t)size) {
        free(text->buffer);
        text->buffer = NULL;
        return text_read_error;
    }

    return text_ok;
}

enum text_error split_buffer(struct text_command* text, long size){
    if (text == NULL || text->buffer == NULL) {
        return text_null_ptr;
    }

    text->nlines = get_count_lines(text->buffer, size);
    text->lines = NULL;

    if (text->nlines == 0) {
        return text_ok;
    }

    text->lines = (char**)calloc((size_t)text->nlines, sizeof(char*));
    if (text->lines == NULL) {
        text->nlines = 0;
        return text_malloc_error;
    }

    long line_start = 0;
    int current_line = 0;

    for (long i = 0; i < size; i++) {
        if (text->buffer[i] == '\n') {
            text->buffer[i] = '\0';
            text->lines[current_line] = &text->buffer[line_start];
            current_line++;
            line_start = i + 1;
        }
    }

    if (line_start < size) {
        text->lines[current_line] = &text->buffer[line_start];
    }

    return text_ok;
}

int get_count_lines(char* buffer, long file_size){
    int count = 0;

    for (long i = 0; i < file_size; i++) {
        if (buffer[i] == '\n') {
            count++;
        }
    }

    if (file_size > 0 && buffer[file_size - 1] != '\n') {
        count++;
    }

    return count;
}

enum command_code find_command(const char* name){
    const struct command_entry command[] = {
        {"HLT",  cmd_hlt},
        {"PUSH", cmd_push},
        {"ADD",  cmd_add},
        {"SUB",  cmd_sub},
        {"MUL",  cmd_mul},
        {"DIV",  cmd_div},
        {"OUT",  cmd_out},
        {"JMP",  cmd_jmp}
    };
    for (int i = 0; i < 8; i++) {
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

    prog->commands = (struct instruction*)calloc(
        text->nlines, sizeof(struct instruction));

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

        bool needs_argument = (cmd == cmd_push || cmd == cmd_jmp);

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

enum stack_error calculator_add(struct stack* st){
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (st->size < 2) {
        return stack_empty;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    enum stack_error status = stack_pop(st, &first);
    if (status != stack_ok) {
        return status;
    }

    status = stack_pop(st, &second);
    if (status != stack_ok) {
        return status;
    }

    return stack_push(st, second + first);
}

enum stack_error calculator_sub(struct stack* st){
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (st->size < 2) {
        return stack_empty;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    enum stack_error status = stack_pop(st, &first);
    if (status != stack_ok) {
        return status;
    }

    status = stack_pop(st, &second);
    if (status != stack_ok) {
        return status;
    }

    return stack_push(st, second - first);
}

enum stack_error calculator_mul(struct stack* st){
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (st->size < 2) {
        return stack_empty;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    enum stack_error status = stack_pop(st, &first);
    if (status != stack_ok) {
        return status;
    }

    status = stack_pop(st, &second);
    if (status != stack_ok) {
        return status;
    }

    return stack_push(st, second * first);
}

enum stack_error calculator_div(struct stack* st){
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (st->size < 2) {
        return stack_empty;
    }

    if (st->data[st->size - 1] == 0) {
        return stack_value_zero;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    enum stack_error status = stack_pop(st, &first);
    if (status != stack_ok) {
        return status;
    }

    status = stack_pop(st, &second);
    if (status != stack_ok) {
        return status;
    }

    return stack_push(st, second / first);
}

enum stack_error calculator_out(struct stack* st){
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (st->size == 0) {
        return stack_empty;
    }

    stack_element value = 0.0;
    enum stack_error status = stack_pop(st, &value);
    if (status != stack_ok) {
        return status;
    }

    printf("OUT: %.2f\n", value);
    return stack_ok;
}

enum stack_error processor(struct program* prog, struct stack_cnd* cpu){
    if (prog == NULL || cpu == NULL) {
        return stack_null_ptr;
    }
    enum stack_error status;

    while(cpu->ip < prog->count){
        switch (prog->commands[cpu->ip].code) {
            case cmd_push:
                status = stack_push(cpu->st, prog->commands[cpu->ip].argument);
                cpu->ip++ ;
                break;
            case cmd_add:
                status = calculator_add(cpu->st) ;
                cpu->ip++ ;
                break;
            case cmd_sub:
                status = calculator_sub(cpu->st) ;
                cpu->ip++ ;
                break;
            case cmd_mul:
                status = calculator_mul(cpu->st);
                cpu->ip++ ;
                break;
            case cmd_div:
                status = calculator_div(cpu->st);
                cpu->ip++ ;
                break;
            case cmd_hlt:
                return stack_ok;
            case cmd_out:
                status = calculator_out(cpu->st);
                cpu->ip++ ;
                break;
            case cmd_jmp:
                status = calcucator_jmp(cpu, prog);
                break;
            case cmd_unknown:
            default:
                return stack_null_ptr;
        }
        if(status != stack_ok)
        return status;
    }

    return stack_ok;
}

enum stack_error calcucator_jmp(struct stack_cnd* cpu, struct program* prog){
    if(floor(prog->commands[cpu->ip].argument) != prog->commands[cpu->ip].argument || prog->commands[cpu->ip].argument < 0 || prog->commands[cpu->ip].argument >= prog->count ){
        return stack_bad_capacity;
    }
    cpu->ip = prog->commands[cpu->ip].argument ;
    return stack_ok;
}


