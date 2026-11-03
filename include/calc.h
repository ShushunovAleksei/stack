#ifndef CALC_H
#define CALC_H

enum command_code {
    cmd_unknown = -1,
    cmd_hlt = 0,
    cmd_push,
    cmd_add,
    cmd_sub,
    cmd_mul,
    cmd_div,
    cmd_out,
    cmd_jmp,
    cmd_ja,
    cmd_jb,
    cmd_je
};

struct command_entry{
    const char* name;
    enum command_code code;
};

struct instruction{
    enum command_code code;
    double argument;
};

struct program {
    struct instruction* commands;
    int count;
};

#endif
