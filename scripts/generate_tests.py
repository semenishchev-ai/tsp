import argparse
import random
import numpy as np
import os

def generate_random(n, width, height, seed):
    random.seed(seed)
    points = []
    for _ in range(n):
        points.append((random.uniform(0, width), random.uniform(0, height)))
    return points

def generate_clustered(n, width, height, clusters, seed):
    np.random.seed(seed)
    points = []
    centers = []
    for _ in range(clusters):
        centers.append((np.random.uniform(0, width), np.random.uniform(0, height)))
    
    points_per_cluster = n // clusters
    for cx, cy in centers:
        # Generate points around the center with Gaussian distribution
        # sigma is heuristic based on width/clusters
        sigma = width / (clusters * 4) 
        x_pts = np.random.normal(cx, sigma, points_per_cluster)
        y_pts = np.random.normal(cy, sigma, points_per_cluster)
        for x, y in zip(x_pts, y_pts):
            points.append((np.clip(x, 0, width), np.clip(y, 0, height)))
            
    # Handle remainder
    remainder = n - len(points)
    for _ in range(remainder):
         points.append((np.random.uniform(0, width), np.random.uniform(0, height)))
         
    return points

def save_points(points, filename):
    with open(filename, 'w') as f:
        for x, y in points:
            f.write(f"{x:.4f} {y:.4f}\n")
    print(f"Saved {len(points)} points to {filename}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate TSP instances")
    parser.add_argument("N", type=int, help="Number of cities")
    parser.add_argument("--type", choices=['random', 'clustered'], default='random', help="Generation type")
    parser.add_argument("--width", type=float, default=100.0, help="Width of the area")
    parser.add_argument("--height", type=float, default=100.0, help="Height of the area")
    parser.add_argument("--clusters", type=int, default=5, help="Number of clusters (only for clustered type)")
    parser.add_argument("--seed", type=int, default=42, help="Random seed")
    parser.add_argument("--output", type=str, default="problem.txt", help="Output filename")

    args = parser.parse_args()

    if args.type == 'random':
        points = generate_random(args.N, args.width, args.height, args.seed)
    else:
        points = generate_clustered(args.N, args.width, args.height, args.clusters, args.seed)

    save_points(points, args.output)
