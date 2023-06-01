#include <chrono>
#include <cmath>

#include "deltastep_algorithm.cpp"
#include "djikstra_algorithm.cpp"

pii smallestAndLongestEdges(vector< vector<pii> > adjMat, int N){
	int min = INF;
	int max = -INF;
	for (size_t i = 0; i < N; ++i) {
		for(auto edge : adjMat[i]) {
            int weight = edge.second;
			if (weight > max) {
                max = weight;
            }
			if (weight < min && weight > 0) {
				min = weight;
			}
		}
	}
	return make_pair(min, max);
}

int main(int argc, char* argv[]) {
    // USING DELTA STEP OR DIJKSTRA?
    bool delta_step = true;

    // Test graph from Abdul Bari (youtube)
    // with source node 0
    // result should be:
    // 0 2 3 8 6 9
    // Graph g(6);

    // g.add_edge(0, 1, 2);
    // g.add_edge(0, 2, 4);
    // g.add_edge(1, 2, 1);
    // g.add_edge(1, 3, 7);
    // g.add_edge(2, 4, 3);
    // g.add_edge(3, 5, 1);
    // g.add_edge(4, 5, 5);
    // g.add_edge(4, 3, 2);

    // Test Graph
    // with source node 0
    // result should be:
    // 0 6 4 3 11 10 5 7 14 18 11 10 8 10 18
    Graph g(15); // create a graph with 15 vertices

    // // add edges to the graph
    g.add_edge(0, 1, 6);
    g.add_edge(0, 2, 4);
    g.add_edge(0, 3, 3);
    g.add_edge(1, 0, 6);
    g.add_edge(1, 4, 5);
    g.add_edge(1, 5, 9);
    g.add_edge(2, 0, 4);
    g.add_edge(2, 5, 6);
    g.add_edge(2, 6, 8);
    g.add_edge(3, 0, 3);
    g.add_edge(3, 6, 2);
    g.add_edge(3, 7, 4);
    g.add_edge(4, 1, 5);
    g.add_edge(4, 8, 3);
    g.add_edge(4, 9, 7);
    g.add_edge(5, 1, 9);
    g.add_edge(5, 2, 6);
    g.add_edge(5, 10, 5);
    g.add_edge(6, 2, 8);
    g.add_edge(6, 3, 2);
    g.add_edge(6, 11, 6);
    g.add_edge(7, 3, 4);
    g.add_edge(7, 11, 3);
    g.add_edge(7, 12, 1);
    g.add_edge(8, 4, 3);
    g.add_edge(8, 9, 6);
    g.add_edge(8, 13, 9);
    g.add_edge(9, 4, 7);
    g.add_edge(9, 8, 6);
    g.add_edge(9, 14, 2);
    g.add_edge(10, 5, 5);
    g.add_edge(10, 11, 4);
    g.add_edge(10, 12, 3);
    g.add_edge(11, 6, 6);
    g.add_edge(11, 7, 3);
    g.add_edge(11, 10, 4);
    g.add_edge(12, 7, 1);
    g.add_edge(12, 10, 3);
    g.add_edge(12, 13, 2);
    g.add_edge(13, 8, 9);
    g.add_edge(13, 12, 2);
    g.add_edge(13, 14, 8);
    g.add_edge(14, 9, 2);
    g.add_edge(14, 13, 8);


    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    vector<int> dist;

    // Find the shortest paths from vertex 0
    if (delta_step) {
        std::cout << "here1" << std::endl; 
        vector< vector<pii> > adjMat = g.get_adj_list();

        pii p = smallestAndLongestEdges(adjMat, g.size());
        std::cout << "here2" << std::endl; 
        int DELTA = p.first;
        if (argc == 2) {
            DELTA = stoi(argv[1]);
        }
        std::cout << "here3" << std::endl; 
        int b = 1 + std::ceil(p.second/DELTA);
        std::cout << "b=" << b << "DELTA=" << DELTA << std::endl; 
        dist = seqDeltaStepping(g, 0, DELTA, b);
        std::cout << "here5" << std::endl;
    } else {
        dist = dijkstra(g, 0);
    }

    // print the shortest paths
    cout << "Vertex\tDistance from Source\n";
    for (int i = 0; i < g.size(); ++i) {
        cout << i << "\t" << dist[i] << "\n";
    }

    // End the timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    // Print the time taken
    std::cout << "Operation took " << duration << " microseconds." << std::endl;

    return 0;
}