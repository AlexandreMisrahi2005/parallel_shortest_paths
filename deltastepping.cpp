#include <queue>
#include <limits>

#include "graph.h"

// constants
const int INF = std::numeric_limits<double>::max(); // virtual infinity
int DELTA;                                          // parameter of Delta-Stepping
int b;                                              // number of buckets
int N;                                              // number of vertices

bool isNotEmpty(std::vector<std::priority_queue<int>> bucket_vec)
{
    for (size_t i = 0; i < bucket_vec.size(); ++i)
    {
        if (!bucket_vec[i].empty())
        {
            return true;
        }
    }
    return false;
}

void relax(int w, int x, std::vector<double> &tent, std::vector<std::priority_queue<int>> &B)
{
    if (x < tent[w])
    {
        if (tent[w] != INF)
        {

            B[(tent[w] / DELTA)].pop(); // remove from old bucket
        }
        B[(x / DELTA)].push(w); // insert in new bucket
        tent[w] = x;
    }
}

std::vector<Pii> findRequests(std::priority_queue<int> bucket, bool light_edge, std::vector<std::vector<Pii>> adj_list, std::vector<double> tent)
{
    std::vector<Pii> requests;

    while (!bucket.empty())
    {
        int i = bucket.top();
        bucket.pop();

        for (auto edge : adj_list[i])
        {
            int j = edge.first;
            double weight = edge.second;

            if (weight > 0 && ((light_edge && weight <= DELTA) || (!light_edge && weight > DELTA)))
            {
                requests.push_back(std::make_pair(j, tent[i] + weight));
            }
        }
    }
    return requests;
}

void relaxRequests(std::vector<Pii> requests, std::vector<double> &tent, std::vector<std::priority_queue<int>> &B)
{
    for (int i = 0; i < requests.size(); ++i)
    {
        relax(requests[i].first, requests[i].second, tent, B);
    }
}

/*
 * adj_list: adjacency matrix of the graph
 * source: source, 0 ≤ s < N
 * tent: result array of length N, contains the shortest paths to source
 * N: number of nodes in adj_list, length of tent
 */
std::vector<double> seqDeltaStepping(const Graph &graph, int source, double _DELTA, int _b)
{
    N = graph.size();
    DELTA = _DELTA;
    b = _b;

    std::vector<std::priority_queue<int>> B(b); // b buckets (vectors) stored in B, priority queues
    std::vector<double> tent(N, INF);                   // tentative distances
    std::vector<std::vector<Pii>> adj_list = graph.get_adj_list();

    // for (int i = 0; i < N; ++i)
    // {
    //     tent[i] = INF;
    // }

    tent[source] = 0;
    B[0].push(source);
    int i = b;
    while (isNotEmpty(B))
    {
        for (size_t j = 0; j < b; ++j)
        {
            if (!(B[j].empty()))
            {
                i = j;
                break;
            }
        }
        std::priority_queue<int> R;
        while (!(B[i].empty()))
        {
            std::vector<Pii> Req_light = findRequests(B[i], true, adj_list, tent);
            R = B[i];
            B[i] = std::priority_queue<int>();
            relaxRequests(Req_light, tent, B);
        }
        std::vector<Pii> Req_heavy = findRequests(R, false, adj_list, tent);
        relaxRequests(Req_heavy, tent, B);
    }
    return tent;
}
