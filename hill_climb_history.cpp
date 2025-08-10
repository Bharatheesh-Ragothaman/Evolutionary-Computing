#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

class HillClimb {
private:
  vector<vector<pair<int, int>>> adj;
  vector<int> heuristic;
  int nodes;
  int totalWeight = 0;

public:
  HillClimb() : nodes(7) {
    adj.resize(7);
    adj[0] = {{1,3}, {2,5}};
    adj[1] = {{2,4}, {4,3}};
    adj[2] = {{1,4}, {3,4}};
    adj[3] = {{5,6}};
    adj[4] = {{6,5}};

    heuristic = {12, 7, 6, 7, 5, 4, 0};
  }

  bool hillClimb(int current, int goal, unordered_set<int>& history, vector<int>& path, int currentWeight) {
    history.insert(current);
    path.push_back(current);

    if (current == goal) {
      totalWeight = currentWeight;
      return true;
    }

    vector<pair<int, int>> neighbors = adj[current];
    sort(neighbors.begin(), neighbors.end(), [&](const auto& a, const auto& b) {
      return (a.second + heuristic[a.first]) < (b.second + heuristic[b.first]);
    });

    for (const auto& nb : neighbors) {
      int neighbor = nb.first;
      int weight = nb.second;
      if (history.find(neighbor) == history.end()) {
        if (hillClimb(neighbor, goal, history, path, currentWeight + weight))
          return true;
      }
    }

    path.pop_back();
    history.erase(current);
    return false;
  }

  void findPath(int start, int goal) {
    unordered_set<int> history;
    vector<int> path;
    totalWeight = 0;

    if (hillClimb(start, goal, history, path, 0)) {
      cout << "Hill Climbing Path (with history & heuristic):\n";
      for (int node : path) cout << node << " ";
      cout << "\nTotal weight: " << totalWeight << endl;
    } else {
      cout << "No path found!\n";
    }
  }
};

int main() {
  HillClimb bms;
  bms.findPath(0, 6);
  return 0;
}
