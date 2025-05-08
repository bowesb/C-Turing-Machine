#include "state.h"
#include <stdlib.h>
#include <string.h>

/// @brief creates a state for a machine
/// @param totRules the number of rules the state will hold
/// @param inps the chars the state can read and do something with
/// @param outs the chars the state can write to the reel
/// @param states pointer to an array of states the machine can move to from this state
/// @param next pointer to an array holding the moves for a given input - left/right
/// @return pointer to the state
State* state_create(size_t totRules, const char* inps, const char* outs, State** states, const short* next, char* name) {
    State* S = malloc(sizeof *S);
    S->totRules = totRules;
    S->inps = malloc(totRules * sizeof *S->inps);
    S->outs = malloc(totRules * sizeof *S->outs);
    S->states = malloc(totRules * sizeof *S->states);
    S->next = malloc(totRules * sizeof *S->next);
    S->name = malloc(sizeof *S->name);
    
    for(size_t i = 0; i < totRules; ++i) {
        S->inps[i] = inps[i];
        S->outs[i] = outs[i];
        S->states[i] = states[i];
        S->next[i] = next[i];
    }
    
    return S;
}

/// @brief frees the memory occupied by the state
/// @param S pointer to the state to be freed
void state_free(State* S) {
    free(S->inps);
    free(S->outs);
    free(S->states);
    free(S->next);
    free(S->name);
    
    free(S);
}