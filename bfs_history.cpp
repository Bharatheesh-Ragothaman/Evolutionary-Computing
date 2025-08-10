#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;

class BfsHistorySearch {
    private:
        vector<vector<int>> adj;
        int nodes;
        vector<int> explorationOrder;
        vector<int> levelOrder;
    
    public:
        BfsHistorySearch() : nodes(7){
            adj.resize(7);
            adj[0] = {1, 2};
            adj[1] = {2, 4};
            adj[2] = {1, 3};
            adj[3] = {5};
            adj[4] = {6};
        }

        void search(int start, int goal){
            vector<bool> visited(nodes, false);
            vector<int> parent(nodes, -1);
            vector<int> level(nodes, 0);
            queue<int> q;

            explorationOrder.clear();
            levelOrder.clear();

            visited[start] = true;
            level[start] = 0;
            q.push(start);

            while(!q.empty()){
                int current = q.front();
                q.pop();
                
                explorationOrder.push_back(current);
                levelOrder.push_back(level[current]);

                if(current == goal){
                    vector<int> path;
                    int node = goal;
                    while(node != -1){
                        path.push_back(node);
                        node = parent[node];
                    };
                    reverse(path.begin(), path.end());

                    cout<<"Path Found :" <<endl;
                    for(int n : path){
                        cout<<n<<" ";
                    }
                    cout<<endl;

                    return;
                }

                for(int neighbour : adj[current]){
                    if(!visited[neighbour]){
                        visited[neighbour] = true;
                        parent[neighbour] = current;
                        level[neighbour] = level[current] + 1;
                        q.push(neighbour);
                    }
                }
            };

            cout<< "No path found"<<endl;
        }; 
};

int main(){
    BfsHistorySearch BfsHistorySearch;
    BfsHistorySearch.search(0,6);
    return 0;
}