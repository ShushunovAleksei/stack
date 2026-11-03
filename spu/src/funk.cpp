#include "spu.h"

#include <stdio.h>
#include <math.h>

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

    stack_element val = 0.0;
    enum stack_error status = stack_pop(st, &val);
    if (status != stack_ok) {
        return status;
    }

    printf("OUT: %.2f\n", val);
    return stack_ok;
}

enum stack_error calcucator_jmp(struct stack_cnd* cpu, struct program* prog){
    double val = prog->commands[cpu->ip].argument;
    if(floor(val) != val || val < 0 || val >= prog->count || !isfinite(val)){
        return stack_bad_capacity;
    }
    cpu->ip = val ;
    return stack_ok;
}

enum stack_error calc_ja(struct stack_cnd* cpu, struct program* prog){
    double val = prog->commands[cpu->ip].argument;
    if(floor(val) != val || val < 0 || val >= prog->count || !isfinite(val)){
        return stack_bad_capacity;
    }
    if(cpu->st->size < 2){
        return stack_bad_capacity;
    }
    enum stack_error status;
    stack_element value1, value2 ;
    status = stack_pop(cpu->st, &value1);
    if(status != stack_ok){
        return status;
    }
    status = stack_pop(cpu->st, &value2);
    if(status != stack_ok){
        return status;
    }
    if(value2 > value1){
        cpu->ip = prog->commands[cpu->ip].argument ;
    }else{
        cpu->ip++;
    }
    return stack_ok;
}

enum stack_error calc_jb(struct stack_cnd* cpu, struct program* prog){
    double val = prog->commands[cpu->ip].argument;
    if(floor(val) != val || val < 0 || val >= prog->count || !isfinite(val)){
        return stack_bad_capacity;
    }
    if(cpu->st->size < 2){
        return stack_bad_capacity;
    }
    enum stack_error status;
    stack_element value1, value2 ;
    status = stack_pop(cpu->st, &value1);
    if(status != stack_ok){
        return status;
    }
    status = stack_pop(cpu->st, &value2);
    if(status != stack_ok){
        return status;
    }
    if(value2 < value1){
        cpu->ip = prog->commands[cpu->ip].argument ;
    }else{
        cpu->ip++;
    }
    return stack_ok;
}

enum stack_error calc_je(struct stack_cnd* cpu, struct program* prog){
    double val = prog->commands[cpu->ip].argument;
    if(floor(val) != val || val < 0 || val >= prog->count || !isfinite(val)){
        return stack_bad_capacity;
    }
    if(cpu->st->size < 2){
        return stack_bad_capacity;
    }
    enum stack_error status;
    stack_element value1, value2 ;
    status = stack_pop(cpu->st, &value1);
    if(status != stack_ok){
        return status;
    }
    status = stack_pop(cpu->st, &value2);
    if(status != stack_ok){
        return status;
    }
    if(value2 == value1){
        cpu->ip = prog->commands[cpu->ip].argument ;
    }else{
        cpu->ip++;
    }
    return stack_ok;
}
