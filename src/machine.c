#include "machine.h"
#include <stdio.h>
#include <unistd.h>

#define REEL_SIZE 1024

/// @brief creates a mchine from the given inputs
/// @param totStates total number of states
/// @param states pointer to an array of pointers holding the addresses of the states
/// @param inputSize length of the starting data on the reel
/// @param inputStart initial index of the reel
/// @param input pointer to the input
/// @param blank the char to represent a blank tile on the reel
/// @return pointer to the machine created
Machine* machine_create(size_t totStates, State** states, size_t inputSize, size_t inputStart, char* input, char blank, double sleep) {
    Machine* M = malloc(sizeof *M);
    M->pos = inputStart;
    M->state = states[0];
    M->totStates = totStates;
    M->blank = blank;
    M->reel = malloc((REEL_SIZE + 2) * sizeof *M->reel);
    M->sleep = sleep;
    
    for(size_t i = 0; i < REEL_SIZE + 2; ++i) M->reel[i] = blank;
    for(size_t i = 0; i < inputSize; ++i) M->reel[inputStart + i] = input[i];
    
    return M;
}

/// @brief frees the memory occupied by a machine
/// @param M pointer to the machine to be freed from memory
void machine_free(Machine* M) {
    free(M->reel);
    free(M);
}

/// @brief print 10 tiles of the reel either side of the "scanner" 
/// @param M the machine whose reel is to be printed
void machine_print_reel(Machine* M) {
    printf("\e[1;1H\e[2J");
    
    for(size_t i = M->pos > 10 ? M->pos - 10 : 0; i < M->pos + 10; ++i) putchar(M->reel[i]);
    putchar('\n');
    for(size_t i = M->pos > 10 ? M->pos - 10 : 0; i < M->pos; ++i) putchar(' ');
    printf("^\n\nCurrent State - %s:\n\n", M->state->name);
    printf("Read\tWrite\tMove\tState\n\n");
    for(size_t i = 0; i < M->state->totRules; ++i) {
        printf("%c\t%c\t%d\t%s\n", M->state->inps[i],
        M->state->outs[i], M->state->next[i], M->state->states[i]->name);
    }

    sleep(M->sleep);
}

/// @brief read the current tile, find the correct rule, write, change state, and move as given by the rules
/// @param M pointer to the machine that is reading the current reel
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

/// @brief first step of the machine
/// @param M pointer to the machine to start
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