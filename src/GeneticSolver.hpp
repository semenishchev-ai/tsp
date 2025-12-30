#pragma once

#include "Solver.hpp"
#include <algorithm>
#include <mutex>
#include <random>
#include <thread>

struct GAParams {
  int populationSize = 100;
  double mutationRate = 0.1;
  int maxGenerations = 1000;
  int eliteSize = 2;
  int numThreads = 1; // 1 means single threaded
};

class GeneticSolver : public Solver {
public:
  GeneticSolver(GAParams params);
  Solution solve(const Problem &problem) override;

private:
  GAParams params;
  std::mt19937 rng;

  // GA Operators
  void initializePopulation(std::vector<Solution> &pop, const Problem &p);
  void evaluatePopulation(std::vector<Solution> &pop, const Problem &p);
  Solution crossover(const Solution &p1, const Solution &p2);
  void mutate(Solution &s);
  const Solution &selectParent(const std::vector<Solution> &pop);

  // Helper to generate random individual
  Solution randomSolution(const Problem &p);
};
