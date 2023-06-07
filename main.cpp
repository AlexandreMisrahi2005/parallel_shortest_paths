#include <chrono>
#include <cmath>
#include <fstream>

#include "deltastepping.cpp"
#include "dijkstra.cpp"

Pii smallestAndLongestEdges(std::vector<std::vector<Pii>> adjMat, int N)
{
    int min = INF;
    int max = -INF;
    for (size_t i = 0; i < N; ++i)
    {
        for (auto edge : adjMat[i])
        {
            double weight = edge.second;
            if (weight > max)
            {
                max = weight;
            }
            if (weight < min && weight > 0)
            {
                min = weight;
            }
        }
    }
    return std::make_pair(min, max);
}

// Graph* genRandomGraph(int n, double m){

// }

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <algorithm to use: [dijkstra, deltastepping]> "
                  << " <optional: delta for deltastepping algorithm: [integer]> " << std::endl;
        return 1;
    }

    /*
    Test graph from Abdul Bari (youtube)
    with source node 0
    result should be:
    0 2 3 8 6 9
    */

    Graph g(6);

    g.add_edge(0, 1, 2);
    g.add_edge(0, 2, 4);
    g.add_edge(1, 2, 1);
    g.add_edge(1, 3, 7);
    g.add_edge(2, 4, 3);
    g.add_edge(3, 5, 1);
    g.add_edge(4, 5, 5);
    g.add_edge(4, 3, 2);

    /*
    Test Graph
    with source node 0
    result should be:
    0 6 4 3 11 10 5 7 14 18 11 10 8 10 18
    */

    // Graph g(15);
    // g.add_edge(0, 1, 6);
    // g.add_edge(0, 2, 4);
    // g.add_edge(0, 3, 3);
    // g.add_edge(1, 0, 6);
    // g.add_edge(1, 4, 5);
    // g.add_edge(1, 5, 9);
    // g.add_edge(2, 0, 4);
    // g.add_edge(2, 5, 6);
    // g.add_edge(2, 6, 8);
    // g.add_edge(3, 0, 3);
    // g.add_edge(3, 6, 2);
    // g.add_edge(3, 7, 4);
    // g.add_edge(4, 1, 5);
    // g.add_edge(4, 8, 3);
    // g.add_edge(4, 9, 7);
    // g.add_edge(5, 1, 9);
    // g.add_edge(5, 2, 6);
    // g.add_edge(5, 10, 5);
    // g.add_edge(6, 2, 8);
    // g.add_edge(6, 3, 2);
    // g.add_edge(6, 11, 6);
    // g.add_edge(7, 3, 4);
    // g.add_edge(7, 11, 3);
    // g.add_edge(7, 12, 1);
    // g.add_edge(8, 4, 3);
    // g.add_edge(8, 9, 6);
    // g.add_edge(8, 13, 9);
    // g.add_edge(9, 4, 7);
    // g.add_edge(9, 8, 6);
    // g.add_edge(9, 14, 2);
    // g.add_edge(10, 5, 5);
    // g.add_edge(10, 11, 4);
    // g.add_edge(10, 12, 3);
    // g.add_edge(11, 6, 6);
    // g.add_edge(11, 7, 3);
    // g.add_edge(11, 10, 4);
    // g.add_edge(12, 7, 1);
    // g.add_edge(12, 10, 3);
    // g.add_edge(12, 13, 2);
    // g.add_edge(13, 8, 9);
    // g.add_edge(13, 12, 2);
    // g.add_edge(13, 14, 8);
    // g.add_edge(14, 9, 2);
    // g.add_edge(14, 13, 8);

    std::vector<double> dist;

    // Find the shortest paths from vertex 0
    std::string algo = argv[1];
    if (algo == "deltastepping")
    {
        std::vector<std::vector<Pii>> adjMat = g.get_adj_list();
        Pii p = smallestAndLongestEdges(adjMat, g.size());
        double DELTA = p.first;
        if (argc == 3)
        {
            DELTA = std::stod(argv[2]);
        }
        int b = 1 + std::ceil(p.second / DELTA);
        b = 50;

        // Start the timer
        auto start = std::chrono::high_resolution_clock::now();

        dist = seqDeltaStepping(g, 0, DELTA, b); // graph, source node, delta, b (number of buckets)

        // End the timer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Operation took " << duration << " µs." << std::endl;
    }
    else if (algo == "dijkstra")
    {
        dist = dijkstra(g, 0);
    }

    // print the shortest paths
    std::cout << "Vertex\tDistance from Source" << std::endl;
    for (int i = 0; i < g.size(); ++i)
    {
        std::cout << i << "\t" << dist[i] << std::endl;
    }

    std::ofstream outFile("output_sssp.txt");
    for (const auto &e : dist) outFile << e << "\n";

    return 0;
}