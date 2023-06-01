#include <queue>
#include <limits>

#include "graph.h"

using namespace std;

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