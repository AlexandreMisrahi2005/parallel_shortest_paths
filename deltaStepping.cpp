#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <cmath>
#include <mutex>
#include <numeric>
#include <thread>
#include <queue>

using namespace std;

// global parameters
int DELTA;     // parameter DELTA for delta-stepping algo
int N;         // nb of nodes in graph
int b;         // nb of buckets in B

void extend(vector<vector<int> >* B, int n){
    for(size_t i = 0; i < n; ++i){
        vector<int> v;
        (*B).push_back(v);
    }
}

bool isNotEmpty(vector<vector<int> > bucket_vec){
    for(size_t i = 0; i < bucket_vec.size(); ++i){
        if(!bucket_vec[i].empty()){
            return true;
        }
    }
    return false;
}

void relax(int w, int x, int* tent, vector< vector<int> >* B){
    if(x < tent[w]){
        int ind;
        std::cout << "checkpoint 1" << std::endl;
        if(tent[w] < INT_MAX){
            int ind = int(floor(tent[w]/DELTA));
            // std::cout << "index " << ind << std::endl;
            // if(ind >= (*B).size()){
            //     std::cout << "extending" << std::endl;
            //     extend(B, ind-((*B).size())+1);
            // }
            (*B)[ind].erase(std::remove((*B)[ind].begin(), (*B)[ind].end(), w), (*B)[ind].end());   // remove from old bucket
        }
        std::cout << "checkpoint 2 " << (*B).size() << " " << x << std::endl;
        std::cout << "floor(x/DELTA) " << floor(x/DELTA) << std::endl;
        (*B)[floor(x/DELTA)].push_back(w);      // insert in new bucket
        std::cout << "checkpoint 3" << std::endl;
        tent[w] = x;
    }
}

vector<pair<int, int> > findRequests(vector<int> bucket, bool light_edge, int** adjMat, int* tent){
    vector<pair<int, int> > requests;
    std::cout << "here" << std::endl;
    for(size_t i = 0; i < bucket.size(); ++i){
        for(int j = 0; j < N; ++j){
            int edge = adjMat[bucket[i]][j];
            if(edge > 0 && ((light_edge && edge <= DELTA)||(!light_edge && edge > DELTA))){
                std::cout << "tentative dist and extra edge dist: " << tent[bucket[i]] << " " << edge << std::endl;
                requests.push_back(std::make_pair(j, tent[bucket[i]]+edge));
            }
        }
    }
    return requests;
}

void relaxRequests(vector<pair<int,int> > requests, int* tent, vector< vector<int> >* B){
    for(size_t i = 0; i < requests.size(); ++i){
        relax(requests[i].first, requests[i].second, tent, B);
    }
}

/*
 * adjMat: adjacency matrix of the graph
 * s: source, 0 ≤ s < N
 * res: result array of length N, contains the shortest paths to s
 * N: number of nodes in adjMat, length of res
*/
void seqDeltaStepping(int** adjMat, int s, int* tent, int N_, int b, int DELTA_){
    DELTA = DELTA_;
    N = N_;
    std::cout << "nb of buckets: " << b << std::endl;
    vector<vector<int> > *B = new vector<vector<int> >(b); // b buckets (vectors) stored in B
    // vector<vector<int> > *B = new vector<vector<int> >(1); // 1 initial bucket stored in B
    for(size_t i = 0; i < N; ++i){
        tent[i] = INT_MAX;
    }
    tent[s] = 0;
    (*B)[0].push_back(s);
    int i = b;
    while(isNotEmpty(*B)){
        std::cout << "outer while" << std::endl;
        for(size_t j = 0; j < b; ++j){
            if(!(*B)[j].empty()){
                i = j;
                break;
            }
        }
        // std::cout << "first non-empty bucket: " << i << std::endl;
        vector<int> R;
        while(!(*B)[i].empty()){
            std::cout << "inner while" << std::endl;
            vector<pair<int, int> > Req_light = findRequests((*B)[i], true, adjMat, tent);
            R = (*B)[i];
            (*B)[i].clear();
            // std::cout << R.size() << std::endl;
            relaxRequests(Req_light, tent, B);
            // std::cout << (*B)[i].size() << std::endl;
        }
        std::cout << "heavdy requests" << std::endl;
        vector<pair<int, int> > Req_heavy = findRequests(R, false, adjMat, tent);
        relaxRequests(Req_heavy, tent, B);
    }
}


/*

TO-DO

- change the buckets (each (*B)[i]) to a priority queue. Nodes should come in increasing
    order based on their current tentative distance??? (not sure, check in paper)

- change INT_MAX to std::numeric_limits<int>::infinity()  ???
    INT_MAX is not inf so the graph is limited. 
    -> Alternative: find the maximum distance in the graph and standardize all graph distances
    if we change the graph weight type to double, then we can use infinity
    --> make a template for graph edge type???

*/