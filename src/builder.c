#include "builder.h"
#include <string.h>
#include <stdlib.h>

#define SB_INIT_CAP 4
#define MB_INIT_CAP 4

MachineBuilder* builder_create(const char* input, char blank) {
    MachineBuilder* mb = malloc(sizeof *mb);
    mb->input = strdup(input);
    mb->blank = blank;
    mb->scount = 0;
    mb->scap = MB_INIT_CAP;
    mb->sbuild = malloc(sizeof *mb->sbuild * mb->scap);
    
    return mb;
}

void builder_free(MachineBuilder* mb) {
    free(mb->input);
    for(size_t i = 0; i < mb->scount; ++i) {
        free(mb->sbuild[i]->rules);
        free((void*)mb->sbuild[i]->name);
        free(mb->sbuild[i]);
    }
    free(mb->sbuild);
    free(mb);
}

StateBuilder* builder_add_state(MachineBuilder* mb, const char* name) {
    if(mb->scount == mb->scap) {
        mb->scap *= 2;
        mb->sbuild = realloc(mb->sbuild, sizeof *mb->sbuild * mb->scap);
    }

    StateBuilder* sb = malloc(sizeof *sb);
    sb->name = strdup(name);
    sb->count = 0;
    sb->cap = SB_INIT_CAP;
    sb->rules = malloc(sizeof *sb->rules * sb->cap);
    mb->sbuild[mb->scount++] = sb;
    
    return sb;
}

void builder_add_rule(StateBuilder* sb, char in, char out, Dir move, const char* next_name) {
    if(sb->count == sb->cap) {
        sb->cap *= 2;
        sb->rules = realloc(sb->rules, sizeof *sb->rules * sb->cap);
    }
    
    sb->rules[sb->count++] = (Rule){in, out, move, next_name};
}

static StateBuilder* find_state(MachineBuilder* mb, const char* name) {
    for(size_t i = 0; i < mb->scount; ++i) if(strcmp(mb->sbuild[i]->name, name) == 0) return mb->sbuild[i];
    return NULL;
}

Machine* builder_build(MachineBuilder* mb) {
    size_t n = mb->scount;
    State** states = malloc(sizeof *states * n);
    for(size_t i = 0; i < n; ++i) {
        StateBuilder* sb = mb->sbuild[i];
        State* S = malloc(sizeof *S);
        S->totRules = sb->count;
        S->inps = malloc(sb->count * sizeof *S->inps);
        S->outs = malloc(sb->count * sizeof *S->outs);
        S->states = malloc(sb->count * sizeof *S->states);
        S->next = malloc(sb->count * sizeof *S->next);
        states[i] = S;
    }
    
    for(size_t i = 0; i < n; ++i) {
        StateBuilder* sb = mb->sbuild[i];
        State* S = states[i];
        for(size_t j = 0; j < sb->count; ++j) {
            Rule* r = &sb->rules[j];
            S->inps[j] = r->in;
            S->outs[j] = r->out;
            S->next[j] = (short)r->move;
            StateBuilder* nsb = find_state(mb, r->next_name);
            size_t idx = 0;
            
            for(; idx < n; ++idx) if(mb->sbuild[idx] == nsb) break;

            S->states[j] = idx < n ? states[idx] : NULL;
        }
    }
    size_t inputSize = strlen(mb->input);
    size_t inputStart = 0;
    char* tape = strdup(mb->input);
    Machine* M = machine_create(n, states, inputSize, inputStart, tape, mb->blank);

    free(states);

    return M;
}