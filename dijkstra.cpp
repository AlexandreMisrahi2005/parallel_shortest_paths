#include <queue>
#include <limits>

#include "graph.h"

// Dijkstra algorithm from Wikipedia Pseudo-code
std::vector<double> dijkstra(const Graph &graph, int source)
{
    int n = graph.size();

    if (source < 0 || source >= n)
        throw std::invalid_argument("Invalid source vertex");

    std::vector<double> dist(n, std::numeric_limits<double>::max());
    std::vector<std::vector<Pii> > adj_list = graph.get_adj_list();

    std::priority_queue<Pii> Q; // Priority queue of vertices

    Q.push(std::make_pair(0, source));
    dist[source] = 0;

    while (!Q.empty())
    {
        int u = Q.top().second;
        Q.pop();

        // iterate through all the neighbor vertices of u
        for (auto edge : adj_list[u])
        {
            int v = edge.first;
            double weight = edge.second;

            // the distance to v through u must be less than the current distance
            int alt = dist[u] + weight;
            if (alt < dist[v]) {
                dist[v] = alt;
                Q.push(std::make_pair(dist[v], v));
            }
        }
    }
    return dist;
}