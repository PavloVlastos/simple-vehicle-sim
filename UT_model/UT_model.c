#include <stdio.h>
#include <math.h>
#include "common.h"
#include "model.h"

int main(void)
{
    state_t state;

    printf("Unit tests: UT_model.c\r\n");

    model_init(&state);

    // model_update(0.1, 0.0, 0.0, 0.0);

    // printf("Unit tests: complete\r\n");

    return SUCCESS;
}
