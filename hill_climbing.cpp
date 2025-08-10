#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

class HillClimb{
    private:
        vector<vector<pair<int, int>>> adj;
        vector<int> heuristic;
        int nodes;
        int totalWeight = 0;

    public:
        HillClimb() : nodes(7){
            adj.resize(7);
            adj[0] = {{1,3}, {2,5}};
            adj[1] = {{2,4}, {4,3}};
            adj[2] = {{1,4}, {3,4}};
            adj[3] = {{5,6}};
            adj[4] = {{6,5}};

            heuristic = {12, 7, 6, 7, 5, 4, 0};
        }
    bool hillClimb(int current, int goal, vector<bool>& visited, vector<int>& path, int currentWeight) {
    visited[current] = true;
    path.push_back(current);

    if (current == goal) {
      totalWeight = currentWeight;
      return true;
    }

    vector<pair<int, int>> neighbors = adj[current];

    sort(neighbors.begin(), neighbors.end(), [&](const auto& a, const auto& b) {
      int f1 = a.second + heuristic[a.first];
      int f2 = b.second + heuristic[b.first];
      return f1 < f2;
    });

    for (const auto& nb : neighbors) {
      int neighbor = nb.first;
      int weight = nb.second;
      if (!visited[neighbor]) {
        if (hillClimb(neighbor, goal, visited, path, currentWeight + weight))
          return true;
      }
    }

    path.pop_back();
    visited[current] = false;
    return false;
  }

  void findPath(int start, int goal) {
    vector<bool> visited(nodes, false);
    vector<int> path;
    totalWeight = 0;

    if (hillClimb(start, goal, visited, path, 0)) {
      cout << "Hill Climbing with Heuristic Path:\n";
      for (int node : path) cout << node << " ";
      cout << "\nTotal weight: " << totalWeight << endl;
    } else {
      cout << "No path found!\n";
    }
  }
};

int main() {
  HillClimb hc;
  hc.findPath(0, 6);
  return 0;
}   