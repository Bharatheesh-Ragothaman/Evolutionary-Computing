#include<iostream>
#include<vector>
#include<set>
using namespace std;

class DfsHistorySearch {
    private:
        vector<vector<int>> adj;
        int nodes;
        vector<int> foundPath;
        set<int> visitedNodes;

        bool DfsWithHistory(int current, int goal, vector<int> path, vector<bool>& visited){
            visited[current] = true;
            path.push_back(current);
            visitedNodes.insert(current);

            if(current == goal){
                foundPath = path;
                return true;
            }

            for (int neighbour : adj[current]){
                if (!visited[neighbour]){
                    if(DfsWithHistory(neighbour, goal, path, visited)){
                        return true;
                    };
                }
            }

            path.pop_back();
            visited[current] = false;
            return false;
        }
    
    public:
        DfsHistorySearch() : nodes(7) {
            adj.resize(7);
            adj[0] = {1, 2};
            adj[1] = {4};
            adj[2] = {1, 3};
            adj[3] = {5};
            adj[4] = {6};
        }

        void search(int start, int goal){
            vector<bool> visited(nodes, false);
            vector<int> path;
            foundPath.clear();
            visitedNodes.clear();
            
            bool pathFound = DfsWithHistory(start, goal, path, visited);
            cout<< "DFS with History" <<endl;
            if(pathFound){
                cout<<"Path Found :"<<endl;
                for(int i : foundPath){
                    cout<< i << " ";
                }
            }
            else{
                cout<<"No path found"<<endl;
            }
        }
};

int main(){
    DfsHistorySearch DfsHistorySearch;
    DfsHistorySearch.search(0,6);
    return 0;
}