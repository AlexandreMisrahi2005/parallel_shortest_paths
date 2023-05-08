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
#include <list>

bool isNotEmpty(std::list<std::vector> l){
    for(size_t i = 0; i < l.size(); ++i){
        if(!l[i].empty()){
            return true;
        }
    }
    return false;
}

void relax(int w, int x, int* res, ??? B){
    if(res[w] > x){
        B[]
    }
}

/*
 * adjMat: adjacency matrix of the graph
 * s: source, 0 ≤ s < N
 * res: result array of length N, contains the shortest paths to s
 * N: number of nodes in adjMat, length of res
*/
void seqDeltaStepping(int** adjMat, int s, int* res, int N, int L, int DELTA){
    for(size_t i = 0; i < N; ++i){
        res[i] = INT_MAX;  // change to std::numeric_limits<int>::infinity() ??????????
    }
    res[s] = 0;
    const size_t b(std::ceil(L / DELTA) + 1); // b
    std::list< std::vector<int*> > B(b); // b buckets (vectors) stored in B
    while(isNotEmpty(B)){
        // do something
    }
}