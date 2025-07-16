# TSP Solver

## Authors
- **Author:** Sébastien Dubois
- **Professor:** Devan Sohier

## Description
This project provides a C implementation of two solvers for the Travelling Salesman Problem (TSP): a brute-force solver and a branch-and-bound solver. The TSP, also known as the Hamiltonian path problem on a complete graph, is a classic algorithmic challenge where the goal is to find the shortest possible route that visits each city exactly once before returning to the origin city.

The cost of traveling from one city to another is not necessarily symmetric. The programs were developed and tested on Arch Linux (`5.17.3-arch1-1`) with `gcc@11.3.0`.

## How to Compile
A `Makefile` is provided for easy compilation of both the branch-and-bound and the brute-force solvers.

To compile the solvers, run the following command:
```bash
make
```

This will generate two executables: `branch` and `bruteforce`.

## How to Use It
Both the `branch` and `bruteforce` solvers can be executed with the following options:

- **With a custom file**:
  ```bash
  ./<solver> -f <file_path>
  ```
- **With an embedded file (for demonstration purposes)**:
  ```bash
  ./<solver> -e
  ```
- **With a randomly generated graph of a given size**:
  ```bash
  ./<solver> -n <size>
  ```

Replace `<solver>` with either `branch` or `bruteforce`, `<file_path>` with the path to your input file, and `<size>` with the desired number of cities for the random graph.

### File Format
The input file for the `-f` option must be in a specific format. For a graph with `n` cities, the file should contain `n` lines, each with `n` space-separated numbers representing the cost of travel between the corresponding cities. The keyword `inf` can be used to represent an infinite distance between two cities.

Here is an example of a 5x5 input file:
```
inf 7 9 6 1
7 inf 3 4 9
9 3 inf 2 5
6 4 2 inf 3
1 9 5 3 inf
```

## How It Works
The project implements two different algorithms to solve the TSP:

### Brute-Force
The brute-force approach systematically enumerates all possible permutations of cities to find the one with the minimum total distance. It begins at a starting city, explores every possible path, and calculates the length of each. The path with the smallest total length is the solution. While this method guarantees finding the optimal solution, its computational complexity of O(n!) makes it impractical for large numbers of cities.

The core of the brute-force algorithm is a recursive function that generates all possible permutations of the cities. Here is a simplified pseudo-code representation of the permutation logic:

```c
void permutation(array, l, r) {
    if (l == r) {
        // A complete path has been formed, calculate its score
        score(array);
    } else {
        for (i from l to r) {
            swap(array[l], array[i]);
            permutation(array, l + 1, r);
            swap(array[l], array[i]); // backtrack
        }
    }
}
```

This recursive function explores all possible orderings of cities. For each complete path, the `score` function calculates the total distance and updates the best solution found so far if the current path is shorter.

### Branch and Bound
The Branch and Bound method (in French, *Séparation et évaluation*) is an optimized algorithm for solving combinatorial problems. It performs an intelligent search of the solution space by systematically dividing the problem into smaller sub-problems (branching) and eliminating those that are not promising (bounding).

The algorithm works as follows:
1.  **Initialization**: The algorithm starts at a root node, which represents the initial state of the problem. An initial upper bound for the optimal solution's cost is set (e.g., to infinity or a value from a fast heuristic).
2.  **Branching**: The problem is divided into smaller sub-problems, creating child nodes in the search tree. In the context of TSP, this means selecting the next city to visit from the current one.
3.  **Bounding**: For each new node, a lower bound on the cost of any solution that can be derived from that node is calculated. If this lower bound is greater than the current upper bound, the node (and the entire branch it represents) is pruned from the tree, as it cannot lead to a better solution than what has already been found.
4.  **Selection**: The algorithm selects the most promising node to explore next, typically the one with the lowest lower bound.
5.  **Termination**: The process continues until a complete solution is found with a cost that is less than or equal to the lower bound of all other unexplored nodes. This solution is guaranteed to be optimal.

The effectiveness of the Branch and Bound method depends heavily on the quality of the bounding function. A tight lower bound allows for more effective pruning, significantly reducing the search space and improving performance over a naive brute-force search.

### Implementation Details

#### Data Structures
- `s_problem`: A struct to manage data parsed from an input file, holding the raw configuration and the resulting cost matrix.
- `s_node`: Represents a node in the search tree. It stores its own modified cost matrix, the path taken to reach it, the cumulative cost, and its level in the tree.
- `reduced_matrix`: The cost matrix within each `s_node` is stored as a one-dimensional array rather than a 2D array. This improves cache performance and simplifies memory management.
- `s_segment`: A simple struct with `from` and `to` fields, used to represent the segments of a path.
- `data.nodes`: A dynamic array that holds all the live nodes in the search tree. The node with the smallest cost is extracted at each iteration, and its children (newly generated nodes) are added to the list.

#### Memory Complexity
A key detail of this implementation is that each node in the search tree stores its own copy of the reduced cost matrix. This leads to high memory consumption, as the number of nodes can grow very large. An alternative approach with lower memory overhead would be to recalculate the matrix for each node as it's being processed, trading CPU cycles for memory.

## Example of a Solution
Here is an example of the output when running the solvers on the embedded 5-city problem from `configuration.txt`:

**Branch and Bound:**
```
$ ./branch -e
--- Branch And Bound method --- 
You selected the embedded file option.
Read file with success ! 
read data ..
Size of graph : 5

Solution :
 - Length : 16.000000
 - Path   : 0 - 4 - 3 - 2 - 1 - 0

Elapsed time :
 - s      : 0.000012
 - ms     : 0.012320
```

**Brute-Force:**
```
$ ./bruteforce -e
-----  Bruteforce method ----- 
You selected the embedded file option.
Read file with success ! 
read data ..
Size of graph : 5

Solution : 
 - Length : 16.000000
 - Path   : 0 - 1 - 2 - 3 - 4 - 0

Elapsed time : 
 - s      : 0.000011
 - ms     : 0.011190
```
> **Note:** The optimal paths found by the two methods are reversed. This is because the cost matrix in this example is symmetric, meaning the cost of traveling `A -> B` is the same as `B -> A`. As a result, there are at least two optimal paths with the same total length.

Running on a larger 10-city problem from `configuration_large.txt` highlights the performance difference:

**Branch and Bound (10 cities):**
```
$ ./branch -f configuration_large.txt
--- Branch And Bound method --- 
You selected the file option.
Read file with success ! 
read data ..
Size of graph : 10

Solution :
 - Length : 37.000000
 - Path   : 0 - 6 - 4 - 2 - 3 - 5 - 8 - 9 - 1 - 7 - 0

Elapsed time :
 - s      : 0.000336
 - ms     : 0.335880
```

**Brute-Force (10 cities):**
```
$ ./bruteforce -f configuration_large.txt
-----  Bruteforce method ----- 
You selected the file option.
Read file with success ! 
read data ..
Size of graph : 10

Solution : 
 - Length : 37.000000
 - Path   : 0 - 6 - 4 - 2 - 3 - 5 - 8 - 9 - 1 - 7 - 0

Elapsed time : 
 - s      : 0.004558
 - ms     : 4.557880
```
On this larger problem, the Branch and Bound method is over 10x faster.

## Performance
To provide a clear comparison of the two algorithms, we benchmarked their performance on randomly generated problems of increasing size. The tests were compiled with `gcc -Ofast` and run on an Arch Linux machine. The execution time for each solver is shown in the table below.

| Graph Size (n) | Branch & Bound (ms) | Brute-Force (ms) |
| :---: | :---: | :---: |
| 2 | 0.002 | 0.018 |
| 3 | 0.004 | 0.012 |
| 4 | 0.007 | 0.016 |
| 5 | 0.016 | 0.017 |
| 6 | 0.024 | 0.019 |
| 7 | 0.062 | 0.027 |
| 8 | 0.121 | 0.106 |
| 9 | 0.120 | 0.793 |
| 10 | 0.361 | 6.400 |
| 11 | 1.164 | 62.011 |
| 12 | 1.727 | 553.986 |
| 13 | 0.875 | 4235.917 |
| 14 | 1.302 | 56149.168 |

As the table shows, the execution time of the brute-force algorithm grows exponentially with the problem size, which is consistent with its O(n!) complexity. The Branch and Bound algorithm, on the other hand, is significantly more efficient. While its performance can vary depending on the specific problem instance (as seen with n=13), it consistently outperforms the brute-force method by several orders of magnitude on larger problems.

## TODO
- [x] Implement brute-force solver
- [x] Implement branch-and-bound solver
- [ ] Add support for different input formats (e.g., TSPLIB)
- [ ] Implement a graphical user interface (GUI) to visualize the paths
- [ ] Add more advanced TSP algorithms (e.g., genetic algorithms, simulated annealing)
- [ ] Improve the performance of the branch-and-bound solver
- [ ] Add unit tests

## HPC Point of View
From a High-Performance Computing (HPC) perspective, the two algorithms present different challenges and opportunities for parallelization.

The **brute-force algorithm** is a classic example of an "embarrassingly parallel" problem. Each permutation of cities can be evaluated independently. This means the workload can be easily distributed across multiple processors or compute nodes with minimal communication overhead, making it a good candidate for parallelization with frameworks like MPI or OpenMP.

The **branch-and-bound algorithm** is more complex to parallelize. While different branches of the search tree can be explored in parallel, processes need to communicate to share the current best solution's cost (the global upper bound). This shared state is crucial for effective pruning across all parallel tasks. The efficiency of a parallel implementation depends heavily on the strategy for balancing the workload and managing the communication of this global bound.

Furthermore, the performance of the Branch and Bound method is highly dependent on the input data. In the worst-case scenario (where few branches can be pruned), its complexity can approach that of the brute-force algorithm. Parallelism can help here; by having different workers explore the search tree in different ways, one might find a good solution faster, helping to prune the work of the others.

## Conclusion
This project serves as a practical demonstration of two fundamental approaches to solving the Traveling Salesman Problem, highlighting the trade-offs between solution optimality, computational complexity, and implementation difficulty. The Branch and Bound method proves to be a significantly more efficient approach than a naive brute-force search for non-trivial problem sizes. While the current implementation is effective, there are many avenues for further exploration, including more advanced optimization algorithms, parallelization strategies, and more memory-efficient data management techniques.






