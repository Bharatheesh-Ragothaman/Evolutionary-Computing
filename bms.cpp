#include <iostream>
#include <vector>
using namespace std;

class BritishMuseumSearch {
private:
    vector<vector<int>> adj;
    int nodes;

    void generateAllPaths(int current, int goal, vector<bool>& visited, vector<int>& path, vector<vector<int>>& allPaths) {
        visited[current] = true;
        path.push_back(current);

        if (current == goal) {
            allPaths.push_back(path);
        } else {
            for (int neighbor : adj[current]) {
                if (!visited[neighbor]) {
                    generateAllPaths(neighbor, goal, visited, path, allPaths);
                }
            }
        }

        path.pop_back();
        visited[current] = false;
    }

public:
    BritishMuseumSearch() : nodes(7) {
        adj.resize(7);
        adj[0] = {1, 2};
        adj[1] = {4};
        adj[2] = {1, 3};
        adj[3] = {5};
        adj[4] = {6};
    }

    void exhaustiveSearch(int start, int goal) {
        vector<bool> visited(nodes, false);
        vector<int> path;
        vector<vector<int>> allPaths;

        generateAllPaths(start, goal, visited, path, allPaths);

        cout << "British Museum Search Result:\n";
        if (allPaths.empty()) {
            cout << "No path found!\n";
        } else {
            for (const auto& p : allPaths) {
                for (int n : p) cout << n << " ";
                cout << endl;
            }
        }
    }
};

int main() {
    BritishMuseumSearch bms;
    bms.exhaustiveSearch(0, 6);
    return 0;
}
