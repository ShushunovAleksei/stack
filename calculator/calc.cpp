#include "../stack/stack.h"

#include <stdio.h>

#define CHECK_STATUS(status)        \
    do {                            \
        if ((status) != stack_ok) { \
            STACK_REPORT(status);   \
            return status;          \
        }                           \
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
    if(st->data[st->size-1] == 0){
        STACK_REPORT(stack_value_zero);
        return stack_value_zero ;
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

    status = stack_push(&st, 10.0);
    CHECK_STATUS(status);

    status = stack_push(&st, 7.5);
    CHECK_STATUS(status);

    status = calculator_add(&st);
    CHECK_STATUS(status);

    stack_element result = 0.0;
    status = stack_pop(&st, &result);
    CHECK_STATUS(status);

    printf("Result: %.2f\n", result);

    status = stack_destroy(&st);
    CHECK_STATUS(status);

    return 0;
}