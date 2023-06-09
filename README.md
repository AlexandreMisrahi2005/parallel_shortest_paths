# Parallel Shortest Paths
 Course Project for CSE305 - Concurrent and Distributed Computing
 
 To run the project: in the `parallel_shortest_paths` folder run `make`. This creates a run file called `sssp` (single-source shortest paths).
 
 EXAMPLE: To run a test on graph *g1* that contains 6 nodes, using the deltastepping algorithm with parameter $\Delta=2$: `./sssp 6 graphs/g1.txt deltastepping 2`. If the output looks like 
```
Sequential operation took 295179 µs.
Parallel operation took 37937 µs.
Sequential operation
Vertex  Distance from Source
0       0
1       2
2       3
3       8
4       6
5       9
Parallel operation
Vertex  Distance from Source
0       0
1       2
2       3
3       8
4       6
5       9
```
then the project runs correctly.
 
Command line arguments:
- N: number of nodes in input graph
- path to input graph file: each line should be a triple u, v, w where u, v are node ids and w is the weight of the edge (u,v). If path is 'random', a random graph with N nodes is generated (the number of edges can be modified in main.cpp). If path is 'grid', a random grid of size n is generated with edges between each node with a probability
- algorithm to use: one of [dijkstra, deltastepping]

Optional:
- delta for deltastepping algorithm (float)
