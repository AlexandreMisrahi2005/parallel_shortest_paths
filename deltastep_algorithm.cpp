#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <set>
#include <memory>
#include <cmath>

using namespace std;

typedef pair<int, int> pii; // (weight, vertex)

const int INF = numeric_limits<int>::max();

class Graph {
    int vertices; // number of vertices
    vector< vector<pii> > adj_list; // adjacency list

    public:
        Graph(int n) : vertices(n) {
            adj_list.resize(n);
        }

        // add an edge between vertices u and v with weight
        void add_edge(int u, int v, int weight) {
            adj_list[u].push_back(make_pair(v, weight));
            adj_list[v].push_back(make_pair(u, weight));
        }

        int size() const {
            return vertices;
        }
        vector< vector<pii> > get_adj_list() const {
            return adj_list;
        }
};

// void extend(vector<set<int> >* B, int n){
//     for(size_t i = 0; i < n; ++i){
//         set<int> v;
//         (B).push_back(v);
//     }
// }

bool isNotEmpty(vector<set<int> > bucket_vec){
    for(size_t i = 0; i < bucket_vec.size(); ++i){
        if(!bucket_vec[i].empty()){
            return true;
        }
    }
    return false;
}

void relax(int w, int x, vector<int> &tent, vector< set<int> > &B, int DELTA){
    if(x < tent[w]){
        if(tent[w] != INF){

            // if(ind >= (B).size()){
            //     extend(B, ind-((B).size())+1);
            // }

            B[tent[w]/DELTA].erase(w);   // remove from old bucket
        }
        B[x/DELTA].insert(w);      // insert in new bucket
        tent[w] = x;
    }
}

vector<pii> findRequests(set<int> bucket, bool light_edge, vector< vector<pii> > adj_list, vector<int> tent, int N, int DELTA){
    vector<pii> requests;

    for(auto i : bucket){
        for(auto edge : adj_list[i]){
            int j = edge.first;
            int weight = edge.second;

            if (weight > 0 && ((light_edge && weight <= DELTA)||(!light_edge && weight > DELTA))){
                requests.push_back(std::make_pair(j, tent[i]+weight));
            }
        }
    }
    return requests;
}

void relaxRequests(vector<pii> requests, vector<int> &tent, vector< set<int> > &B, int DELTA){
    for(int i = 0; i < requests.size(); ++i){
        relax(requests[i].first, requests[i].second, tent, B, DELTA);
    }
}

/*
 * adj_list: adjacency matrix of the graph
 * source: source, 0 ≤ s < N
 * tent: result array of length N, contains the shortest paths to source
 * N: number of nodes in adj_list, length of tent
*/
vector<int> seqDeltaStepping(const Graph& graph, int source, int delta, int b) {
    int N = graph.size();
    vector< set<int> > B(b); // b buckets (vectors) stored in B
    vector<int> tent(N); // tentative distances
    vector< vector<pii> > adj_list = graph.get_adj_list();

    for(int i = 0; i < N; ++i){
        tent[i] = INF;
    }

    tent[source] = 0;
    B[0].insert(source);
    int i = b;

    while(isNotEmpty(B)){

        for(size_t j = 0; j < b; ++j){
            if( !(B[j].empty()) ){
                i = j;
                break;
            }
        }

        set<int> R;
        while( !(B[i].empty()) ){

            vector<pii> Req_light = findRequests(B[i], true, adj_list, tent, N, delta);
            R = B[i];
            B[i].clear();

            relaxRequests(Req_light, tent, B, delta);

        }
        vector<pii> Req_heavy = findRequests(R, false, adj_list, tent, N, delta);
        relaxRequests(Req_heavy, tent, B, delta);
    }
    return tent;
}

int main() {
    // Test graph from Abdul Bari (youtube)
    // with source node 0
    // result should be:
    // 0 2 3 8 6 9
    Graph g(6);

    g.add_edge(0, 1, 2);
    g.add_edge(0, 2, 4);
    g.add_edge(1, 2, 1);
    g.add_edge(1, 3, 7);
    g.add_edge(2, 4, 3);
    g.add_edge(3, 5, 1);
    g.add_edge(4, 5, 5);
    g.add_edge(4, 3, 2);

    // Test Graph
    // with source node 0
    // result should be:
    // 0 6 4 3 11 10 5 7 14 18 11 10 8 10 18
    // Graph g(15); // create a graph with 15 vertices

    // // add edges to the graph
    // g.add_edge(0, 1, 6);
    // g.add_edge(0, 2, 4);
    // g.add_edge(0, 3, 3);
    // g.add_edge(1, 0, 6);
    // g.add_edge(1, 4, 5);
    // g.add_edge(1, 5, 9);
    // g.add_edge(2, 0, 4);
    // g.add_edge(2, 5, 6);
    // g.add_edge(2, 6, 8);
    // g.add_edge(3, 0, 3);
    // g.add_edge(3, 6, 2);
    // g.add_edge(3, 7, 4);
    // g.add_edge(4, 1, 5);
    // g.add_edge(4, 8, 3);
    // g.add_edge(4, 9, 7);
    // g.add_edge(5, 1, 9);
    // g.add_edge(5, 2, 6);
    // g.add_edge(5, 10, 5);
    // g.add_edge(6, 2, 8);
    // g.add_edge(6, 3, 2);
    // g.add_edge(6, 11, 6);
    // g.add_edge(7, 3, 4);
    // g.add_edge(7, 11, 3);
    // g.add_edge(7, 12, 1);
    // g.add_edge(8, 4, 3);
    // g.add_edge(8, 9, 6);
    // g.add_edge(8, 13, 9);
    // g.add_edge(9, 4, 7);
    // g.add_edge(9, 8, 6);
    // g.add_edge(9, 14, 2);
    // g.add_edge(10, 5, 5);
    // g.add_edge(10, 11, 4);
    // g.add_edge(10, 12, 3);
    // g.add_edge(11, 6, 6);
    // g.add_edge(11, 7, 3);
    // g.add_edge(11, 10, 4);
    // g.add_edge(12, 7, 1);
    // g.add_edge(12, 10, 3);
    // g.add_edge(12, 13, 2);
    // g.add_edge(13, 8, 9);
    // g.add_edge(13, 12, 2);
    // g.add_edge(13, 14, 8);
    // g.add_edge(14, 9, 2);
    // g.add_edge(14, 13, 8);


    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();;

    // Find the shortest paths from vertex 0
    vector<int> dist = seqDeltaStepping(g, 0, 1, 45);

    // print the shortest paths
    cout << "Vertex\tDistance from Source\n";
    for (int i = 0; i < g.size(); ++i) {
        cout << i << "\t" << dist[i] << "\n";
    }

    // End the timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Print the time taken
    std::cout << "Operation took " << duration << " microseconds." << std::endl;

    return 0;
}
