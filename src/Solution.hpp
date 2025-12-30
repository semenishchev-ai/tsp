#pragma once

#include "Problem.hpp"
#include <numeric>
#include <vector>

struct Solution {
  std::vector<int> path;
  double cost = 0.0;

  // Recalculate cost based on the problem
  void calculateCost(const Problem &problem) {
    if (path.empty()) {
      cost = 0.0;
      return;
    }

    double currentTime = 0.0;
    cost = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
      double legCost = problem.getCost(path[i], path[i + 1], currentTime);
      cost += legCost;
      currentTime += legCost; // Assuming speed is 1 unit/time roughly, or cost
                              // IS time. If cost is time, then calculating
                              // arrival time is just adding cost.
    }
    // Return to start? TSP usually implies a cycle.
    // If cycle:
    double returnCost = problem.getCost(path.back(), path[0], currentTime);
    cost += returnCost;
  }
};
