#include <stdio.h>
#include "common.h"
#include "lin_alg.h"

#ifndef KLEE
#include <klee.h>
#endif

float sum_vec(int j, float a[2]);

int main(void)
{
    float vector[2] = {0.0};

    float a = 0.0;
    float b = 0.0;

#ifndef KLEE
    klee_make_symbolic(&a, sizeof(a), "a");
    klee_make_symbolic(&b, sizeof(b), "b");
#endif


    vector[0] = a;
    vector[1] = b;

#ifndef KLEE
    klee_make_symbolic(vector, sizeof(vector), "vector");
#endif

    printf("vector norm = %f\r\n", lin_alg_v_norm(vector));

    return 0;
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
