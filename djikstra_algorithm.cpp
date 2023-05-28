#include <iostream>
#include <vector>
#include <chrono>
#include <queue>
#include <limits>

using namespace std;

typedef pair<int, int> pii; // (weight, vertex)

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

// Dijkstra algorithm from Wikipedia Pseudo-code
vector<int> dijkstra(const Graph& graph, int source) {
    int n = graph.size();

    if (source < 0 || source >= n)
        throw invalid_argument("Invalid source vertex");

    vector<int> dist(n, numeric_limits<int>::max());
    vector<bool> visited(n, false);
    vector< vector<pii> > adj_list = graph.get_adj_list();
    std::queue<pii> Q; // Queue of vertices

    Q.push(make_pair(0, source));
    dist[source] = 0;

    while (!Q.empty()) {
        int u = Q.front().second;
        Q.pop();

        if (visited[u])
            continue;

        visited[u] = true;

        // iterate through all the neighbor vertices of u
        for (auto edge : adj_list[u]) {
            int v = edge.first;
            int weight = edge.second;

            // v must not be visited and the distance to v through u must be less than the current distance
            int alt = dist[u] + weight;
            if (alt < dist[v]) { // !visited[v] &&
                dist[v] = alt;
                Q.push(make_pair(dist[v], v));
            }
        }
    }
    return dist;
}

int main() {
    Graph g(15); // create a graph with 15 vertices

    // add edges to the graph
    g.add_edge(0, 1, 6);
    g.add_edge(0, 2, 4);
    g.add_edge(0, 3, 3);
    g.add_edge(1, 0, 6);
    g.add_edge(1, 4, 5);
    g.add_edge(1, 5, 9);
    g.add_edge(2, 0, 4);
    g.add_edge(2, 5, 6);
    g.add_edge(2, 6, 8);
    g.add_edge(3, 0, 3);
    g.add_edge(3, 6, 2);
    g.add_edge(3, 7, 4);
    g.add_edge(4, 1, 5);
    g.add_edge(4, 8, 3);
    g.add_edge(4, 9, 7);
    g.add_edge(5, 1, 9);
    g.add_edge(5, 2, 6);
    g.add_edge(5, 10, 5);
    g.add_edge(6, 2, 8);
    g.add_edge(6, 3, 2);
    g.add_edge(6, 11, 6);
    g.add_edge(7, 3, 4);
    g.add_edge(7, 11, 3);
    g.add_edge(7, 12, 1);
    g.add_edge(8, 4, 3);
    g.add_edge(8, 9, 6);
    g.add_edge(8, 13, 9);
    g.add_edge(9, 4, 7);
    g.add_edge(9, 8, 6);
    g.add_edge(9, 14, 2);
    g.add_edge(10, 5, 5);
    g.add_edge(10, 11, 4);
    g.add_edge(10, 12, 3);
    g.add_edge(11, 6, 6);
    g.add_edge(11, 7, 3);
    g.add_edge(11, 10, 4);
    g.add_edge(12, 7, 1);
    g.add_edge(12, 10, 3);
    g.add_edge(12, 13, 2);
    g.add_edge(13, 8, 9);
    g.add_edge(13, 12, 2);
    g.add_edge(13, 14, 8);
    g.add_edge(14, 9, 2);
    g.add_edge(14, 13, 8);


    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();;

    // Find the shortest paths from vertex 0
    vector<int> dist = dijkstra(g, 0);

    // Print the shortest paths
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