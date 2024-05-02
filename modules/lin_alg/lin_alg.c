#include "lin_alg.h"

float lin_alg_v_norm(const float vector[DIM2]) {
    return sqrtf(vector[0] * vector[0] + vector[1] * vector[1]);
}

int lin_alg_m_m_mul(float m_a[DIM2][DIM2], float m_b[DIM2][DIM2], float m_out[DIM2][DIM2])
{
    int i = 0;
    int j = 0;
    int k = 0;

    for (i = 0; i < DIM2; i++) {
        for (j = 0; j < DIM2; j++) {
            m_out[i][j] = 0.0; 

            for (k = 0; k < DIM2; k++) {
                m_out[i][j] += m_a[i][k] * m_b[k][j];
            }
        }
    }
    
    return SUCCESS;
}