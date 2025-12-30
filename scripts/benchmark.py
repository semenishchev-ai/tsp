import subprocess
import time
import json
import os
import argparse

def run_solver(solver_path, problem_file, config_file, output_file):
    start_time = time.time()
    result = subprocess.run([solver_path, problem_file, config_file, output_file], 
                            capture_output=True, text=True)
    end_time = time.time()
    
    if result.returncode != 0:
        print(f"Error running solver: {result.stderr}")
        return None
        
    return end_time - start_time

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--solver", default="./build/bin/tsp_solver", help="Path to solver executable")
    parser.add_argument("--tests", default="tests", help="Directory containing test instances")
    args = parser.parse_args()

    if not os.path.exists(args.solver):
        print(f"Solver not found at {args.solver}. Please build the project first.")
        return

    # Ensure output dir exists
    os.makedirs("results", exist_ok=True)

    print(f"{'Problem':<20} | {'Cost':<15} | {'Time (s)':<10}")
    print("-" * 55)

    for filename in sorted(os.listdir(args.tests)):
        if not filename.endswith(".txt"): continue
        
        problem_path = os.path.join(args.tests, filename)
        output_path = os.path.join("results", f"sol_{filename}.json")
        
        # Run with default config (or you could vary configs here)
        config_path = "config/params.json"
        
        runtime = run_solver(args.solver, problem_path, config_path, output_path)
        
        if runtime is not None:
             with open(output_path, 'r') as f:
                 data = json.load(f)
                 cost = data.get('cost', -1)
             
             print(f"{filename:<20} | {cost:<15.2f} | {runtime:<10.4f}")

if __name__ == "__main__":
    main()
