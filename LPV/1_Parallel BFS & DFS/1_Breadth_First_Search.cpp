#include <iostream>
#include <queue>
#include <omp.h> // Required for OpenMP directives
using namespace std;

class Node
{
public:
    Node *left, *right;
    char data;
};

class BreadthFS
{
public:
    Node *insert(Node *root, char data); // Insert a node into the tree
    void bfs(Node *root);                // Perform parallel BFS
};

// Insert a new node using level-order insertion
Node *BreadthFS::insert(Node *root, char data)
{
    if (!root)
    {
        root = new Node;
        root->left = nullptr;
        root->right = nullptr;
        root->data = data;
        return root;
    }

    std::queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        Node *current = q.front();
        q.pop();

        if (!current->left)
        {
            current->left = new Node;
            current->left->left = nullptr;
            current->left->right = nullptr;
            current->left->data = data;
            return root;
        }
        else
        {
            q.push(current->left);
        }

        if (!current->right)
        {
            current->right = new Node;
            current->right->left = nullptr;
            current->right->right = nullptr;
            current->right->data = data;
            return root;
        }
        else
        {
            q.push(current->right);
        }
    }
    return root; // Ensures all control paths return a value
}

// Parallel BFS using OpenMP
void BreadthFS::bfs(Node *root)
{
    if (!root)
        return;

    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        int level_size = q.size();

#pragma omp parallel for // Parallelize processing of nodes at the current level
        for (int i = 0; i < level_size; i++)
        {
            Node *current = nullptr;

#pragma omp critical // Thread-safe access to the queue
            {
                current = q.front();
                q.pop();
                cout << current->data << "\t";
            }

#pragma omp critical // Thread-safe insertion of children
            {
                if (current->left)
                    q.push(current->left);
                if (current->right)
                    q.push(current->right);
            }
        }
    }
}

int main()
{
    BreadthFS bfs;
    Node *root = nullptr;
    char data;
    char choice;

    // cout << "\n\nName: Shriharsh Deshmukh\nRoll No.62 \t Div.A\n\n";

    do
    {
        cout << "Enter data: ";
        cin >> data;
        root = bfs.insert(root, data);
        cout << "Insert another node? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "BFS Traversal:\n";
    bfs.bfs(root);

    return 0;
}

// Run Commands (Ubuntu Terminal):
// g++ -fopenmp  1_Breadth_First_Search.cpp -o parallel_bfs
//  ./parallel_bfs

// Run Commands (VS Code):
// g++ -fopenmp -o parallel_bfs 1_Breadth_First_Search.cpp
// .\parallel_bfs

// Example:
// Enter data: a
// Insert another node? (y/n): y
// Enter data: b
// Insert another node? (y/n): y
// Enter data: c
// Insert another node? (y/n): y
// Enter data: d
// Insert another node? (y/n): y
// Enter data: e
// Insert another node? (y/n): n
// BFS Traversal:
// a       b       c       d       e