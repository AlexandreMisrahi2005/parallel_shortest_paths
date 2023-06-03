#include <queue>
#include <limits>
#include <thread>

#include "graph.h"

// constants
const int INF = std::numeric_limits<int>::max(); // virtual infinity
int DELTA;                                       // parameter of Delta-Stepping
int b;                                           // number of buckets
int N;                                           // number of vertices

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

void relax(int w, int x, std::vector<int> &tent, std::vector<std::priority_queue<int>> &B)
{
    if (x < tent[w])
    {
        if (tent[w] != INF)
        {
            B[(tent[w] / DELTA) % b].pop(); // remove from old bucket
        }
        B[(x / DELTA) % b].push(w); // insert in new bucket
        tent[w] = x;
    }
}

std::vector<Pii> findRequests(std::priority_queue<int> bucket, bool light_edge, std::vector<std::vector<Pii>> adj_list, std::vector<int> tent)
{
    std::vector<Pii> requests;

    while (!bucket.empty())
    {
        int i = bucket.top();
        bucket.pop();

        for (auto edge : adj_list[i])
        {
            int j = edge.first;
            int weight = edge.second;

            if (weight > 0 && ((light_edge && weight <= DELTA) || (!light_edge && weight > DELTA)))
            {
                requests.push_back(std::make_pair(j, tent[i] + weight));
            }
        }
    }
    return requests;
}

void relaxRequests(std::vector<Pii> requests, std::vector<int> &tent, std::vector<std::priority_queue<int>> &B)
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
std::vector<int> seqDeltaStepping(const Graph &graph, int source, int _DELTA, int _b)
{
    N = graph.size();
    DELTA = _DELTA;
    b = _b;

    std::vector<std::priority_queue<int>> B(b); // b buckets (vectors) stored in B, priority queues
    std::vector<int> tent(N);                   // tentative distances
    std::vector<std::vector<Pii>> adj_list = graph.get_adj_list();

    for (int i = 0; i < N; ++i)
    {
        tent[i] = INF;
    }

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

std::vector<int> parDeltaStepping(const Graph &graph, int source, int _DELTA, int _b)
{
    N = graph.size();
    DELTA = _DELTA;
    b = _b;

    int MAX = 50; // Change later
    int THREAD_NUM = 15;
    std::vector<std::thread> workersL;
    std::vector<std::thread> workersH;

    std::vector<std::priority_queue<int>> B(b); // b buckets (vectors) stored in B, priority queues
    std::vector<int> tent(N);                   // tentative distances
    std::vector<std::vector<Pii>> adj_list = graph.get_adj_list();

    for (int i = 0; i < N; ++i)
    {
        tent[i] = INF;
    }

    tent[source] = 0;

    int k = 0;
    while (k < MAX)
    {
        // define heavy and light requests
        std::vector<Pii> R_h;
        std::vector<Pii> R_l;
        while (!B[k].empty())
        {

            R_l = findRequests(B[k], true, adj_list, tent);
            R_h = findRequests(B[k], false, adj_list, tent);

            relaxRequests(R_l, tent, B);

            std::vector<std::vector<Pii>> SubRL;
            int threads = THREAD_NUM;
            if (threads > R_l.size())
            {
                threads = R_l.size();
            }

            int blockstart = 0;
            int blocksize = R_l.size() / threads;

            for (int i = 0; i < threads - 1; i++)
            {
                int blockend = blockstart;
                blockend = blockend + blocksize;
                std::vector<Pii> R;
                for (int i = blockstart; i < blockend; i++)
                {
                    R.push_back(R_l[i]);
                }
                workersL.push_back(std::thread(&relaxRequests, std::ref(R), tent, B));
                SubRL.push_back(R);
            }

            std::vector<Pii> R;
            for (int i = blockstart; i < R_l.size(); i++)
            {
                R.push_back(R_l[i]);
            }
            relaxRequests(R, tent, B);
            SubRL.push_back(R);

            for (int i = 0; i < threads - 1; i++)
            {
                if (workersL[i].joinable())
                {
                    workersL[i].join();
                };
            };

            R_l.clear();
            for (int it = 0; it < SubRL.size(); it++)
            {
                if (SubRL[it].size() > 0)
                {
                    for (int i = 0; i < SubRL.size(); i++)
                    {
                        R_l.push_back(SubRL[it][i]);
                    }
                }
            };
        };

        relaxRequests(R_h, tent, B);
        std::vector<std::vector<Pii>> SubRH;
        int threads = THREAD_NUM;
        if (threads > R_h.size())
        {
            threads = R_h.size();
        }
        int blockstart = 0;
        int blocksize = R_h.size() / threads;
        for (int i = 0; i < threads - 1; i++)
        {
            int blockend = blockstart;
            blockend = blockend + blocksize;
            std::vector<Pii> Rh;
            for (int i = blockstart; i < blockend; i++)
            {
                Rh.push_back(R_h[i]);
            }
            workersH.push_back(std::thread(&relaxRequests, std::ref(Rh), tent, B));
            SubRH.push_back(Rh);
        }
        std::vector<Pii> Rh;
        for (int i = blockstart; i < R_h.size(); i++)
        {
            Rh.push_back(R_h[i]);
        }
        relaxRequests(Rh, tent, B);
        SubRH.push_back(Rh);

        for (int i = 0; i < threads - 1; i++)
        {
            if (workersH[i].joinable())
            {
                workersH[i].join();
            };
        };

        R_h.clear();
        for (int it = 0; it < SubRH.size(); it++)
        {
            if (SubRH[it].size() > 0)
            {
                for (int i = 0; i < SubRH.size(); i++)
                {
                    R_h.push_back(SubRH[it][i]);
                }
            }
        };

        int i = 0;
        while (B[i].empty())
        {
            i++;
        }
        k = i;
    }
    return tent;
};