#include <queue>
#include <limits>

#include "graph.h"

// constants
const int INF = numeric_limits<int>::max(); // virtual infinity
int DELTA;                                  // parameter of Delta-Stepping
int b;                                      // number of buckets
int N;                                      // number of vertices

bool isNotEmpty(std::vector<std::priority_queue<int> > B){
    std::cout << "here 8: length of B: " << std::endl;
    std::cout << B.size() << std::endl;
    for(size_t i = 0; i < B.size(); ++i){
        if(!B[i].empty()){
            return true;
        }
    }
    return false;
}

void relax(int w, int x, std::vector<int> &tent, std::vector< std::priority_queue<int> > &B){
    if(x < tent[w]){
        std::cout << "here 20: " << std::endl;
        std::cout << (tent[w]/DELTA)%b << std::endl;
        std::cout << (x/DELTA)%b << std::endl;
        if(tent[w] != INF){

            B[(tent[w]/DELTA)%b].pop();   // remove from old bucket
        }
        B[(x/DELTA)%b].push(w);      // insert in new bucket
        tent[w] = x;
    }
}

std::vector<pii> findRequests(std::priority_queue<int> bucket, bool light_edge, std::vector< std::vector<pii> > adj_list, vector<int> tent){
    std::vector<pii> requests;

    while (!bucket.empty()) {
        int i = bucket.top();
        bucket.pop();

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

void relaxRequests(std::vector<pii> requests, std::vector<int> &tent, std::vector< std::priority_queue<int> > &B){
    for(int i = 0; i < requests.size(); ++i){
        relax(requests[i].first, requests[i].second, tent, B);
    }
}

/*
 * adj_list: adjacency matrix of the graph
 * source: source, 0 ≤ s < N
 * tent: result array of length N, contains the shortest paths to source
 * N: number of nodes in adj_list, length of tent
*/
std::vector<int> seqDeltaStepping(const Graph& graph, int source, int _DELTA, int _b) {
    N = graph.size();
    DELTA = _DELTA;
    b = _b;
    std::cout << "b: " << b << std::endl;
    std::vector< std::priority_queue<int> > B(b); // b buckets (vectors) stored in B, priority queues
    std::vector<int> tent(N); // tentative distances
    std::vector< std::vector<pii> > adj_list = graph.get_adj_list();

    for(int i = 0; i < N; ++i){
        tent[i] = INF;
    }

    tent[source] = 0;
    B[0].push(source);
    int i = b;
    std::cout << "here 0" << std::endl;
    while(isNotEmpty(B)){
        std::cout << "here 1: length of bucket vector: " << std::endl;
        std::cout << B.size() << std::endl;
        // problem here due to modulo b?
        for(size_t j = 0; j < b; ++j){
            if( !(B[j].empty()) ){
                i = j;
                break;
            }
        }
        std::cout << "here 2" << std::endl;
        std::priority_queue<int> R;
        while( !(B[i].empty()) ){
            std::cout << "here 3" << std::endl;
            std::vector<pii> Req_light = findRequests(B[i], true, adj_list, tent);
            R = B[i];
            B[i] = std::priority_queue<int>();
            std::cout << "here 4" << std::endl;
            relaxRequests(Req_light, tent, B);
            std::cout << "here 5" << std::endl;
        }
        std::vector<pii> Req_heavy = findRequests(R, false, adj_list, tent);
        std::cout << "here 6" << std::endl;
        relaxRequests(Req_heavy, tent, B);
        std::cout << "here 7" << std::endl;
    }
    std::cout << "finish" << std::endl;
    return tent;
}
