## Desription
Here is a prototype solver implementation in full C of a Travelling Salesman Problem (TSP) using the Branch&Bound method. 
Please note that it is a prototype and then an non-ideal implementation in termes of readability, bugs and performance. 

## Compilation
We provide a `Makefile` file to compile the branch&bound and the Bruteforce solvers : 
```bash 
make -j 
```

## Execution
In both `branch` and `bruteforce` solvers, you can launch the resolution with : 

- A custom file with the command `<exe> -f <file>`</li>
- An embedded file (for demonstration purpose) with the command `<exe> -e` </li>
- A random generated graph with the command `<exe> -n <size>`</li>


>[!TIP]
>You can also simply launch an example execution with :
>```bash
>make run
>```
>Then the Branch&Bound and the Bruteforce methods attemps to compute the optimal path with a random 12 nodes problem.

## file format

The solvers may need an input file for the `<exe> -f <file>` alternative. the input file need to be in a precise format. Here is a short description. 
Consider that we need to input a `n` sized graph. 
- The input file is composed of 5 lines of 5 numbers each containing 5 numbers representing the cost between the corresponding two nodes. 
- We can use `inf` keyword to set an infinite distance in a edge between the corresponding two edges

Here is an example of a simple input file : 

```
inf 7 9 6 1
7 inf 3 4 9
9 3 inf 2 5
6 4 2 inf 3
1 9 5 3 inf
```






