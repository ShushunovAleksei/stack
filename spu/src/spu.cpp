#include "spu.h"
#include "assem.h"

#include <stdio.h>
#include <stdlib.h>

#define CHECK_STATUS(status)        \
    do {                            \
        if ((status) != stack_ok) { \
            STACK_REPORT(status);   \
            return status;          \
        }                           \
    } while (0)

#define BINARY_CHECK_STATUS(status, file, text, prog, st) \
    do { \
        if ((status) != binary_ok) { \
            printf("ошибка в записи бинарного файла, ошибка :%d\n", (status)); \
            cleanup_resources(&(file), &(text), &(prog) , &(loaded_program)); \
            stack_destroy(&(st)); \
            return 1; \
        } \
    } while (0)


static void cleanup_resources(FILE** file,
                             struct text_command* text,
                             struct program* prog,
                             struct program* loaded_program)
{
    if (file != NULL && *file != NULL) {
        fclose(*file);
        *file = NULL;
    }

    if (text != NULL) {
        if (text->buffer != NULL) {
            free(text->buffer);
            text->buffer = NULL;
        }

        if (text->lines != NULL) {
            free(text->lines);
            text->lines = NULL;
        }
    }

    if (prog != NULL) {
        if (prog->commands != NULL) {
            free(prog->commands);
            prog->commands = NULL;
        }
        prog->count = 0;
    }

    if (loaded_program != NULL) {
        if (loaded_program->commands != NULL) {
            free(loaded_program->commands);
            loaded_program->commands = NULL;
        }
        loaded_program->count = 0;
    }
}

#define CHECK_TEXT_STATUS(status, file, text, prog, loaded_program)         \
    do {                                                                    \
        if ((status) != text_ok) {                                           \
            fprintf(stderr, "Ошибка чтения: %d\n", (status));              \
            cleanup_resources(&(file), &(text), &(prog), &(loaded_program)); \
            stack_destroy(&st);                                              \
            return 1;                                                       \
        }                                                                   \
    } while (0)


int main() {
    struct stack st;
    enum stack_error status = stack_construct(&st, 4);
    CHECK_STATUS(status);

    FILE* file = NULL;
    long size = 0;
    struct text_command text = {};
    struct program prog = {};

    struct program loaded_program = {};

    enum text_error text_status = open_program_file("tests/arithmetic.asm", &file, &size);
    CHECK_TEXT_STATUS(text_status, file, text, prog, loaded_program);

    text_status = text_to_buffer(&text, file, size);
    CHECK_TEXT_STATUS(text_status, file, text, prog, loaded_program);
    fclose(file);
    file = NULL;

    text_status = split_buffer(&text, size);
    CHECK_TEXT_STATUS(text_status, file, text, prog, loaded_program);

    text_status = get_command(&text, &prog);
    CHECK_TEXT_STATUS(text_status, file, text, prog, loaded_program);

    enum binary_error write_status = write_binary("binary.bin", &prog);
    BINARY_CHECK_STATUS(write_status, file, text, prog, st);
    text_status = text_code_writer(&prog, "program.lst");
    CHECK_TEXT_STATUS(text_status, file, text, prog, loaded_program);

    write_status = read_binary(&loaded_program, "binary.bin");
    BINARY_CHECK_STATUS(write_status, file, text, prog, st);


    struct stack_cnd cpu = {} ;
    cpu.st = &st;
    cpu.ip = 0;

    enum stack_error execution_status = processor(&loaded_program, &cpu);

    status = stack_destroy(&st);
    cleanup_resources(&file, &text, &prog, &loaded_program); 

    CHECK_STATUS(execution_status);
    CHECK_STATUS(status);
    return 0;
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
            case cmd_ja:
                status = calc_ja(cpu, prog);
                break;
            case cmd_jb:
                status = calc_jb(cpu, prog);
                break;
            case cmd_je:
                status = calc_je(cpu, prog);
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
