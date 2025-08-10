#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class BranchBoundSearch {
private:
  vector<vector<pair<int, int>>> adj;
  int nodes;

  struct Node {
    int id;
    int costSoFar;
    vector<int> path;

    bool operator>(const Node& other) const {
      return costSoFar > other.costSoFar;
    }
  };

public:
  BranchBoundSearch() : nodes(7) {
    adj.resize(7);
    adj[0] = {{1,3}, {2,5}};
    adj[1] = {{2,4}, {4,3}};
    adj[2] = {{1,4}, {3,4}};
    adj[3] = {{5,6}};
    adj[4] = {{6,5}};
  }

  void findPath(int start, int goal) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({start, 0, {start}});

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
        pq.push({neighbor, newCost, newPath});
      }
    }

    cout << "No path found.\n";
  }
};

int main() {
  BranchBoundSearch bnb;
  bnb.findPath(0, 6);
  return 0;
}
