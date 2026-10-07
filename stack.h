#ifndef STACK_H
#define STACK_H

typedef double stack_element;

struct stack {
    long long left_canary;

    int capacity;
    stack_element* data;
    void* raw_data;
    int size;

    long long right_canary;


};

enum stack_error {
    stack_ok = 0,
    stack_null_ptr,
    stack_data_null,
    stack_bad_capacity,
    stack_bad_size,
    stack_calloc_error,
    stack_realloc_error,
    stack_empty,
    left_canary_error,
    right_canary_error,
    left_data_canary_error,
    right_data_canary_error
};

enum stack_error stack_construct(struct stack* st, int initial_capacity);
enum stack_error stack_verify(const struct stack* st);
enum stack_error stack_push(struct stack* st, stack_element value);
enum stack_error stack_resize(struct stack* st);
enum stack_error stack_pop(struct stack* st, stack_element* value);
enum stack_error stack_destroy(struct stack* st);

void stack_dump(const struct stack* st);

#endif // STACK_H
