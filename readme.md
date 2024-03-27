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

<ul>
	<li>A custom file with the command `<exe> -f <file>` </li>
        <li>An ambedded file (for demonstration purpose) with the command `<exe> -e` </li>
        <li>A random generated graph with the command `<exe> -n <size>`</li>
</ul>

>[!TIP]
>You can also simply launch an example execution with :
>```bash
>make run
>```
>Then the Branch&Bound and the Bruteforce methods attemps to compute the optimal path with a random 12 nodes problem.








