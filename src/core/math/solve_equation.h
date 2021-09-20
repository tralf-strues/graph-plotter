//------------------------------------------------------------------------------
//! @brief A collection of equation solvers.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file solve_equation.h
//! @date 2021-09-19
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef SOLVE_EQUATION_H
#define SOLVE_EQUATION_H

#include "inttypes.h"

static const int32_t EQUATION_INF_SOLUTIONS = -1;

int32_t solveLinearEquation(float a, float b, float *solution);
int32_t solveQuadraticEquation(float a, float b, float c, float *solution1, float* solution2);

#endif // SOLVE_EQUATION_H