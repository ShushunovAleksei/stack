#include <stdio.h>
#include <stdlib.h>

typedef double stack_element;

struct stack {
    int capacity;
    stack_element* data;
    int size;
};

enum stack_error {
    stack_ok = 0,
    stack_null_ptr,
    stack_data_null,
    stack_bad_capacity,
    stack_bad_size,
    stack_calloc_error,
    stack_realloc_error,
    stack_empty
};

enum stack_error stack_construct(struct stack* st, int initial_capacity);
enum stack_error stack_verify(const struct stack* st);
enum stack_error stack_push(struct stack* st, stack_element value);
enum stack_error stack_resize(stack_element** data, int* capacity);
enum stack_error stack_pop(struct stack* st, stack_element* value);

void stack_dump(const struct stack* st);
enum stack_error stack_destroyer(struct stack* st);


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

    stack_dump(&my_stack);

    status = stack_push(&my_stack, add_value);

    if (status != stack_ok) {
        printf("stack push error: %d\n", status);
        return 1;
    }

    stack_dump(&my_stack);
    stack_destroyer(&my_stack);
    return 0;
}


enum stack_error stack_construct(struct stack* st, int initial_capacity)
{
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (initial_capacity <= 0) {
        return stack_bad_capacity;
    }

    st->capacity = initial_capacity;
    st->size = 0;

    st->data = (stack_element*)calloc(
        initial_capacity,
        sizeof(*st->data)
    );

    if (st->data == NULL) {
        return stack_calloc_error;
    }

    return stack_ok;
}


enum stack_error stack_verify(const struct stack* st)
{
    if (st == NULL) {
        return stack_null_ptr;
    }

    if (st->data == NULL) {
        return stack_data_null;
    }

    if (st->capacity <= 0) {
        return stack_bad_capacity;
    }

    if (st->size < 0 || st->size > st->capacity) {
        return stack_bad_size;
    }

    return stack_ok;
}


enum stack_error stack_push(struct stack* st, stack_element value)
{
    enum stack_error status = stack_verify(st);

    if (status != stack_ok) {
        return status;
    }

    if (st->size == st->capacity) {
        status = stack_resize(&st->data, &st->capacity);

        if (status != stack_ok) {
            return status;
        }
    }

    st->data[st->size] = value;
    st->size++;

    return stack_ok;
}


void stack_dump(const struct stack* st)
{
    enum stack_error status = stack_verify(st);

    if (status != stack_ok) {
        printf("stack dump error: %d\n", status);
        return;
    }

    printf("size = %d\n", st->size);
    printf("capacity = %d\n", st->capacity);

    for (int i = 0; i < st->size; i++) {
        printf("data[%d] = %lf\n", i, st->data[i]);
    }
}


enum stack_error stack_resize(stack_element** data, int* capacity)
{
    int new_capacity = (*capacity) * 2;

    stack_element* new_data = (stack_element*)realloc(*data,new_capacity * sizeof(**data));

    if (new_data == NULL) {
        return stack_realloc_error;
    }

    *data = new_data;
    *capacity = new_capacity;

    return stack_ok;
}

enum stack_error stack_pop(struct stack* st, stack_element* value){
    enum stack_error status = stack_verify(st);

    if (status != stack_ok) {
        return status;
    }

    if (value == NULL) {
        return stack_null_ptr;
    }

    if (st->size == 0) {
        return stack_empty;
    }

    st->size--;
    *value = st->data[st->size];

    return stack_ok;
}

enum stack_error stack_destroyer(struct stack* st){
    free(st->data);
    st->data = NULL;
    st->size = 0;
    st->capacity = 0;
    return stack_ok;
}