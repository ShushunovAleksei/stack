#ifndef ASSEM_H
#define ASSEM_H

#include <stdio.h>
#include "calc.h"

struct text_command {
    char* buffer;
    char** lines;
    int nlines;
};


enum text_error {
    text_ok = 0,
    text_null_ptr,
    text_open_error,
    text_seek_error,
    text_tell_error,
    text_malloc_error,
    text_read_error
};

enum text_error open_program_file(const char* filename, FILE** out_file, long* file_size);
enum text_error text_to_buffer(struct text_command* text, FILE* fp, long size);
enum text_error split_buffer(struct text_command* text, long size);
int get_count_lines(char* buffer, long file_size);
enum command_code find_command(const char* name);
enum text_error get_command(const struct text_command* text, struct program* prog);

#endif
