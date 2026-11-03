#ifndef SPU_H
#define SPU_H

#include "stack.h"
#include "calc.h"

struct stack_cnd{
    struct stack* st;
    int ip ;
};

enum stack_error processor(struct program* prog, struct stack_cnd* cpu);
enum stack_error calculator_add(struct stack* st);
enum stack_error calculator_sub(struct stack* st);
enum stack_error calculator_mul(struct stack* st);
enum stack_error calculator_div(struct stack* st);
enum stack_error calcucator_jmp(struct stack_cnd* cpu, struct program* prog);
enum stack_error calc_ja(struct stack_cnd* cpu, struct program* prog);
enum stack_error calculator_out(struct stack* st);
enum stack_error calc_jb(struct stack_cnd* cpu, struct program* prog);
enum stack_error calc_je(struct stack_cnd* cpu, struct program* prog);

#endif
