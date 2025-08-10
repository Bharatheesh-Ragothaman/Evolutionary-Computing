#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

class BeamSearch {
private:
  vector<vector<pair<int, int>>> adj;
  vector<int> heuristic;
  int nodes;
  int beamWidth;

public:
  BeamSearch(int beamWidth = 2) : nodes(7), beamWidth(beamWidth) {
    adj.resize(7);
    adj[0] = {{1,3}, {2,5}};
    adj[1] = {{2,4}, {4,3}};
    adj[2] = {{1,4}, {3,4}};
    adj[3] = {{5,6}};
    adj[4] = {{6,5}};

    heuristic = {12, 7, 6, 7, 5, 4, 0};
  }

  struct Path {
    vector<int> nodes;
    unordered_set<int> visited;
    int cost;

    int totalCostToGoal(int goal, const vector<int>& heuristic) const {
      return cost + heuristic[nodes.back()];
    }
  };

  void findPath(int start, int goal) {
    vector<Path> beam = { { {start}, {start}, 0 } };

    while (!beam.empty()) {
      vector<Path> candidates;

      for (const auto& path : beam) {
        int current = path.nodes.back();

        if (current == goal) {
          cout << "Beam Search Path (with heuristic & history):\n";
          for (int node : path.nodes) cout << node << " ";
          cout << "\nTotal cost: " << path.cost << endl;
          return;
        }

        for (const auto& nb : adj[current]) {
          int neighbor = nb.first;
          int weight = nb.second;

          if (path.visited.count(neighbor)) continue;

          vector<int> newNodes = path.nodes;
          newNodes.push_back(neighbor);

          unordered_set<int> newVisited = path.visited;
          newVisited.insert(neighbor);

          int newCost = path.cost + weight;
          candidates.push_back({ newNodes, newVisited, newCost });
        }
      }

      if (candidates.empty()) break;

      sort(candidates.begin(), candidates.end(), [&](const Path& a, const Path& b) {
        return a.totalCostToGoal(goal, heuristic) < b.totalCostToGoal(goal, heuristic);
      });

      if ((int)candidates.size() > beamWidth)
        candidates.resize(beamWidth);

      beam = candidates;
    }

    cout << "No path found!\n";
  }
};

int main() {
  BeamSearch bs(2);
  bs.findPath(0, 6);
  return 0;
}
