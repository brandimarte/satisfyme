# SatisfyMe! #

Given a set *U* = {*x<sub>1</sub>, x<sub>2</sub>, ... , x<sub>n</sub>*} of boolean variables (literals) and a boolean formula *k*-CNF (conjunctive normal form composed by "and" of clauses in {*c<sub>1</sub>, c<sub>2</sub>, ..., c<sub>m</sub>*}, where each clause consists in "or" of *k* literals belonging to the set *U* plus respective negations), the main objective of this program consists on searching attributions (true and false) for the set of literals such that the *k*-CNF is satisfied (true).
This is a NP-complete problem known as *k*-SAT.

The *k*-SAT problem can be interpreted as a generalized cover set problem and, therefore, can be solved by a backtracking algorithm based on the D. Knuth's [Dancing Links X][KnuthDLX] algorithm.

Besides solving the *k*-SAT problem, the code is also able to determine the solutions, if any, of an exact cover set problem.

## Input and Output ##

For the *k*-CNF, the program reads an input from `stdin` containing integer numbers, where the first line corresponds to "*k n m*" (amount of literals, cardinal of *U* and number of clauses, respectively) and the following lines correspond to the clauses, where a number *i* represents the variable *x<sub>i</sub>* and *-i* its negation !*x<sub>i</sub>* (i.e., *not x<sub>i</sub>*).

The program output is send to `stdout`.
With the command line option `-n` the code prints out the number of different attributions that satisfy the input.
With the option `-N`, besides printing the number of attributions that satisfy the input, the code also prints all these attributions, line by line and in lexicographical order.
In case no option is passed through the command line, the code prints `1` if the *k*-CNF is satisfiable or `0`, otherwise.

The command line option `-C` indicates that the input contains an exact cover set problem.
In this case, the input represents the problem of covering a set [*n*] = {*1, 2, ... , n*} with elements from a collection of *m* subsets of [*n*].
The first line corresponds to "*n m*" (indicating the set [*n*] and the amount of subsets of [*n*], respectively) and the following *m* lines correspond to the subsets of [*n*], represented by integers belonging to [*n*] in ascending order.
As output, the code prints out the number of different solutions for the problem, followed by the respectively solutions, line by line and represented by numbers belonging to [*n*] in ascending order.

## *k*-SAT as a Cover Set Problem ##

The *k*-SAT problem can be interpreted as a generalized cover set problem.
Adopting the convention `1 = true` and `0 = false`, one can represent a *k*-SAT by a matrix with the rows containing the possible values for which the variables {*x<sub>1</sub>, x<sub>2</sub>, ... , x<sub>n</sub>*} can assume (`0` or `1`), and with the primary columns corresponding to the literals (columns that should be exactly covered, i.e., the subset of rows from a solution must contain exactly one `1` at each of those columns) and the secondary columns corresponding to the clauses {*c<sub>1</sub>, c<sub>2</sub>, ..., c<sub>m</sub>*} which should be covered at least once by the set of rows of a given solution.

For example, consider the input:

     3  4  5
    -1  2  3
     1 -2  4
     1 -3  4
    -1 -2  3
    -1  2 -3

which corresponds to the following *k*-CNF:

(!*x<sub>1</sub>* || *x<sub>2</sub>* || *x<sub>3</sub>*) && (*x<sub>1</sub>* || !*x<sub>2</sub>* || *x<sub>4</sub>*) && (*x<sub>1</sub>* || !*x<sub>3</sub>* || *x<sub>4</sub>*) && (!*x<sub>1</sub>* || !*x<sub>2</sub>* || *x<sub>3</sub>*) && (!*x<sub>1</sub>* || *x<sub>2</sub>* || !*x<sub>3</sub>*)

Therefore, this boolean formula can be represented by the matrix:

              x_1  x_2  x_3  x_4  c_1  c_2  c_3  c_4  c_5
    x_1 = 0 |  1    0    0    0    1    0    0    1    1  |
    x_1 = 1 |  1    0    0    0    0    1    1    0    0  |
    x_2 = 0 |  0    1    0    0    0    1    0    1    0  |
    x_2 = 1 |  0    1    0    0    1    0    0    0    1  |
    x_3 = 0 |  0    0    1    0    0    0    1    0    1  |
    x_3 = 1 |  0    0    1    0    1    0    0    1    0  |
    x_4 = 0 |  0    0    0    1    0    0    0    0    0  |
    x_4 = 1 |  0    0    0    1    0    1    1    0    0  |

Thus, as the primary columns (first four) correspond to an exact cover set problem, once selected the row *x<sub>i</sub>* = 0, the row *x<sub>i</sub>* = 1 cannot be in the solution, and vice-versa.
However, the same criteria cannot be used for the columns representing the clauses, since a clause is satisfied when one or more literals are true.

In the example above, one attribution that satisfies the *k*-CNF is *x<sub>1</sub>*, *x<sub>2</sub>* and *x<sub>3</sub>* true and *x<sub>4</sub>* false, represented by the rows *x<sub>1</sub>* = 1, *x<sub>2</sub>* = 1, *x<sub>3</sub>* = 1 and *x<sub>4</sub>* = 0 (or, in a compact form, `1110`).

## The X Algorithm ##

The [X algorithm from D. Knuth][KnuthDLX] is a backtracking search algorithm that can be applied to a class of combinatorial puzzle problems, such as to find all solutions of an exact cover problem.
It's based on a technique called *dancing links*: given a pointer *x* to an element from a double linked list, and let *L*[*x*] and *R*[*x*] be the pointers to the precedent and posterior elements, respectively, then the removal of *x* from the list is given by the operations:

    L[R[x]] <- L[x]
    R[L[x]] <- R[x]

However, since after removal the element pointed by *x* is still allocated in memory, its return to the list is given by the operations:

    L[R[x]] <- x
    R[L[x]] <- x

what allow thus return back to the previous stage (*backtrack*).


<!---
Links to external and internal sites.
-->
[KnuthDLX]: https://arxiv.org/abs/cs/0011047v1
