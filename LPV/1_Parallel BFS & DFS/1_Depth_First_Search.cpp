#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

const int MAX = 100000;
vector<int> graph[MAX];
bool visited[MAX];
omp_lock_t lock[MAX];

// Function to perform DFS
void dfs(int start_node)
{
    stack<int> s;
    s.push(start_node);

    while (!s.empty())
    {
        int curr_node = s.top();
        s.pop();

        // Locking to avoid race condition on visited array
        omp_set_lock(&lock[curr_node]);
        if (!visited[curr_node])
        {
            visited[curr_node] = true;
            cout << curr_node << " ";
        }
        omp_unset_lock(&lock[curr_node]);

// Explore neighbors in parallel
#pragma omp parallel for shared(s)
        for (int i = 0; i < graph[curr_node].size(); i++)
        {
            int adj_node = graph[curr_node][i];

            omp_set_lock(&lock[adj_node]);
            if (!visited[adj_node])
            {
// Push to stack in critical section to prevent data race
#pragma omp critical
                {
                    s.push(adj_node);
                }
            }
            omp_unset_lock(&lock[adj_node]);
        }
    }
}

int main()
{
    int n, m, start_node;

    cout << "Enter number of nodes, edges, and the starting node: ";
    cin >> n >> m >> start_node;

    cout << "Enter pairs of connected edges (u v):\n";
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;

        // Undirected graph
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

// Initialize visited array and locks
#pragma omp parallel for
    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
        omp_init_lock(&lock[i]);
    }

    cout << "\nDFS Traversal Order:\n";
    dfs(start_node);
    cout << endl;

    // Destroy locks
    for (int i = 0; i < n; i++)
    {
        omp_destroy_lock(&lock[i]);
    }

    return 0;
}

// Run Commands (Ubuntu Terminal):
// g++ -fopenmp  1_Depth_First_Search.cpp -o parallel_dfs
//  ./parallel_dfs

// Run Commands (VS Code):
// g++ -fopenmp -o parallel_dfs 1_Depth_First_Search.cpp
// .\parallel_dfs

// Output Example:
// Enter number of nodes, edges, and the starting node: 6 5 0
// Enter pairs of connected edges (u v):
// 0 1
// 0 2
// 1 3
// 1 4
// 2 5

// DFS Traversal Order:
// 0 1 4 3 2 5