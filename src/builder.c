#include "builder.h"
#include <string.h>
#include <stdlib.h>

#define SB_INIT_CAP 16
#define MB_INIT_CAP 16

/// @brief creates the turing machine
/// @param input pointer to an array of chars, is the starting contents of the reel
/// @param blank the character to represent a blank space on the reel
/// @return pointer to the machine
MachineBuilder* builder_create(const char* input, char blank, size_t inputStart, double sleep) {
    MachineBuilder* MB = malloc(sizeof *MB);
    MB->input = strdup(input);
    MB->blank = blank;
    MB->inputStart = inputStart;
    MB->scount = 0;
    MB->scap = MB_INIT_CAP;
    MB->sbuild = malloc(sizeof *MB->sbuild * MB->scap);
    MB->sleep = sleep;
    
    return MB;
}

/// @brief frees the memory occupied by a machine
/// @param MB pointer to the machine to be freed
void builder_free(MachineBuilder* MB) {
    free(MB->input);
    for(size_t i = 0; i < MB->scount; ++i) {
        free(MB->sbuild[i]->rules);
        free((void*)MB->sbuild[i]->name);
        free(MB->sbuild[i]);
    }

    free(MB->sbuild);
    free(MB);
}

/// @brief adds a set of rules to the machine, known as a state
/// @param MB pointer to the machine to add rules to
/// @param name pointer to the title of the state
/// @return pointer to the new state
StateBuilder* builder_add_state(MachineBuilder* MB, const char* name) {
    if(MB->scount == MB->scap) {
        MB->scap *= 2;
        MB->sbuild = realloc(MB->sbuild, sizeof *MB->sbuild * MB->scap);
    }

    StateBuilder* SB = malloc(sizeof *SB);
    SB->name = strdup(name);
    SB->count = 0;
    SB->cap = SB_INIT_CAP;
    SB->rules = malloc(sizeof *SB->rules * SB->cap);
    MB->sbuild[MB->scount++] = SB;
    
    return SB;
}

/// @brief adds a rule to a state
/// @param SB pointer to the state to add a rule to
/// @param in char that is read by the machine
/// @param out char that is written by the machine
/// @param move +1 for right, -1 for left
/// @param next_name pointer to the name of the state the machine should move to
void builder_add_rule(StateBuilder* SB, char in, char out, Dir move, const char* next_name) {
    if(SB->count == SB->cap) {
        SB->cap *= 2;
        SB->rules = realloc(SB->rules, sizeof *SB->rules * SB->cap);
    }
    
    SB->rules[SB->count++] = (Rule){in, out, move, next_name};
}

/// @brief finds the state given the name of the state
/// @param MB pointer to the machine containing the state to be found
/// @param name pointer to the name of the state to be found
/// @return pointer to the state, or NULL if unfound
static StateBuilder* find_state(MachineBuilder* MB, const char* name) {
    for(size_t i = 0; i < MB->scount; ++i) if(strcmp(MB->sbuild[i]->name, name) == 0) return MB->sbuild[i];
    return NULL;
}

/// @brief assembles the machine from the machine builder
/// @param MB pointer to the machine builder
/// @return pointer to the machine
Machine* builder_build(MachineBuilder* MB) {
    size_t n = MB->scount;
    State** states = malloc(sizeof *states * n);
    for(size_t i = 0; i < n; ++i) {
        StateBuilder* SB = MB->sbuild[i];
        State* S = malloc(sizeof *S);
        
        S->totRules = SB->count;
        S->inps = malloc(SB->count * sizeof *S->inps);
        S->outs = malloc(SB->count * sizeof *S->outs);
        S->states = malloc(SB->count * sizeof *S->states);
        S->next = malloc(SB->count * sizeof *S->next);
        
        size_t nameLen = strlen(SB->name) + 1;
        S->name = malloc(nameLen);
        strcpy(S->name, SB->name);
        
        states[i] = S;
    }
    
    for(size_t i = 0; i < n; ++i) {
        StateBuilder* SB = MB->sbuild[i];
        State* S = states[i];
        for(size_t j = 0; j < SB->count; ++j) {
            Rule* r = &SB->rules[j];
            S->inps[j] = r->in;
            S->outs[j] = r->out;
            S->next[j] = (short)r->move;
            StateBuilder* nSB = find_state(MB, r->next_name);
            size_t idx = 0;
            
            for(; idx < n; ++idx) if(MB->sbuild[idx] == nSB) break;

            S->states[j] = idx < n ? states[idx] : NULL;
        }
    }
    
    size_t inputSize = strlen(MB->input);
    size_t start = MB->inputStart;
    char*  tape = strdup(MB->input);
    Machine* M = machine_create(n, states, inputSize, start, tape, MB->blank, MB->sleep);

    free(states);

    return M;
}