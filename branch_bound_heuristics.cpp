#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class BranchBoundSearchHeuristics {
private:
  vector<vector<pair<int, int>>> adj;
  vector<int> heuristic;
  int nodes;

  struct Node {
    int id;
    int costSoFar;
    int estTotalCost;
    vector<int> path;

    bool operator>(const Node& other) const {
      return estTotalCost > other.estTotalCost;
    }
  };

public:
  BranchBoundSearchHeuristics() : nodes(7) {
    adj.resize(7);
    adj[0] = {{1,3}, {2,5}};
    adj[1] = {{2,4}, {4,3}};
    adj[2] = {{1,4}, {3,4}};
    adj[3] = {{5,6}};
    adj[4] = {{6,5}};

    heuristic = {12, 7, 6, 7, 5, 4, 0};
  }

  void findPath(int start, int goal) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    pq.push({start, 0, heuristic[start], {start}});

    cout << "Branch and Bound Search:\n";

    while (!pq.empty()) {
      Node current = pq.top();
      pq.pop();

      if (current.id == goal) {
        for (int n : current.path) cout << n << " ";
        cout << "=> Cost: " << current.costSoFar << endl;
        return;
      }

      for (const auto& nb : adj[current.id]) {
        int neighbor = nb.first;
        int weight = nb.second;
        int newCost = current.costSoFar + weight;
        vector<int> newPath = current.path;
        newPath.push_back(neighbor);
        int estCost = newCost + heuristic[neighbor];
        pq.push({neighbor, newCost, estCost, newPath});
      }
    }
  }
};

int main() {
  BranchBoundSearchHeuristics bnb;
  bnb.findPath(0, 6);
  return 0;
}
