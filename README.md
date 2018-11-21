# SatisfyMe!

Given a set *U = { x<sub>1</sub>, x<sub>2</sub>, ... , x<sub>n</sub> }* of boolean variables (literals) and a boolean formula *k*-CNF (conjunctive normal form composed by a set *{ c<sub>1</sub>, c<sub>2</sub>, ..., c<sub>m</sub> }* of clauses, where each clause comprises of *k* literals belonging to the set *U* and respective negations), the main objective of this program consists on searching attributions (true and false) for the set of literals such that the *k*-CNF is satisfied (true).
This is a NP-complete problem known as *k*-SAT.

The *k*-SAT problem can be interpreted as a generalized cover set problem and, therefore, it can be solved by a backtracking algorithm based on the D. Knuth's "Dancing Links X" algorithm.

Besides solving the *k*-SAT problem, the code can receive an exact cover problem and determines whether it has a solution.
