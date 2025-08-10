#include <iostream>
#include <vector>
#include <queue>
#include<algorithm>

using namespace std;

void bfsAllPaths(int start, int end, vector<vector<int>>& adj) {
    queue<vector<int>> q;
    q.push({start});

    cout << "All paths from " << start << " to " << end << ":\n";

    while (!q.empty()) {
        vector<int> path = q.front();
        q.pop();
        int lastNode = path.back();

        if (lastNode == end) {
            for (int node : path)
                cout << node << " ";
            cout << endl;
        }

        for (int neighbor : adj[lastNode]) {
            if (find(path.begin(), path.end(), neighbor) == path.end()) { // Avoid cycles
                vector<int> newPath = path;
                newPath.push_back(neighbor);
                q.push(newPath);
            }
        }
    }
}

int main() {
    int n = 7; // Number of nodes (0 to 6)
    vector<vector<int>> adj(n);

    adj[0] = {1, 2};
    adj[1] = {4};
    adj[2] = {1, 3};
    adj[3] = {5};
    adj[4] = {6};

    bfsAllPaths(0, 6, adj);

    return 0;
}
