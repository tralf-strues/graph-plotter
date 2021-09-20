//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file solve_equation.cpp
//! @date 2021-09-19
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#include <assert.h>
#include <math.h>

#include "solve_equation.h"
#include "float_compare.h"

int32_t solveLinearEquation(float a, float b, float *solution)
{
    assert(isfinite(a));
    assert(isfinite(b));
    assert(solution);

    if (cmpFloat(a, 0) == 0)
    {
        return (cmpFloat(b, 0) == 0) ? EQUATION_INF_SOLUTIONS : 0;
    }
    
    *solution = -b / a;
    return 1;
}

int32_t solveQuadraticEquation(float a, float b, float c, float *solution1, float* solution2)
{
    assert(isfinite(a));
    assert(isfinite(b));
    assert(isfinite(c));

    assert(solution1);
    assert(solution2);
    assert(solution1 != solution2);

    if (cmpFloat(a, 0) == 0)
    {
        return solveLinearEquation(b, c, solution1);
    }

    float discriminant = b * b - 4 * a * c;

    if (cmpFloat(discriminant, 0) < 0)
    {
        return 0;
    }
    else if (cmpFloat(discriminant, 0) == 0)
    {
        *solution1 = -b / (2 * a);
        return 1;
    }

    float sqrtDiscriminant = sqrtf(discriminant);
    *solution1 = (-b - sqrtDiscriminant) / (2 * a);
    *solution2 = (-b + sqrtDiscriminant) / (2 * a);

    return 2;
}