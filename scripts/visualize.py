import argparse
import json
import matplotlib.pyplot as plt
import os

def load_problem(filename):
    points = []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 2:
                points.append((float(parts[0]), float(parts[1])))
    return points

def load_solution(filename):
    with open(filename, 'r') as f:
        data = json.load(f)
    return data

def plot_tsp(points, solution_path, cost, output_file=None):
    x = [p[0] for p in points]
    y = [p[1] for p in points]

    plt.figure(figsize=(10, 10))
    
    # Draw path
    if solution_path:
        path_x = []
        path_y = []
        for i, idx in enumerate(solution_path):
            px, py = points[idx]
            path_x.append(px)
            path_y.append(py)
            
            # Annotate order
            plt.annotate(str(i), (px, py), textcoords="offset points", xytext=(0,5), ha='center', fontsize=8, color='darkblue')
            
        # Close loop
        path_x.append(points[solution_path[0]][0])
        path_y.append(points[solution_path[0]][1])
        
        plt.plot(path_x, path_y, 'b-', linewidth=1, zorder=1, alpha=0.7)
        
        # Plot points on top
        plt.scatter(x, y, c='red', s=40, zorder=2)
        
        # Mark start
        start_idx = solution_path[0]
        plt.scatter([points[start_idx][0]], [points[start_idx][1]], c='green', s=100, zorder=3, label='Start')
        plt.legend()

    plt.title(f"TSP Solution (Cost: {cost:.2f})")
    plt.xlabel("X")
    plt.ylabel("Y")
    
    if output_file:
        plt.savefig(output_file)
        print(f"Saved visualization to {output_file}")
    else:
        plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Visualize TSP Solution")
    parser.add_argument("problem_file", help="Path to problem file (txt)")
    parser.add_argument("solution_file", help="Path to solution file (json)")
    parser.add_argument("--output", help="Output image file (optional, displays if not provided)")
    
    args = parser.parse_args()

    points = load_problem(args.problem_file)
    sol_data = load_solution(args.solution_file)
    
    plot_tsp(points, sol_data['path'], sol_data['cost'], args.output)
