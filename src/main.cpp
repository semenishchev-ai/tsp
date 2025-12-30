#include "GeneticSolver.hpp"
#include "Problem.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0]
              << " <problem_file> [config_file] [output_file]" << std::endl;
    return 1;
  }

  std::string problemFile = argv[1];
  std::string configFile = (argc >= 3) ? argv[2] : "";
  std::string outputFile = (argc >= 4) ? argv[3] : "solution.json";

  // Load Loading Problem
  Problem problem;
  if (!problem.loadFromFile(problemFile)) {
    std::cerr << "Error loading problem file: " << problemFile << std::endl;
    return 1;
  }
  std::cout << "Loaded problem with " << problem.getSize() << " cities."
            << std::endl;

  // Load Config
  GAParams params;
  if (!configFile.empty()) {
    std::ifstream f(configFile);
    if (f.is_open()) {
      json j;
      f >> j;
      if (j.contains("populationSize"))
        params.populationSize = j["populationSize"];
      if (j.contains("mutationRate"))
        params.mutationRate = j["mutationRate"];
      if (j.contains("maxGenerations"))
        params.maxGenerations = j["maxGenerations"];
      if (j.contains("eliteSize"))
        params.eliteSize = j["eliteSize"];
      if (j.contains("numThreads"))
        params.numThreads = j["numThreads"];
      std::cout << "Loaded config from " << configFile << std::endl;
    } else {
      std::cerr << "Warning: Could not open config file " << configFile
                << ", using defaults." << std::endl;
    }
  }

  // Solve
  std::cout << "Starting Genetic Algorithm with " << params.numThreads
            << " threads..." << std::endl;
  GeneticSolver solver(params);
  Solution solution = solver.solve(problem);

  std::cout << "Solution found!" << std::endl;
  std::cout << "Cost: " << solution.cost << std::endl;

  // Save Output
  json out;
  out["cost"] = solution.cost;
  out["path"] = solution.path;

  std::ofstream o(outputFile);
  o << out.dump(4) << std::endl;
  std::cout << "Saved solution to " << outputFile << std::endl;

  return 0;
}
