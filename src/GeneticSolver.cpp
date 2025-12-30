#include "GeneticSolver.hpp"
#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>

GeneticSolver::GeneticSolver(GAParams p) : params(p) {
  std::random_device rd;
  rng.seed(rd());
}

Solution GeneticSolver::randomSolution(const Problem &p) {
  Solution s;
  s.path.resize(p.getSize());
  std::iota(s.path.begin(), s.path.end(), 0);
  std::shuffle(s.path.begin(), s.path.end(), rng);
  s.calculateCost(p);
  return s;
}

void GeneticSolver::initializePopulation(std::vector<Solution> &pop,
                                         const Problem &p) {
  pop.resize(params.populationSize);
  for (auto &s : pop) {
    s = randomSolution(p);
  }
}

void GeneticSolver::evaluatePopulation(std::vector<Solution> &pop,
                                       const Problem &p) {
  if (params.numThreads <= 1) {
    for (auto &s : pop) {
      s.calculateCost(p);
    }
  } else {
    std::vector<std::future<void>> futures;
    int batchSize = pop.size() / params.numThreads;

    for (int i = 0; i < params.numThreads; ++i) {
      int start = i * batchSize;
      int end = (i == params.numThreads - 1) ? pop.size() : (i + 1) * batchSize;
      futures.push_back(
          std::async(std::launch::async, [&pop, &p, start, end]() {
            for (int j = start; j < end; ++j) {
              pop[j].calculateCost(p);
            }
          }));
    }
    for (auto &f : futures) {
      f.wait();
    }
  }
}

const Solution &GeneticSolver::selectParent(const std::vector<Solution> &pop) {
  // Tournament selection
  int tournamentSize = 5;
  int bestIdx = -1;
  double bestCost = 1e18;

  for (int i = 0; i < tournamentSize; ++i) {
    int idx = std::uniform_int_distribution<>(0, pop.size() - 1)(rng);
    if (pop[idx].cost < bestCost) {
      bestCost = pop[idx].cost;
      bestIdx = idx;
    }
  }
  return pop[bestIdx];
}

Solution GeneticSolver::crossover(const Solution &p1, const Solution &p2) {
  // OX1 Crossover
  size_t size = p1.path.size();
  Solution child;
  child.path.resize(size, -1);

  int start = std::uniform_int_distribution<>(0, size - 1)(rng);
  int end = std::uniform_int_distribution<>(0, size - 1)(rng);
  if (start > end)
    std::swap(start, end);

  std::vector<bool> inChild(size, false);
  for (int i = start; i <= end; ++i) {
    child.path[i] = p1.path[i];
    inChild[p1.path[i]] = true;
  }

  int current = (end + 1) % size;
  for (int i = 0; i < size; ++i) {
    int p2Idx = (end + 1 + i) % size;
    int candidate = p2.path[p2Idx];
    if (!inChild[candidate]) {
      child.path[current] = candidate;
      current = (current + 1) % size;
    }
  }

  return child;
}

void GeneticSolver::mutate(Solution &s) {
  if (std::uniform_real_distribution<>(0, 1)(rng) < params.mutationRate) {
    // Swap mutation
    int i = std::uniform_int_distribution<>(0, s.path.size() - 1)(rng);
    int j = std::uniform_int_distribution<>(0, s.path.size() - 1)(rng);
    std::swap(s.path[i], s.path[j]);
  }
}

Solution GeneticSolver::solve(const Problem &problem) {
  std::vector<Solution> population;
  initializePopulation(population, problem);

  // Initial evaluation (though init does it indirectly via randomSolution, but
  // randomSolution calls it. evaluatePopulation(population, problem); //
  // Redundant if randomSolution does it but good for safety.

  Solution bestSinceStart = population[0];

  for (int gen = 0; gen < params.maxGenerations; ++gen) {
    std::vector<Solution> newPop;
    newPop.reserve(params.populationSize);

    // Elitism check
    // Sort to find elites (simple implementation, sorting every gen is
    // inefficient but ok for MVP)
    std::sort(
        population.begin(), population.end(),
        [](const Solution &a, const Solution &b) { return a.cost < b.cost; });

    if (population[0].cost < bestSinceStart.cost) {
      bestSinceStart = population[0];
      // std::cout << "Gen " << gen << " New Best: " << bestSinceStart.cost <<
      // std::endl;
    }

    for (int i = 0; i < params.eliteSize; ++i) {
      newPop.push_back(population[i]);
    }

    while (newPop.size() < params.populationSize) {
      const auto &p1 = selectParent(population);
      const auto &p2 = selectParent(population);
      Solution child = crossover(p1, p2);
      mutate(child);
      newPop.push_back(child);
    }

    population = std::move(newPop);
    evaluatePopulation(population, problem);
  }

  std::sort(
      population.begin(), population.end(),
      [](const Solution &a, const Solution &b) { return a.cost < b.cost; });

  if (population[0].cost < bestSinceStart.cost)
    return population[0];
  return bestSinceStart;
}
