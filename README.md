# Time-Dependent TSP Solver

This project implements a genetic algorithm to solve the Time-Dependent Traveling Salesman Problem (TD-TSP). It features a modular C++ solver and Python tools for data generation and visualization.

## Features

- **Genetic Algorithm**: Custom implementation with Order Crossover (OX1), Swap Mutation, and Tournament Selection.
- **Multithreading**: Parallel evaluation of population fitness using standard C++ threads.
- **Time-Dependency**: Framework supports time-dependent costs (currently implemented as a simple periodic congestion function).
- **Python Tools**: Scripts for generating random/clustered datasets and visualizing results.
- **Configurable**: Hyperparameters can be tuned via a JSON configuration file.

## Requirements

### C++
- C++17 compliant compiler (GCC, Clang, MSVC)
- CMake 3.14+

### Python
- Python 3.x
- `matplotlib`
- `numpy`

## Building

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
2. Configure with CMake:
   ```bash
   cmake ..
   ```
3. Build the executable:
   ```bash
   make
   ```
   The executable `tsp_solver` will be placed in `build/bin/`.

## Usage

### 1. Generate a Test Instance
Use the python script to generate a problem file (list of coordinates).
```bash
python3 scripts/generate_tests.py 50 --type clustered --output tests/problem.txt
```

### 2. Run the Solver
Run the C++ solver with the problem file and optional configuration.
```bash
./build/bin/tsp_solver tests/problem.txt config/params.json solution.json
```

**Configuration (`config/params.json`):**
```json
{
    "populationSize": 200,
    "mutationRate": 0.05,
    "maxGenerations": 2000,
    "eliteSize": 5,
    "numThreads": 4
}
```

### 3. Visualize the Result
Visualize the generated solution path.
```bash
python3 scripts/visualize.py tests/problem.txt solution.json --output result.png
```

## Project Structure

- `src/`: C++ source code.
  - `Problem`: Graphic and cost logic.
  - `GeneticSolver`: GA implementation.
- `scripts/`: Python utility scripts.
- `config/`: Configuration files.
- `tests/`: Directory for test instances.
