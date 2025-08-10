#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

class DFSBFSSearch {
private:
    vector<vector<int>> adj;
    int nodes;
    
    bool dfsPhase(int start, int goal, int depth, vector<bool>& visited, vector<int>& path) {
        if (depth == 0) return false;
        
        visited[start] = true;
        path.push_back(start);
        
        if (start == goal) return true;
        
        for (int neighbor : adj[start]) {
            if (!visited[neighbor]) {
                if (dfsPhase(neighbor, goal, depth - 1, visited, path)) {
                    return true;
                }
            }
        }
        
        path.pop_back();
        visited[start] = false;
        return false;
    }
    
public:
    DFSBFSSearch() : nodes(7) {
        adj.resize(7);
        adj[0] = {1, 2};
        adj[1] = {4};
        adj[2] = {1, 3};
        adj[3] = {5};
        adj[4] = {6};
    }
    
    void search(int start, int goal) {
        for (int depth = 1; depth <= nodes; depth++) {
            vector<bool> visited(nodes, false);
            vector<int> path;
            
            if (dfsPhase(start, goal, depth, visited, path)) {
                cout << "DFS+BFS Result:" << endl;
                cout << "Path found: ";
                for (int node : path) {
                    cout << node << " ";
                }
                cout << endl;
                return;
            }
        }
        
        cout << "No path found!" << endl;
    }
};

int main() {
    DFSBFSSearch dfsbfs;
    dfsbfs.search(0, 6);
    return 0;
}
