#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(x) x
#else
#define ON_DEBUG(x)
#endif

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
    st->left_canary = 0xEDA ;
    st->right_canary = 0xBEDA ;

    size_t memory_size = (sizeof(long long))*2 + initial_capacity*sizeof(stack_element) ;

    st->raw_data = calloc(1, memory_size);
    if (st->raw_data == NULL) {
        return stack_calloc_error;
    }


    long long* left_data_canary = (long long*)st->raw_data ;
    st->data = (stack_element*)((char*)st->raw_data + sizeof(long long));
    long long* right_data_canary = (long long*)(st->data + st->capacity);
    
    *left_data_canary = 0xABCD ;
    *right_data_canary = 0xDCBA ;

    ON_DEBUG(
        printf("stack_construct: capacity = %d\n", st->capacity);
        stack_dump(st);
    )

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
    if(st->left_canary != 0xEDA){
        return left_canary_error;
    }
    if(st->right_canary != 0xBEDA){
        return right_canary_error;
    }
    if (st->raw_data == NULL) {
        return stack_data_null;
    }

    long long* left_data_canary = (long long*)st->raw_data;
    long long* right_data_canary = (long long*)(st->data + st->capacity);
    if(*left_data_canary != 0xABCD){
        return left_data_canary_error;
    }
    if(*right_data_canary != 0xDCBA ){
        return right_data_canary_error; 
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
        status = stack_resize(st);

        if (status != stack_ok) {
            return status;
        }
    }

    st->data[st->size] = value;
    st->size++;

    ON_DEBUG(
        printf("stack_push: value = %lf\n", value);
        stack_dump(st);
    )

    return stack_ok;
}


enum stack_error stack_pop(struct stack* st, stack_element* value)
{
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

    ON_DEBUG(
        printf("stack_pop: value = %lf\n", *value);
        stack_dump(st);
    )

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

    printf("\n");
}


enum stack_error stack_destroy(struct stack* st)
{
    if (st == NULL) {
        return stack_null_ptr;
    }

    ON_DEBUG(
        printf("stack_destroy\n");
    )

    free(st->raw_data);

    st->data = NULL;
    st->size = 0;
    st->capacity = 0;
    st->raw_data = NULL;
    

    return stack_ok;
}

enum stack_error stack_resize(struct stack* st)
{
    enum stack_error status = stack_verify(st);

    if (status != stack_ok) {
        return status;
    }

    int old_capacity = st->capacity;
    int new_capacity = old_capacity * 2;

    size_t new_memory_size = sizeof(long long) * 2 + sizeof(stack_element) * new_capacity;

    void* new_raw_data = realloc(st->raw_data, new_memory_size);

    if (new_raw_data == NULL) {
        return stack_realloc_error;
    }

    st->raw_data = new_raw_data;
    st->capacity = new_capacity;

    st->data = (stack_element*)((char*)st->raw_data + sizeof(long long));

    long long* left_data_canary = (long long*)st->raw_data;

    long long* right_data_canary = (long long*)(st->data + st->capacity);

    *left_data_canary = 0xABCD;
    *right_data_canary = 0xDCBA;

    ON_DEBUG(printf("stack_resize: %d -> %d\n", old_capacity, new_capacity);)

    return stack_ok;
}