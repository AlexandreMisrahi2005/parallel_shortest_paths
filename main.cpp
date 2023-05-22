#include <iostream>
#include "deltaStepping.cpp"

using namespace std;

void displayRes(int *res, int N){
	cout << "Result:" << endl;
	for(size_t i = 0; i < N; ++i){
		cout << "Node " << i << " with SP " << res[i] << " " << endl;
	}
}

/*
Test graph from Abdul Bari (youtube)
{{0,2,4,0,0,0},
 {0,0,1,7,0,0},
 {0,0,0,0,3,0},
 {0,0,0,0,0,1},
 {0,0,0,2,0,5},
 {0,0,0,0,0,0}};

 with source node 0
 result should be:
 0 2 3 8 6 9
*/
pair<int**, int> graph_1(){
	const int N = 6;
	int** adjMat = new int*[N];
	for(size_t i = 0; i < N; ++i){
		adjMat[i] = new int[N];
		for(size_t j = 0; j < N; ++j){
			adjMat[i][j] = 0;
		}
	}
	adjMat[0][1] = 2;
	adjMat[0][2] = 4;
	adjMat[1][2] = 1;
	adjMat[1][3] = 7;
	adjMat[2][4] = 3;
	adjMat[3][5] = 1;
	adjMat[4][3] = 2;
	adjMat[4][5] = 5;
	return make_pair(adjMat, N);
}

/*
Helper functions for parsing graphs
*/
pair<int, int> smallestAndLongestEdges(int** adjMat, int N){
	int min = INT_MAX;
	int max = INT_MIN;
	for(size_t i = 0; i < N; ++i){
		for(size_t j = 0; j < N; ++j){
			if (adjMat[i][j] > max) {
                max = adjMat[i][j];
            }
			if (adjMat[i][j] < min && adjMat[i][j] > 0) {
				min = adjMat[i][j];
			}
		}
	}
	return make_pair(min, max);
}

int main(int argc, char* argv[]){
	pair<int**,int> graph = graph_1();
	int** adjMat = graph.first;
	int N = graph.second;
	int res[N];
	pair<int, int> p = smallestAndLongestEdges(adjMat, N);
	int DELTA = p.first;
	if (argc == 2) {
        DELTA = stoi(argv[1]);
    }
	int b = 1 + ceil(p.second/DELTA);
	seqDeltaStepping(adjMat,   // adjacency matrix
					 0,        // source node
					 res,      // result array (size N)
					 N,        // number of nodes
					 b,        // b as in the original paper
					 DELTA     // bucket width as in the original paper
					);
	displayRes(res,N);
    return 0;
}
