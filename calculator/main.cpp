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

#define CHECK_TEXT_STATUS(status)                              \
    do {                                                       \
        if ((status) != text_ok) {                              \
            fprintf(stderr, "Ошибка чтения: %d\n", (status)); \
            return 1;                                          \
        }                                                      \
    } while (0)



enum stack_error calculator_add(struct stack* st)
{
    enum stack_error status = stack_verify(st);
    CHECK_STATUS(status);

    if (st->size < 2) {
        STACK_REPORT(stack_empty);
        return stack_empty;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    status = stack_pop(st, &first);
    CHECK_STATUS(status);

    status = stack_pop(st, &second);
    CHECK_STATUS(status);

    status = stack_push(st, second + first);
    CHECK_STATUS(status);

    return stack_ok;
}

enum stack_error calculator_sub(struct stack* st)
{
    enum stack_error status = stack_verify(st);
    CHECK_STATUS(status);

    if (st->size < 2) {
        STACK_REPORT(stack_empty);
        return stack_empty;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    status = stack_pop(st, &first);
    CHECK_STATUS(status);

    status = stack_pop(st, &second);
    CHECK_STATUS(status);

    status = stack_push(st, second - first);
    CHECK_STATUS(status);

    return stack_ok;
}

enum stack_error calculator_mul(struct stack* st)
{
    enum stack_error status = stack_verify(st);
    CHECK_STATUS(status);

    if (st->size < 2) {
        STACK_REPORT(stack_empty);
        return stack_empty;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    status = stack_pop(st, &first);
    CHECK_STATUS(status);

    status = stack_pop(st, &second);
    CHECK_STATUS(status);

    status = stack_push(st, second * first);
    CHECK_STATUS(status);

    return stack_ok;
}

enum stack_error calculator_div(struct stack* st)
{
    enum stack_error status = stack_verify(st);
    CHECK_STATUS(status);

    if (st->size < 2) {
        STACK_REPORT(stack_empty);
        return stack_empty;
    }

    if (st->data[st->size - 1] == 0) {
        STACK_REPORT(stack_value_zero);
        return stack_value_zero;
    }

    stack_element first = 0.0;
    stack_element second = 0.0;

    status = stack_pop(st, &first);
    CHECK_STATUS(status);

    status = stack_pop(st, &second);
    CHECK_STATUS(status);

    status = stack_push(st, second / first);
    CHECK_STATUS(status);

    return stack_ok;
}

int main()
{
    struct stack st;
    enum stack_error status = stack_construct(&st, 4);
    CHECK_STATUS(status);

    FILE* file = NULL;
    long size = 0;
    enum text_error text_status = open_program_file("program.asm", &file, &size);
    CHECK_TEXT_STATUS(text_status);

    struct text_command text = {};
    text_status = text_to_buffer(&text, file, size);
    CHECK_TEXT_STATUS(text_status);
    fclose(file);

    text_status = split_buffer(&text, size);
    CHECK_TEXT_STATUS(text_status);
    
    struct program prog = {} ;
    text_status = get_command(&text, &prog);
    CHECK_TEXT_STATUS(text_status);

    status = stack_destroy(&st);
    CHECK_STATUS(status);

    free(text.buffer);
    free(text.lines);
    free(prog.commands);


    return 0;
}
