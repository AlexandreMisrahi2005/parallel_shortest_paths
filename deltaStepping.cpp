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

/*
 * adjMat: adjacency matrix of the graph
 * s: source, 0 ≤ s < N
 * res: result array of length N, contains the shortest paths to s
 * N: number of nodes in adjMat, length of res
*/
void seqDeltaStepping(int** adjMat, int s, int* res, int N){
    for(size_t i = 0; i < N; ++i){
        res[i] = std::numeric_limits<int>::max();  // change to inf???
    }
}