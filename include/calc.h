#ifndef CALC_H
#define CALC_H

enum command_code {
    cmd_unknown = -1,
    cmd_hlt = 0,
    cmd_push = 1,
    cmd_add = 2,
    cmd_sub = 3,
    cmd_mul = 4,
    cmd_div = 5,
    cmd_out = 6,
    cmd_jmp = 7,
    cmd_ja = 8,
    cmd_jb = 9,
    cmd_je = 10 
};
enum binary_error {
    binary_ok = 0,
    binary_bad_argument = 1,
    binary_open_error = 2,
    binary_write_error = 3,
    binary_close_error = 4,
    binary_read_error = 5,
    binary_mem_error = 6
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
