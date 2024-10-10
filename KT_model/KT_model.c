#include <stdio.h>
#include <klee.h>

#include "common.h"
#include "model.h"

int main(void) {
    float dt = 0.1;
    float speed = 2.0; 
    float rudder_cmd = 1.0;
    
    state_t s;

    model_init(&s);

    klee_make_symbolic(&dt, sizeof(dt), "dt");
    klee_make_symbolic(&speed, sizeof(speed), "speed");
    klee_make_symbolic(&rudder_cmd, sizeof(rudder_cmd), "rudder_cmd");

    return model_update(dt, rudder_cmd, speed, 0.0, 0.0);
}