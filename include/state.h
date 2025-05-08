#ifndef STATE_H
#define STATE_H

#include <stdlib.h>

typedef struct State State;
struct State {
    size_t totRules;
    char* inps;
    char* outs;
    State** states;
    short* next;
    char* name;
};

State* state_create(size_t totRules, const char* inps, const char* outs, State** states, const short* next, char* name);
void state_free(State* S);

#endif