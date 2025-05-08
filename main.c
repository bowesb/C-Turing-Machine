#include "builder.h"

int main(void) {
    MachineBuilder* mb = builder_create("1011", ' ');

    StateBuilder* s_right = builder_add_state(mb, "RIGHT");
    StateBuilder* s_carry = builder_add_state(mb, "CARRY");
    builder_add_state(mb, "DONE");

    builder_add_rule(s_right, '1', '1', DIR_R, "RIGHT");
    builder_add_rule(s_right, '0', '0', DIR_R, "RIGHT");
    builder_add_rule(s_right, ' ', ' ', DIR_L, "CARRY");

    builder_add_rule(s_carry, '1', '0', DIR_L, "CARRY");
    builder_add_rule(s_carry, '0', '1', DIR_L, "DONE");
    builder_add_rule(s_carry, ' ', '1', DIR_L, "DONE");

    Machine* M = builder_build(mb);
    machine_start(M);
    machine_free(M);
    builder_free(mb);

    return 0;
}