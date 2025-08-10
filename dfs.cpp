#include <iostream>
#include <vector>
using namespace std;

const int N = 7;
vector<int> adj[N];
vector<int> bestPath;
int minLength = 1e9;

void dfsOptimalPath(int current, int goal, vector<bool>& visited, vector<int>& path) {
    visited[current] = true;
    path.push_back(current);

    if (current == goal) {
        if (path.size() < minLength) {
            minLength = path.size();
            bestPath = path;
        }
    } else {
        for (int neighbor : adj[current]) {
            if (!visited[neighbor]) {
                dfsOptimalPath(neighbor, goal, visited, path);
            }
        }
    }

    path.pop_back();             // Backtrack
    visited[current] = false;   // Allow node reuse in different path
}

int main() {
    // Adjacency list setup
    adj[0] = {1, 2};
    adj[1] = {4};
    adj[2] = {1, 3};
    adj[3] = {5};
    adj[4] = {6};

    int start = 0, goal = 6;
    vector<bool> visited(N, false);
    vector<int> path;

    dfsOptimalPath(start, goal, visited, path);

    if (bestPath.empty()) {
        cout << "No path found from " << start << " to " << goal << ".\n";
    } else {
        cout << "Optimal path (shortest DFS path) from " << start << " to " << goal << ":\n";
        for (int node : bestPath) {
            cout << node << " ";
        }
        cout << "\nLength: " << bestPath.size() - 1 << " edges\n";
    }

    return 0;
}
