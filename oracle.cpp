#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class OracleSearch {
private:
  vector<vector<pair<int, int>>> adj;
  int nodes;

  void dfs(int current, int goal, vector<bool>& visited, vector<int>& path, int cost) {
    visited[current] = true;
    path.push_back(current);

    if (current == goal) {
      for (int node : path) cout << node << " ";
      cout << "=> Cost: " << cost << endl;
    } else {
      for (const auto& nb : adj[current]) {
        int neighbor = nb.first;
        int weight = nb.second;
        if (!visited[neighbor]) {
          dfs(neighbor, goal, visited, path, cost + weight);
        }
      }
    }

    path.pop_back();
    visited[current] = false;
  }

public:
  OracleSearch() : nodes(7) {
    adj.resize(7);
    adj[0] = {{1,3}, {2,5}};
    adj[1] = {{2,4}, {4,3}};
    adj[2] = {{1,4}, {3,4}};
    adj[3] = {{5,6}};
    adj[4] = {{6,5}};
  }

  void findAllPaths(int start, int goal) {
    vector<bool> visited(nodes, false);
    vector<int> path;
    cout << "Oracle Search:\n";
    dfs(start, goal, visited, path, 0);
  }
};

int main() {
  OracleSearch oracle;
  oracle.findAllPaths(0, 6);
  return 0;
}
