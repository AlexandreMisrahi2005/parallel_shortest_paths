#include <queue>
#include <limits>
#include <thread>
#include <mutex>

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

void findRequests(std::vector<Pii> &requests, std::priority_queue<int> bucket, bool light_edge, std::vector<std::vector<Pii>> adj_list, std::vector<double> tent)
{
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
}

void relaxRequests(std::vector<Pii> requests, std::vector<double> &tent, std::vector<std::priority_queue<int>> &B)
{
    for (int i = 0; i < requests.size(); ++i)
    {
        relax(requests[i].first, requests[i].second, tent, B);
    }
}

void relaxRequestsPar(std::vector<Pii> requests, std::mutex &tentMutex, std::vector<double> &tent, std::vector<std::priority_queue<int>> &B)
{
    tentMutex.lock();
    for (int i = 0; i < requests.size(); ++i)
    {
        relax(requests[i].first, requests[i].second, tent, B);
    }
    tentMutex.unlock();
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
            std::vector<Pii> Req_light;
            findRequests(Req_light, B[i], true, adj_list, tent);
            R = B[i];
            B[i] = std::priority_queue<int>();
            relaxRequests(Req_light, tent, B);
        }
        std::vector<Pii> Req_heavy;
        findRequests(Req_heavy, R, false, adj_list, tent);
        relaxRequests(Req_heavy, tent, B);
    }
    return tent;
}

std::vector<double> parDeltaStepping(const Graph &graph, int source, int _DELTA, int _b, int NUM_THREADS)
{
    N = graph.size();
    DELTA = _DELTA;
    b = _b;

    int THREAD_NUM = NUM_THREADS;

    std::vector<std::priority_queue<int>> B(b); // b buckets (vectors) stored in B, priority queues
    std::vector<double> tent(N, INF);                   // tentative distances
    std::vector<std::vector<Pii>> adj_list = graph.get_adj_list();

    B[0].push(source);
    tent[source] = 0;

    // define heavy and light requests
    std::vector<Pii> R_h;
    std::vector<Pii> R_l;

    std::mutex tentMutex; // Mutex for protecting access to the "tent" vector

    int k = 0;
    while (k < b)
    {
        while (!B[k].empty())
        {
            std::vector<std::thread> workersL;
            std::vector<std::thread> workersH;

            findRequests(R_l, B[k], true, adj_list, tent);
            findRequests(R_h, B[k], false, adj_list, tent);

            B[k] = std::priority_queue<int>();


            int threads = THREAD_NUM;
            if (threads > R_l.size())
            {
                threads = R_l.size();
            }
            if (threads == 0)
            {
                threads = 1;
            }

            int blockstart = 0;
            int blocksize = R_l.size() / threads;

            for (int i = 0; i < threads - 1; i++)
            {
                int blockend = blockstart;
                blockend = blockend + blocksize;
                std::vector<Pii> R;
                for (int j = blockstart; j < blockend; j++)
                {
                    R.push_back(R_l[j]);
                }
                workersL.push_back(std::thread(&relaxRequestsPar, R, std::ref(tentMutex), std::ref(tent), std::ref(B)));
            }

            std::vector<Pii> R;
            for (int i = blockstart; i < R_l.size(); i++)
            {
                R.push_back(R_l[i]);
            }
            relaxRequestsPar(R, tentMutex, tent, B);

            for (int i = 0; i < threads - 1; i++)
            {
                try
                {
                    if (workersL[i].joinable())
                    {
                        workersL[i].join();
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "LTHREAD: Exception caught while joining thread: " << e.what() << '\n';
                }
            };

            R_l.clear();

            relaxRequestsPar(R_h, tentMutex, tent, B);
            threads = THREAD_NUM;
            if (threads > R_h.size())
            {
                threads = R_h.size();
            }
            if (threads == 0)
            {
                threads = 1;
            }
            blockstart = 0;
            blocksize = R_h.size() / threads;
            for (int i = 0; i < threads - 1; i++)
            {
                int blockend = blockstart;
                blockend = blockend + blocksize;
                std::vector<Pii> Rh;
                for (int j = blockstart; j < blockend; j++)
                {
                    Rh.push_back(R_h[j]);
                }
                workersH.push_back(std::thread(&relaxRequestsPar, Rh, std::ref(tentMutex), std::ref(tent), std::ref(B)));
            }
            std::vector<Pii> Rh;
            for (int i = blockstart; i < R_h.size(); i++)
            {
                Rh.push_back(R_h[i]);
            }
            relaxRequestsPar(Rh, tentMutex, tent, B);

            for (int i = 0; i < threads - 1; i++)
            {
                try
                {
                    if (workersH[i].joinable())
                    {
                        workersH[i].join();
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << "HTHREAD: Exception caught while joining thread: " << e.what() << '\n';
                }
            };

            R_h.clear();
        };

        int i = 0;
        while (B[i].empty() && i < b)
        {
            i++;
        }
        k = i;
    }
    return tent;
};