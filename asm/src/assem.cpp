#include "assem.h"

#include <string.h>
#include <stdlib.h>

enum command_code find_command(const char* name){
    const struct command_entry command[] = {
        {"HLT",  cmd_hlt},
        {"PUSH", cmd_push},
        {"ADD",  cmd_add},
        {"SUB",  cmd_sub},
        {"MUL",  cmd_mul},
        {"DIV",  cmd_div},
        {"OUT",  cmd_out},
        {"JMP",  cmd_jmp},
        {"JA",   cmd_ja},
        {"JB",   cmd_jb},
        {"JE",   cmd_je}
    };
    size_t count = sizeof(command) / sizeof(command[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(name, command[i].name) == 0) {
            return command[i].code;
        }
    }
    return cmd_unknown;
}

enum text_error get_command(const struct text_command* text, struct program* prog){
    if (text == NULL || prog == NULL) {
        return text_null_ptr;
    }

    prog->count = 0;
    prog->commands = NULL;

    if (text->nlines == 0) {
        return text_ok;
    }

    prog->commands = (struct instruction*)calloc(
        text->nlines, sizeof(struct instruction));

    if (prog->commands == NULL) {
        return text_malloc_error;
    }

    for (int i = 0; i < text->nlines; i++) {
        char name[32] = {};
        double argument = 0.0;

        int fields = sscanf(text->lines[i], "%31s %lf", name, &argument);

        if (fields < 1) {
            continue;
        }

        enum command_code cmd = find_command(name);

        bool needs_argument = (cmd == cmd_push || cmd == cmd_jmp || cmd == cmd_ja);

        if (cmd == cmd_unknown || (needs_argument && fields != 2)) {
            free(prog->commands);
            prog->commands = NULL;
            prog->count = 0;
            return text_read_error;
        }

        prog->commands[prog->count].code = cmd;
        prog->commands[prog->count].argument = argument;
        prog->count++;
    }

    return text_ok;
}
