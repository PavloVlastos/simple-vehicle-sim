#include <stdio.h>
#include <math.h>
#include "common.h"
#include "model.h"
#include <assert.h>

int main(void)
{
    int status = SUCCESS;
    state_t intial_state;
    state_t state;

    printf("Unit tests: UT_model.c\r\n");

    status = model_init(&intial_state, &state);
    assert(status == SUCCESS);

    status = model_update(&state, 0.1, 0.0, 0.0, 0.0, 0.0);

    assert(status == SUCCESS);

    printf("Unit tests: complete\r\n");

    return SUCCESS;
}
