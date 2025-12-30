#pragma once

#include "Problem.hpp"
#include "Solution.hpp"

class Solver {
public:
  virtual ~Solver() = default;
  virtual Solution solve(const Problem &problem) = 0;
};
