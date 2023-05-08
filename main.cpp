#include <iostream>
#include "deltaStepping.cpp"

void displayRes(int *res, int N){
	std::cout << "Result:" << std::endl;
	for(size_t i = 0; i < N; ++i){
		std::cout << "index-" << i << " " << res[i] << " " << std::endl;
	}
}

/*
5est graph from Abdul Bari (youtube)
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
std::pair<int**,int> graph_1(){
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
	return std::make_pair(adjMat, N);
}

int main(int argc, char* argv[]){
	std::pair<int**,int> graph = graph_1();
	int N = graph.second;
	int res[N];
	seqDeltaStepping(graph.first, 0, res, N);
	displayRes(res,N);
    return 0;
}
