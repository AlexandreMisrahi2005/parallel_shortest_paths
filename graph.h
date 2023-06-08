#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

typedef std::pair<int, double> Pii; // (vertex, weight)

class Graph
{
    int vertices;                           // number of vertices
    std::vector<std::vector<Pii>> adj_list; // adjacency list

public:
    Graph(int n) : vertices(n)
    {
        adj_list.resize(n);
    }

    // add an edge between vertices u and v with weight
    void add_edge(int u, int v, double weight)
    {
        adj_list[u].push_back(std::make_pair(v, weight));
        // adj_list[v].push_back(std::make_pair(u, weight));  // uncomment to make graph undirected
    }

    int size() const
    {
        return vertices;
    }
    std::vector<std::vector<Pii>> get_adj_list() const
    {
        return adj_list;
    }
};

#endif