#include <queue>
#include <limits>

#include "graph.h"

using namespace std;

// constants
const int INF = numeric_limits<int>::max(); // virtual infinity
int DELTA;                                  // parameter of Delta-Stepping
int b;                                      // number of buckets
int N;                                      // number of vertices

bool isNotEmpty(vector<std::priority_queue<int> > bucket_vec){
    for(size_t i = 0; i < bucket_vec.size(); ++i){
        if(!bucket_vec[i].empty()){
            return true;
        }
    }
    return false;
}

void relax(int w, int x, vector<int> &tent, vector< std::priority_queue<int> > &B){
    if(x < tent[w]){
        if(tent[w] != INF){

            B[(tent[w]/DELTA)%b].pop();   // remove from old bucket
        }
        B[(x/DELTA)%b].push(w);      // insert in new bucket
        tent[w] = x;
    }
}

vector<pii> findRequests(std::priority_queue<int> bucket, bool light_edge, vector< vector<pii> > adj_list, vector<int> tent){
    vector<pii> requests;

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

void relaxRequests(vector<pii> requests, vector<int> &tent, vector< std::priority_queue<int> > &B){
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
vector<int> seqDeltaStepping(const Graph& graph, int source, int _DELTA, int _b) {
    N = graph.size();
    DELTA = _DELTA;
    b = _b;
    vector< std::priority_queue<int> > B(b); // b buckets (vectors) stored in B, priority queues
    vector<int> tent(N); // tentative distances
    vector< vector<pii> > adj_list = graph.get_adj_list();

    for(int i = 0; i < N; ++i){
        tent[i] = INF;
    }

    tent[source] = 0;
    B[0].push(source);
    int i = b;

    while(isNotEmpty(B)){

        // problem here due to modulo b?
        for(size_t j = 0; j < b; ++j){
            if( !(B[j].empty()) ){
                i = j;
                break;
            }
        }

        std::priority_queue<int> R;
        while( !(B[i].empty()) ){

            vector<pii> Req_light = findRequests(B[i], true, adj_list, tent);
            R = B[i];
            B[i] = std::priority_queue<int>();

            relaxRequests(Req_light, tent, B);

        }
        vector<pii> Req_heavy = findRequests(R, false, adj_list, tent);
        relaxRequests(Req_heavy, tent, B);
    }
    return tent;
}
