#include "builder.h"

int main(void) {
    MachineBuilder* MB = builder_create("1011", ' ');

    StateBuilder* s_right = builder_add_state(MB, "RIGHT");
    StateBuilder* s_carry = builder_add_state(MB, "CARRY");
    builder_add_state(MB, "DONE");

    builder_add_rule(s_right, '1', '1', DIR_R, "RIGHT");
    builder_add_rule(s_right, '0', '0', DIR_R, "RIGHT");
    builder_add_rule(s_right, ' ', ' ', DIR_L, "CARRY");

    builder_add_rule(s_carry, '1', '0', DIR_L, "CARRY");
    builder_add_rule(s_carry, '0', '1', DIR_L, "DONE");
    builder_add_rule(s_carry, ' ', '1', DIR_L, "DONE");

    Machine* M = builder_build(MB);
    machine_start(M);
    machine_free(M);
    builder_free(MB);

    return 0;
}