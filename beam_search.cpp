#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class BeamSearch {
private:
  vector<vector<pair<int,int>>> adj;
  vector<int> heuristic;
  int nodes;
  int beamWidth;

public:
  BeamSearch(int beamWidth=2) : nodes(7), beamWidth(beamWidth) {
    adj.resize(7);
    adj[0] = {{1,3}, {2,5}};
    adj[1] = {{2,4}, {4,3}};
    adj[2] = {{1,4}, {3,4}};
    adj[3] = {{5,6}};
    adj[4] = {{6,5}};

    heuristic = {12, 7, 6, 7, 5, 4, 0};
  }

  void findPath(int start, int goal) {
    struct Path {
      vector<int> nodes;
      int costSoFar;
      int estimatedTotal;

      bool operator<(const Path& other) const {
        return estimatedTotal > other.estimatedTotal;
      }
    };

    vector<Path> beam = { { {start}, 0, heuristic[start] } };

    while (!beam.empty()) {
      vector<Path> candidates;

      for (const auto& path : beam) {
        int current = path.nodes.back();

        if (current == goal) {
          cout << "Beam Search with Heuristic Path:\n";
          for (int node : path.nodes) cout << node << " ";
          cout << "\nTotal cost: " << path.costSoFar << endl;
          return;
        }

        for (const auto& nb : adj[current]) {
          int neighbor = nb.first;
          int weight = nb.second;

          if (find(path.nodes.begin(), path.nodes.end(), neighbor) != path.nodes.end())
            continue;

          vector<int> newNodes = path.nodes;
          newNodes.push_back(neighbor);
          int newCost = path.costSoFar + weight;
          int estTotal = newCost + heuristic[neighbor];

          candidates.push_back({ newNodes, newCost, estTotal });
        }
      }

      if (candidates.empty()) break;

      sort(candidates.begin(), candidates.end());
      if (candidates.size() > beamWidth)
        candidates.resize(beamWidth);

      beam = candidates;
    }

    cout << "No path found!\n";
  }
};

int main() {
  BeamSearch bms(2);
  bms.findPath(0, 6);
  return 0;
}