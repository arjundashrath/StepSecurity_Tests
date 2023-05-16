//Load header files
#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

//Helper function for BFS
void bfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    //Initialize a queue and push the source vertex
    queue<int> q;
    q.push(start);
    visited[start] = true;
    //Iterate until queue is empty
    while (!q.empty()) {
        int vertex = q.front();
        q.pop();

        // Parallelize the loop to process neighbors
        #pragma omp parallel for
        //Iterate through vertex edges
        for (int i = 0; i < graph[vertex].size(); i++) {
            int neighbor = graph[vertex][i];
            //Proceed if neighbors are univisited
            if (!visited[neighbor]) {
                // Ensure thread safety when modifying visited array and queue
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        //If neighbor is not visitied, push the neighbor into the queue
                        q.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }
        }
    }
}

//Helper function for Non parallel BFS function
void standardbfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    //Initialize a queue and push the source vertex
    queue<int> q;
    q.push(start);
    visited[start] = true;

    //Iterate until queue is empty
    while (!q.empty()) {
        int vertex = q.front();
        q.pop();
        //Iterate through vertex edges
        for (int i = 0; i < graph[vertex].size(); i++) {
            int neighbor = graph[vertex][i];
            if (!visited[neighbor]) {
                if (!visited[neighbor]) {
                    //If neighbor is not visitied, push the neighbor into the queue
                    q.push(neighbor);
                    visited[neighbor] = true;
                }

            }
        }
    }
}

//Function for parallel breadth first search
void parallel_bfs(vector<vector<int>>& graph, int start) {
    //Initialize visited array
    vector<bool> visited(graph.size(), false);

    // Create parallel region
    #pragma omp parallel
    {
        // Execute single thread to invoke bfs function
        #pragma omp single
        {
            //Call helper function
            bfs(graph, start, visited);
        }
    }
}

void standard_bfs(vector<vector<int>>& graph, int start) {
    //Initialize visited array
    vector<bool> visited(graph.size(), false);
    //Call helper function
    standardbfs(graph, start, visited);
}

//Parallel DFS helper function
void dfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    //Initialize a stack and push the source vertex
    stack<int> s;
    s.push(start);
    visited[start] = true;

    //Iterate stack until empty
    while (!s.empty()) {
        int vertex = s.top();
        s.pop();

        // Parallelize the loop to process neighbors
        #pragma omp parallel for
        //Iterate through vertex neighbors
        for (int i = 0; i < graph[vertex].size(); i++) {
            int neighbor = graph[vertex][i];
            if (!visited[neighbor]) {
                // Ensure thread safety when modifying visited array and stack
                #pragma omp critical
                {
                    //If neighbor is not visited, push into stack 
                    if (!visited[neighbor]) {
                        s.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }
        }
    }
}

//Parallel DFS helper function
void standard_dfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    //Initialize a stack and push the source vertex
    stack<int> s;
    s.push(start);
    visited[start] = true;

    //Iterate stack until empty
    while (!s.empty()) {
        int vertex = s.top();
        s.pop();
        
        for (int i = 0; i < graph[vertex].size(); i++) {
            int neighbor = graph[vertex][i];
            //If neighbor is not visited, push into stack 
            if (!visited[neighbor]) {
                s.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

//Function for parallel depth first search
void parallel_dfs(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);

    // Create parallel region
    #pragma omp parallel
    {
        // Execute single thread to invoke dfs function
        #pragma omp single
        {
            //Call helper function
            dfs(graph, start, visited);
        }
    }
}

//Function for standard depth first search
void standard_dfs(vector<vector<int>>& graph, int start) {
    //Initialize visited array
    vector<bool> visited(graph.size(), false);
    //Call helper function
    standard_dfs(graph, start, visited);
}

int main() {
    int vertex = 1000;
    vector<vector<int>> graph;
    
    //Iterate for each vertex
    for(int i = 0; i < vertex; i++){
        vector<int>edge_list;
        //For each vertex, generate random number of edges
        int edgeCount = rand()%vertex;
        //Generate random edges
        for(int j = 0; j < edgeCount; j++){
            int randomEdge = rand()%vertex;
            if(randomEdge != i){
                edge_list.push_back(randomEdge);
            }
        }
        graph.push_back(edge_list);
    }
    //Print graph data
    cout<<"First 5 Vertices: \n";
    for(int i = 0; i < 5; i++){
        cout<<"Vertex "<<i<<" First 5 Edges:{";
        for(int j = 0; j < 5; j++){
            cout<<graph[i][j]<<",";
        }
        cout<<"}\n";
    }

    cout<<"Running BFS:\nStandard BFS Time: ";

    //Compare time
    auto start = high_resolution_clock::now();
    standard_bfs(graph, 0);
    auto end = high_resolution_clock::now();
    auto diff = duration_cast<microseconds>(end-start);
    cout<<diff.count()<<" ms\nParallel BFS Time: ";

    //Compare time
    start = high_resolution_clock::now();
    parallel_bfs(graph, 0);
    end = high_resolution_clock::now();
    diff = duration_cast<microseconds>(end-start);
    cout<<diff.count()<<" ms\n";

    cout<<"Running DFS:\nStandard DFS Time: ";

    //Compare time
    start = high_resolution_clock::now();
    standard_dfs(graph, 0);
    end = high_resolution_clock::now();
    diff = duration_cast<microseconds>(end-start);
    cout<<diff.count()<<" ms\nParallel DFS Time: ";

    //Compare time
    start = high_resolution_clock::now();
    parallel_dfs(graph, 0);
    end = high_resolution_clock::now();
    diff = duration_cast<microseconds>(end-start);
    cout<<diff.count()<<" ms\n";

    return 0;
}
