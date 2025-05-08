#ifndef BUILDER_H
#define BUILDER_H

#include "machine.h"
#include "state.h"
#include <stdlib.h>

typedef enum { DIR_L = -1, DIR_R = 1 } Dir;

typedef struct {
    char in;
    char out;
    Dir move;
    const char* next_name;
} Rule;

typedef struct {
    const char* name;
    Rule* rules;
    size_t count;
    size_t cap;
} StateBuilder;

typedef struct {
    char* input;
    char blank;
    StateBuilder** sbuild;
    size_t scount;
    size_t scap;
} MachineBuilder;

MachineBuilder* builder_create(const char* input, char blank);
void builder_free(MachineBuilder* MB);
StateBuilder* builder_add_state(MachineBuilder* MB, const char* name);
void builder_add_rule(StateBuilder* SB, char in, char out, Dir move, const char* next_name);
Machine* builder_build(MachineBuilder* MB);

#endif