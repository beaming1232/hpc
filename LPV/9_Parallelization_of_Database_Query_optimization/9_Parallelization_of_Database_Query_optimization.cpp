#include <vector>
#include <iostream>
#include <algorithm>
#include <windows.h> // Windows API for threading

struct Query
{
    int id;
    std::vector<int> conditions;
};

std::vector<int> data; // Global data
HANDLE mutex;
int total_matches = 0;

DWORD WINAPI thread_function(LPVOID lpParam);

struct ThreadData
{
    Query *query;
};

int main()
{
    int n;
    std::cout << "Enter total number of data elements: ";
    std::cin >> n;

    data.resize(n);
    std::cout << "Enter data elements:\n";
    for (int i = 0; i < n; i++)
    {
        std::cin >> data[i];
    }

    int num_queries;
    std::cout << "\nEnter number of queries: ";
    std::cin >> num_queries;

    std::vector<Query> queries(num_queries);
    for (int i = 0; i < num_queries; i++)
    {
        int k;
        std::cout << "Enter number of conditions for Query " << i + 1 << ": ";
        std::cin >> k;

        std::cout << "Enter " << k << " condition values:\n";
        queries[i].id = i;
        queries[i].conditions.resize(k);
        for (int j = 0; j < k; j++)
        {
            std::cin >> queries[i].conditions[j];
        }
    }

    // Create mutex
    mutex = CreateMutex(NULL, FALSE, NULL);

    // Create threads
    std::vector<HANDLE> threads(num_queries);
    std::vector<ThreadData> thread_data(num_queries);

    for (int i = 0; i < num_queries; i++)
    {
        thread_data[i].query = &queries[i];
        threads[i] = CreateThread(
            NULL, 0, thread_function, &thread_data[i], 0, NULL);
    }

    // Wait for all threads
    WaitForMultipleObjects(num_queries, threads.data(), TRUE, INFINITE);

    // Clean up
    for (auto &th : threads)
    {
        CloseHandle(th);
    }
    CloseHandle(mutex);

    std::cout << "\n\nFinal total matches found: " << total_matches << std::endl;

    return 0;
}

DWORD WINAPI thread_function(LPVOID lpParam)
{
    ThreadData *data_ptr = (ThreadData *)lpParam;
    Query *query = data_ptr->query;

    int local_count = 0;
    for (int record : data)
    {
        for (int cond : query->conditions)
        {
            if (record % cond == 0)
            {
                local_count++;
                break;
            }
        }
    }

    // Safely update global count
    WaitForSingleObject(mutex, INFINITE);
    total_matches += local_count;
    std::cout << "Thread for Query " << query->id + 1 << " found " << local_count << " matches. Total so far: " << total_matches << std::endl;
    ReleaseMutex(mutex);

    return 0;
}

// _________________________________________________________________________________
// Run Commands (Ubuntu Terminal):
// g++ -fopenmp  9_Parallelization_of_Database_Query_optimization.cpp -o threaded_query_app
//  ./threaded_query_app

// Run Commands (VS Code):
// g++ -o threaded_query_app 9_Parallelization_of_Database_Query_optimization.cpp -static -lws2_32
// .\threaded_query_app
// _________________________________________________________________________________

// Output Example:
// Enter total number of data elements: 10
// Enter data elements:
// 1 2 3 4 5 6 7 8 9 10

// Enter number of queries: 3
// Enter number of conditions for Query 1: 1
// Enter 1 condition values:
// 2
// Enter number of conditions for Query 2: 2
// Enter 2 condition values:
// 3 5
// Enter number of conditions for Query 3: 1
// Enter 1 condition values:
// 7
// Thread for Query 1 found 5 matches. Total so far: 5
// Thread for Query 2 found 5 matches. Total so far: 10
// Thread for Query 3 found 1 matches. Total so far: 11

// Final total matches found: 11
// _________________________________________________________________________________