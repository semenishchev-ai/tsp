#include "Problem.hpp"
#include <sstream>

bool Problem::loadFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return false;

  coordinates.clear();
  double x, y;
  // Simple format: X Y per line
  while (file >> x >> y) {
    coordinates.push_back({x, y});
  }
  return true;
}

double Problem::getDistance(int from, int to) const {
  if (from < 0 || from >= coordinates.size() || to < 0 ||
      to >= coordinates.size())
    return 1e9;
  double dx = coordinates[from].x - coordinates[to].x;
  double dy = coordinates[from].y - coordinates[to].y;
  return std::sqrt(dx * dx + dy * dy);
}

double Problem::getCost(int from, int to, double departureTime) const {
  double dist = getDistance(from, to);

  // Simple time dependency: congestion increases cost during "rush hour"
  // Let's assume time is periodic or just affects cost
  // Example: cost = dist * (1 + 0.5 * sin(departureTime / 100.0))
  // ensuring weight is always positive
  double timeFactor = 1.0 + 0.5 * std::sin(departureTime / 10.0);
  // Make sure it doesn't go below 1.0 (optional, but realistic)
  if (timeFactor < 1.0)
    timeFactor = 1.0;

  // For now, let's keep it simple or make it effectively static if we want to
  // test that first return dist; // Uncomment for static
  return dist * timeFactor;
}
