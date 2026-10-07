#include "stack.h"

#include <stdio.h>

int main()
{
    struct stack my_stack;
    

    int initial_capacity = 4;
    stack_element add_value = 3;

    enum stack_error status = stack_construct(&my_stack, initial_capacity);

    if (status != stack_ok) {
        printf("stack construct error: %d\n", status);
        return 1;
    }

    status = stack_push(&my_stack, add_value);

    if (status != stack_ok) {
        printf("stack push error: %d\n", status);
        return 1;
    }

    stack_element popped_value = 0;

    status = stack_pop(&my_stack, &popped_value);

    if (status != stack_ok) {
        printf("stack pop error: %d\n", status);
        return 1;
    }

    status = stack_destroy(&my_stack);

    if (status != stack_ok) {
        printf("stack destroy error: %d\n", status);
        return 1;
    }

    return 0;
}
