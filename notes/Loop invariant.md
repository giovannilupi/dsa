**A loop invariant is a property of a program loop that is true before (and after) each iteration**. Note that this says nothing about its truth or falsity part way through an iteration. It is a predicate (condition) that holds for every iteration of the loop.

For example, let’s look at a simple for loop that looks like this:
```
int j = 9;
for(int i=0; i<10; i++)  
j--;
```
In this example it is true (for every iteration) that i + j == 9.

When using an invariant, we need to show three things:
1.  **Initialization**: it is true prior to the first iteration of the loop.
2.  **Maintenance**: if it is true before an iteration of the loop, it remains true before the next iteration.
3.  **Termination**: the loop terminates. When it does, the invariant, along with the loop termination condition, can give us some useful property to prove the correctness of the algorithm.

So **a loop invariant can be used to prove properties of loops and by extension algorithms that employ loops** (usually correctness properties). In particular, the loop invariants will be true on entry into a loop and following each iteration, so that on exit from the loop both the loop invariants and the loop termination condition can be guaranteed.

A loop-invariant proof is a form of mathematical induction, where to prove that a property holds, you prove a base case and an inductive step. Here, showing that the invariant holds before the first iteration corresponds to the base case, and showing that the invariant holds from iteration to iteration corresponds to the inductive step. While mathematical induction goes on indefinitely, in a loop invariant the induction stops when the loop terminates.
