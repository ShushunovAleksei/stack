#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>

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

enum command_code {
    cmd_unknown = -1,
    cmd_hlt = 0,
    cmd_push,
    cmd_add,
    cmd_sub,
    cmd_mul,
    cmd_div,
    cmd_out
};

enum text_error open_program_file(const char* filename, FILE** out_file, long* file_size);
enum text_error text_to_buffer(struct text_command* text, FILE* fp, long size);
enum text_error split_buffer(struct text_command* text, long size);
int get_count_lines(char* buffer, long file_size);


#endif
