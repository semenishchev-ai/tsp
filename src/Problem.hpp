#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

struct Point {
    double x, y;
};

class Problem {
public:
    Problem() = default;
    
    // Load problem from a file (format to be defined, basic text for now)
    bool loadFromFile(const std::string& filename);

    // Get travel cost between two nodes given a departure time
    // For MVP: Time dependency can be a multiplier based on time/1000 or similar
    double getCost(int from, int to, double departureTime) const;

    // Get simple Euclidean distance (static)
    double getDistance(int from, int to) const;

    size_t getSize() const { return coordinates.size(); }
    const std::vector<Point>& getCoordinates() const { return coordinates; }

private:
    std::vector<Point> coordinates;
    // Potentially other parameters for time dependency
};
