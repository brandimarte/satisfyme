# SatisfyMe!

Given a set *U* = {*x<sub>1</sub>, x<sub>2</sub>, ... , x<sub>n</sub>*} of boolean variables (literals) and a boolean formula *k*-CNF (conjunctive normal form composed by a set {*c<sub>1</sub>, c<sub>2</sub>, ..., c<sub>m</sub>*} of clauses, where each clause comprises of *k* literals belonging to the set *U* and respective negations), the main objective of this program consists on searching attributions (true and false) for the set of literals such that the *k*-CNF is satisfied (true).
This is a NP-complete problem known as *k*-SAT.

The *k*-SAT problem can be interpreted as a generalized cover set problem and, therefore, can be solved by a backtracking algorithm based on the D. Knuth's "Dancing Links X" algorithm.

Besides solving the *k*-SAT problem, the code is also able to determine the solutions, if any, of an exact cover set problem.

## Input and Output ##

For the *k*-CNF, the program reads an input from `stdin` containing integer numbers, where the first line corresponds to "*k n m*" (amount of literals, cardinal of *U* and number of clauses, respectively) and the following lines correspond to the clauses, where a number *i* represents the variable *x<sub>i</sub>* and *-i* its negation *~x<sub>i</sub>*.

The program output is send to `stdout`.
With the command line option `-n` the code prints out the number of different attributions that satisfy the input.
With the option `-N`, besides printing the number of attributions that satisfy the input, the code also prints all these attributions, line by line and in lexicographical order.
In case no option is passed through the command line, the code prints `1` if the *k*-CNF is satisfiable or `0`, otherwise.

The command line option `-C` indicates that the input contains an exact cover set problem.
In this case, the input represents the problem of covering a set [*n*] = {*1, 2, ... , n*} with elements from a collection of *m* subsets of [*n*].
The first line corresponds to "*n m*" (indicating the set [*n*] and the amount of subsets of [*n*], respectively) and the following *m* lines correspond to the subsets of [*n*], represented by integers belonging to [*n*] in ascending order.
As output, the code prints out the number of different solutions for the problem, followed by the respectively solutions, line by line and represented by numbers belonging to [*n*] in ascending order.
