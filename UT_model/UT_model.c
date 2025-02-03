#include <stdio.h>
#include <math.h>
#include "common.h"
#include "model.h"
#include <assert.h>

int main(void)
{
    int status = SUCCESS;
    state_t state;

    printf("Unit tests: UT_model.c\r\n");

    status = model_init(&state);
    assert(status == SUCCESS);

    model_update(0.1, 0.0, 0.0, 0.0, 0.0);

    printf("Unit tests: complete\r\n");

    return SUCCESS;
}
