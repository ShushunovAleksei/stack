#include "text.h"

#include "../stack/stack.h"

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define CHECK_STATUS(status)        \
    do {                            \
        if ((status) != stack_ok) { \
            STACK_REPORT(status);   \
            return status;          \
        }                           \
    } while (0)

static void cleanup_resources(FILE** file,struct text_command* text,struct program* prog){
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
}

#define CHECK_TEXT_STATUS(status, file, text, prog)             \
    do {                                                       \
        if ((status) != text_ok) {                              \
            fprintf(stderr, "Ошибка чтения: %d\n", (status)); \
            cleanup_resources(&(file), &(text), &(prog));   \
            stack_destroy(&st);                             \
            return 1;                                          \
        }                                                      \
    } while (0)


int main()
{
    struct stack st;
    enum stack_error status = stack_construct(&st, 4);
    CHECK_STATUS(status);

    FILE* file = NULL;
    long size = 0;
    struct text_command text = {0};
    struct program prog = {0};

    enum text_error text_status = open_program_file("program.asm", &file, &size);
    CHECK_TEXT_STATUS(text_status, file, text, prog);

    text_status = text_to_buffer(&text, file, size);
    CHECK_TEXT_STATUS(text_status, file, text, prog);
    fclose(file);
    file = NULL;

    text_status = split_buffer(&text, size);
    CHECK_TEXT_STATUS(text_status, file, text, prog);

    text_status = get_command(&text, &prog);
    CHECK_TEXT_STATUS(text_status, file, text, prog);
    struct stack_cnd cpu = {} ;
    cpu.st = &st;
    cpu.ip = 0;

    enum stack_error execution_status = processor(&prog, &cpu);

    status = stack_destroy(&st);
    cleanup_resources(&file, &text, &prog);

    CHECK_STATUS(execution_status);
    CHECK_STATUS(status);
    return 0;
}
