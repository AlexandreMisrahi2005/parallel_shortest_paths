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
Graph parseGraph(int n, std::string filename = "graphs/g1.txt"){
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

Graph genGridGraph(int n, double edge_proba){
    Graph g(n);
    double w;
    double proba;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int node = i * n + j;
            
            // left node
            if (j > 0) {
                int left = node - 1;
                w = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                proba = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                if(proba > edge_proba){
                    g.add_edge(node, left, w);
                }
            }

            // right node
            if (j < n - 1) {
                int right = node + 1;
                w = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                proba = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                if(proba > edge_proba){
                    g.add_edge(node, right, w);
                }
            }

            // north node
            if (i > 0) {
                int north = node - n;
                w = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                proba = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                if(proba > edge_proba){
                    g.add_edge(node, north, w);
                }
            }

            // south node
            if (i < n - 1) {
                int south = node + n;
                w = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                proba = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // random double in [0,1]
                if(proba > edge_proba){
                    g.add_edge(node, south, w);
                }
            }
        }
    }
    return g;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] 
                  << " \n<N: number of nodes in input graph> "
                  << " \n<path to input graph file: each line should be a triple u, v, w where u, v are node ids and w is the weight of the edge (u,v). If path is 'random', a random graph with N nodes is generated (the number of edges can be modified in main.cpp). If path is 'grid', a random grid of size n is generated with edges between each node with a probability> "
                  << " \n<algorithm to use: [dijkstra, deltastepping]> "
                  << " \n<optional: delta for deltastepping algorithm: [float]> " << std::endl;
        return 1;
    }

    // parse arguments
    int N = std::stoi(argv[1]);
    std::string graph_file = argv[2];
    std::string algo = argv[3];

    Graph g(N);
    if(graph_file == "random"){
        g = genRandomGraph(N, 100); // change number of edges in the second argument
    }
    else if (graph_file == "grid"){
        g = genGridGraph(N, 0.9); // change edge probability in the second argument
    }
    else{
        g = parseGraph(N, graph_file);
    }

    /*
    TEST GRAPHS:

        graphs/g1.txt
        Test graph from Abdul Bari (youtube)
        with source node 0
        result should be:
        0 2 3 8 6 9

        graphs/g2.txt
        Test Graph
        with source node 0
        result should be:
        0 6 4 3 11 10 5 7 14 18 11 10 8 10 18

        graphs/rmat_small.txt, graphs/rmat_medium.txt, graphs/rmat_large.txt
        Test RMAT:
        rmat_small.txt: 100 nodes, 7000 edges
        rmat_medium.txt: 10 000 nodes, 70 000 edges
        rmat_large.txt: 1 000 000 nodes, 80 000 000 edges

    */


    std::vector<double> dist_seq;
    std::vector<double> dist_par;
    std::vector<double> dist_dij;

    // Find the shortest paths from vertex 0
    if (algo == "deltastepping")
    {
        std::vector<std::vector<Pii>> adjMat = g.get_adj_list();
        std::cout << adjMat.size() << std::endl;
        Pii p = smallestAndLongestEdges(adjMat, g.size());
        double DELTA = p.first;
        if (argc == 5)
        {
            DELTA = std::stod(argv[4]);
        }
        int b = 1 + std::ceil(p.second / DELTA);
        b = 500000;

        // Start the timer
        auto start = std::chrono::high_resolution_clock::now();

        dist_seq = seqDeltaStepping(g, 0, DELTA, b);
        
        // End the timer
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Sequential operation took " << duration << " µs." << std::endl;

        // Start the timer
        start = std::chrono::high_resolution_clock::now();

        dist_par = parDeltaStepping(g, 0, DELTA, b);  // graph, source node, delta, b (number of buckets)
        
        // End the timer
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Parallel operation took " << duration << " µs." << std::endl;


        // print the shortest paths if graph is small
        if(N < 20){
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
        }
    } 
    else if (algo == "dijkstra") {
        dist_dij = dijkstra(g, 0);
        std::cout << "Dijkstra" << std::endl;
        std::cout << "Vertex\tDistance from Source" << std::endl;
        for (int i = 0; i < g.size(); ++i) {
            std::cout << i << "\t" << dist_dij[i] << std::endl;
        }
    }

    std::ofstream outFile("output_dijkstra.txt");
    for (const auto &e : dist_dij) outFile << e << "\n";

    return 0;
}