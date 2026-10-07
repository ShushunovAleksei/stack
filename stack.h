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
    stack_null_ptr = 1,
    stack_data_null = 2,
    stack_bad_capacity = 3,
    stack_bad_size = 4,
    stack_calloc_error = 5,
    stack_realloc_error = 6,
    stack_empty = 7,
    left_canary_error = 8,
    right_canary_error = 9,
    left_data_canary_error = 10,
    right_data_canary_error = 11
};

enum stack_error stack_construct(struct stack* st, int initial_capacity);
enum stack_error stack_verify(const struct stack* st);
enum stack_error stack_push(struct stack* st, stack_element value);
enum stack_error stack_resize(struct stack* st);
enum stack_error stack_pop(struct stack* st, stack_element* value);
enum stack_error stack_destroy(struct stack* st);

void stack_dump(const struct stack* st);


void stack_report_error(enum stack_error error, const char* file,int line, const char* function);

// Макрос передаёт код ошибки и место, где написан STACK_REPORT.
#define STACK_REPORT(error) \
    stack_report_error((error), __FILE__, __LINE__, __func__)

#endif 
