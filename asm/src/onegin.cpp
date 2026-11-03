#include "assem.h"

#include <stdlib.h>

enum text_error open_program_file(const char* filename, FILE** out_file, long* file_size){
    if (filename == NULL || out_file == NULL || file_size == NULL) {
        return text_null_ptr;
    }

    *out_file = NULL;
    *file_size = 0;

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return text_open_error;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return text_seek_error;
    }

    long size = ftell(file);
    if (size == -1L) {
        fclose(file);
        return text_tell_error;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return text_seek_error;
    }

    *file_size = size;
    *out_file = file;
    return text_ok;
}

enum text_error text_to_buffer(struct text_command* text, FILE* fp, long size){
    if (text == NULL || fp == NULL) {
        return text_null_ptr;
    }

    text->buffer = (char*)malloc((size_t)size + 1);
    if (text->buffer == NULL) {
        return text_malloc_error;
    }

    size_t read_count = fread(text->buffer, sizeof(char), (size_t)size, fp);
    text->buffer[size] = '\0';

    if (read_count != (size_t)size) {
        free(text->buffer);
        text->buffer = NULL;
        return text_read_error;
    }

    return text_ok;
}

enum text_error split_buffer(struct text_command* text, long size){
    if (text == NULL || text->buffer == NULL) {
        return text_null_ptr;
    }

    text->nlines = get_count_lines(text->buffer, size);
    text->lines = NULL;

    if (text->nlines == 0) {
        return text_ok;
    }

    text->lines = (char**)calloc((size_t)text->nlines, sizeof(char*));
    if (text->lines == NULL) {
        text->nlines = 0;
        return text_malloc_error;
    }

    long line_start = 0;
    int current_line = 0;

    for (long i = 0; i < size; i++) {
        if (text->buffer[i] == '\n') {
            text->buffer[i] = '\0';
            text->lines[current_line] = &text->buffer[line_start];
            current_line++;
            line_start = i + 1;
        }
    }

    if (line_start < size) {
        text->lines[current_line] = &text->buffer[line_start];
    }

    return text_ok;
}

int get_count_lines(char* buffer, long file_size){
    int count = 0;

    for (long i = 0; i < file_size; i++) {
        if (buffer[i] == '\n') {
            count++;
        }
    }

    if (file_size > 0 && buffer[file_size - 1] != '\n') {
        count++;
    }

    return count;
}
