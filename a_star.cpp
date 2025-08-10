#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

class AStarSearch {
private:
    struct Node {
        int id;
        int g;
        int f;
        int parent;
        Node(int id = 0, int g = INT_MAX, int f = INT_MAX, int parent = -1)
            : id(id), g(g), f(f), parent(parent) {}
    };

    vector<vector<pair<int, int>>> adj;
    int nodes;
    vector<int> h;  // heuristic values

    int heuristic(int node, int goal) {
        return h[node];
    }

public:
    AStarSearch() : nodes(7) {
        adj.resize(nodes);
        adj[0] = {{1,3}, {2,5}};
        adj[1] = {{2,4}, {4,3}};
        adj[2] = {{1,4}, {3,4}};
        adj[3] = {{5,6}};
        adj[4] = {{6,5}};

        h = {12, 7, 6, 7, 5, 4, 0};
    }

    void findPath(int start, int goal) {
        auto cmp = [](const Node& a, const Node& b) { return a.f > b.f; };
        priority_queue<Node, vector<Node>, decltype(cmp)> openSet(cmp);

        vector<int> gScore(nodes, INT_MAX);
        vector<int> fScore(nodes, INT_MAX);
        vector<int> cameFrom(nodes, -1);
        vector<bool> closedSet(nodes, false);

        gScore[start] = 0;
        fScore[start] = heuristic(start, goal);
        openSet.push(Node(start, gScore[start], fScore[start], -1));

        while (!openSet.empty()) {
            Node current = openSet.top();
            openSet.pop();

            if (closedSet[current.id]) 
                continue;

            if (current.id == goal) {
                vector<int> path;
                int cur = current.id;
                while (cur != -1) {
                    path.push_back(cur);
                    cur = cameFrom[cur];
                }
                reverse(path.begin(), path.end());

                cout << "A* Search Path:\n";
                for (int node : path) cout << node << " ";
                cout << "\nTotal cost: " << gScore[goal] << endl;
                return;
            }

            closedSet[current.id] = true;

            for (const auto& neighbor : adj[current.id]) {
                int nb = neighbor.first;
                int weight = neighbor.second;

                if (closedSet[nb]) 
                    continue;

                int tentative_gScore = gScore[current.id] + weight;
                if (tentative_gScore < gScore[nb]) {
                    cameFrom[nb] = current.id;
                    gScore[nb] = tentative_gScore;
                    fScore[nb] = tentative_gScore + heuristic(nb, goal);
                    openSet.push(Node(nb, gScore[nb], fScore[nb], current.id));
                }
            }
        }

        cout << "No path found!\n";
    }
};

int main() {
    AStarSearch astar;
    astar.findPath(0, 6);
    return 0;
}
