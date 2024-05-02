#include <stdio.h>
#include "common.h"
#include "lin_alg.h"

float sum_vec(int j, float a[2]);

int main(void)
{

    float vector[] = {3.0, -4.0};
    printf("This is an example secondary executable\r\n");

    lin_alg_v_norm(vector);

    return SUCCESS;
}

float sum_vec(int j, float a[2])
{
    float sum = 0;
    int i;
    for (i = 0; i < j; i++)
    {
        sum += a[i];
    }
    return sum;
}
