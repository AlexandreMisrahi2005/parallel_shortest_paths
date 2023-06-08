#include <chrono>
#include <cmath>
#include <fstream>
#include <sstream>

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

Graph genRandomGraph(int n, int m){
    Graph g(n);
    for (int i = 0; i < m; ++i){
        std::srand(std::time(nullptr)); // use current time as seed for random generator
        int u = std::rand()%n;          // generate random integer in [0, INT_MAX] and scale
        int v = std::rand()%n;
        double w = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
        g.add_edge(u,v,w);
    }
    return g;
}

/*
Function to parse an RMAT file into a Graph object
*/
Graph parseRMAT(int n, std::string filename = "rmat.txt"){
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Failed to open the file." << std::endl;
        return 1;
    }
    Graph g(n);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int u, v, w;
        if (iss >> u >> v >> w) {
            g.add_edge(u,v,w);
        } else {
            std::cerr << "Error: Failed to parse line: " << line << std::endl;
        }
    }
    file.close();
    return g;
}

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

    // Graph g(6);

    // g.add_edge(0, 1, 2);
    // g.add_edge(0, 2, 4);
    // g.add_edge(1, 2, 1);
    // g.add_edge(1, 3, 7);
    // g.add_edge(2, 4, 3);
    // g.add_edge(3, 5, 1);
    // g.add_edge(4, 5, 5);
    // g.add_edge(4, 3, 2);

    /*
    Test Graph
    with source node 0
    result should be:
    0 6 4 3 11 10 5 7 14 18 11 10 8 10 18
    */

    Graph g(15);
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


    /*
    Test random graph
    with source node 0
    */
    // g = genRandomGraph(1<<20, 1<<18);


    /*
    Test RMAT:
    rmat_small.txt: 100 nodes, 20 edges
    rmat_medium.txt: 10 000 nodes, 10 000 edges
    rmat_large.txt: 1 000 000 nodes, 100 000 edges
    */
    // Graph g(100);
    // g = parseRMAT(100, "rmat_small.txt");
    // Graph g(10000);
    // g = parseRMAT(10000, "rmat_medium.txt");
    // Graph g(1000000);
    // g = parseRMAT(1000000, "rmat_large.txt");


    std::vector<double> dist;

    std::vector<double> dist_seq;
    std::vector<double> dist_par;
    std::vector<double> dist_dij;

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

        dist_seq = seqDeltaStepping(g, 0, DELTA, 50);
        
        // End the timer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Sequential operation took " << duration << " µs." << std::endl;

        // Start the timer
        start = std::chrono::high_resolution_clock::now();

        dist_par = parDeltaStepping(g, 0, DELTA, 50);  // graph, source node, delta, b (number of buckets)
        
        // End the timer
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Parallel operation took " << duration << " µs." << std::endl;
    } 
    else if (algo == "dijkstra") {
        dist_dij = dijkstra(g, 0);
        std::cout << "Dijkstra" << std::endl;
        std::cout << "Vertex\tDistance from Source" << std::endl;
        for (int i = 0; i < g.size(); ++i) {
            std::cout << i << "\t" << dist_dij[i] << std::endl;
        }
    }

    // print the shortest paths
    std::cout << "Sequential operation" << std::endl;
    std::cout << "Vertex\tDistance from Source" << std::endl;
    for (int i = 0; i < g.size(); ++i) {
        std::cout << i << "\t" << dist_seq[i] << std::endl;
    }

    std::cout << "Parallel operation" << std::endl;
    std::cout << "Vertex\tDistance from Source" << std::endl;
    for (int i = 0; i < g.size(); ++i) {
        std::cout << i << "\t" << dist_par[i] << std::endl;
    }

    // std::ofstream outFile("output_sssp.txt");
    // for (const auto &e : dist) outFile << e << "\n";

    return 0;
}