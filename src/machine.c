#include "machine.h"
#include <stdio.h>

#define REEL_SIZE 2048

Machine* machine_create(size_t totStates, State** states, size_t inputSize, size_t inputStart, char* input, char blank) {
    Machine* M = malloc(sizeof *M);
    M->pos = inputStart;
    M->state = states[0];
    M->totStates = totStates;
    M->blank = blank;
    M->reel = malloc((REEL_SIZE + 2) * sizeof *M->reel);
    for(size_t i = 0; i < REEL_SIZE + 2; ++i) M->reel[i] = blank;
    for(size_t i = 0; i < inputSize; ++i) M->reel[inputStart + i] = input[i];
    
    return M;
}

void machine_free(Machine* M) {
    free(M->reel);
    free(M);
}

void machine_print_reel(Machine* M) {
    for(size_t i = M->pos > 10 ? M->pos - 10 : 0; i < M->pos + 10; ++i) putchar(M->reel[i]);
    putchar('\n');
}

void machine_step(Machine* M) {
    if(!M->state) return;
    char inp = M->reel[M->pos];
    size_t idx;
    int found = 0;
    for(idx = 0; idx < M->state->totRules; ++idx) {
        if(inp == M->state->inps[idx]) {
            found = 1;
            break;
        }
    }
    if(!found) return;
    
    M->reel[M->pos] = M->state->outs[idx];
    M->pos += M->state->next[idx];
    M->state = M->state->states[idx];
    
    machine_print_reel(M);
}

void machine_start(Machine* M) {
    while(M->state) {
        char inp = M->reel[M->pos];
        int match = 0;
        for(size_t i = 0; i < M->state->totRules; ++i) {
            if(inp == M->state->inps[i]) {
                match = 1; 
                break;
            }
        }
        if(!match) break;

        machine_step(M);
    }
}