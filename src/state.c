#include "state.h"
#include <stdlib.h>

State* state_create(size_t totRules, const char* inps, const char* outs, State** states, const short* next) {
    State* S = malloc(sizeof *S);
    S->totRules = totRules;
    S->inps = malloc(totRules * sizeof *S->inps);
    S->outs = malloc(totRules * sizeof *S->outs);
    S->states = malloc(totRules * sizeof *S->states);
    S->next = malloc(totRules * sizeof *S->next);
    for(size_t i = 0; i < totRules; ++i) {
        S->inps[i] = inps[i];
        S->outs[i] = outs[i];
        S->states[i] = states[i];
        S->next[i] = next[i];
    }
    
    return S;
}

void state_free(State* S) {
    free(S->inps);
    free(S->outs);
    free(S->states);
    free(S->next);
    free(S);
}