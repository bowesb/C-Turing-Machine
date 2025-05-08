#ifndef MACHINE_H
#define MACHINE_H

#include "state.h"
#include <stdlib.h>

typedef struct {
    size_t pos;
    State* state;
    char* reel;
    size_t totStates;
    char blank;
    double sleep;
} Machine;

Machine* machine_create(size_t totStates, State** states, size_t inputSize, size_t inputStart, char* input, char blank, double sleep);
void machine_free(Machine* M);
void machine_start(Machine* M);
void machine_step(Machine* M);
void machine_print_reel(Machine* M);

#endif