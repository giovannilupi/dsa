**Recursion is a method of solving a computational problem where the solution depends on solutions to smaller instances of the same problem**. Recursion solves such recursive problems by using functions that call themselves from within their own code. **A function that calls itself directly or indirectly is said to be recursive**. In particular, if a procedure *P* contains an explicit reference to itself, it is said to be directly recursive. If *P* contains a reference to another procedure *Q* which references (directly or not) *P*, then *P* is said to be indirectly recursive. This means that the use of recursion may not be immediately apparent.

An object is said to be recursive when it partially consists or is defined in terms of itself. One example is that of natural numbers:

- 1 is a natural number
- the successor of a natural number is a natural number

**Recursive algorithms are primarily appropriate when the problem to be solved, or the function to be computed, or the data structure to be processed are already defined in recursive terms**. **This, however, does not guarantee that such recursive definitions guarantee that a recursive algorithm is the best way to solve the problem**.

### Definition

An algorithm exhibits recursive behavior when it can be defined by two properties:

1.  A simple base case (or cases) — a terminating scenario that does not use recursion to produce an answer
2.  A recursive step — a set of rules that reduces all successive cases toward the base case.

The problem of termination is fundamental when writing recursive procedure. **The recursive call must be subject to a condition which at some point becomes non-satisfied**. An algorithm can have one or more base cases, for which a result can be produced trivially. Similarly, an algorithm can have multiple recursive cases. The job of the recursive cases can be seen as breaking down complex inputs into simpler ones.

**In practice, it is also mandatory to show that the ultimate depth of recursion is not only finite, but that is actually small**. This is because each procedure invocation requires some amount of storage in the call stack to accomodate its variables.

### Avoiding recursion

A typical example of algorithm that can be naturally defined recursively is that of computing the $n$th number of the Fibonacci sequence. **Just because an algorithm can be defined recursively, that does not mean that a recursive implementation is the best way to solve the problem**. This is a typical implementation of Fibonacci:

```C++
template <integral T>
T fib(T n) {
    if (n == 0 || n == 1) return n;
    return fib(n-1) + fib(n-2);
}
```

In this example we see that each call with $n>1$ causes 2 further calls, so the total number of calls grows exponentially. This type of implementation is clearly unpractical.

Similarly, computing the factorial of a number is another problem that can be solved easily using recursion, but causes the size of the stack to grow linearly with the number to compute. An iterative approach is clearly preferable for such a case.

Typically, there is a significant time and space cost associated with recursive programs, due to the overhead required to manage the stack and the relative slowness of function calls. For example, an iterative implementation of the factorial can be several order of magnitudes faster than the recursive approach.

Every recursive program can be transformed into a purely iterative one. The key point is that some algorithms are iterative in nature and transforming them to iterative ones would imply manually handling a recursion stack and possibly losing clarity. **Algorithms that are recursive in nature should be formulated as recursive**.

### Tail recursion

**A tail call is a subroutine call performed as the final action of a procedure. If the target of a tail is the same subroutine, the subroutine is said to be tail recursive**. In other words, a recursive function is tail recursive if the final result of the recursive call is the final result of the function itself. If the result of the recursive call must be further processed (say, by adding 1 to it), it is not tail recursive.

Tail calls can be implemented without adding a new stack frame to the call stack. Most of the frame of the current procedure is no longer needed, and can be replaced by the frame of the tail call, modified as appropriate. The program can then jump to the called subroutine. In other terms, **since in a tail call the containing function is about to return, its environment can be discarded and the recursive call can be entered without creating a new stack frame**. For tail calls, there is no need to even remember the caller: the tail-called function can return directly to the original caller. Producing such code instead of a standard call sequence is called tail-call elimination or tail-call optimization.

Tail-call elimination often reduces asymptotic stack space requirements from linear, or $O(n)$, to constant, or $O(1)$.

Notice that not all tail calls are necessarily located at the syntactical end of a subroutine:

```
function bar(data) {
    if (a(data)) {
        return b(data);
    }
    return c(data);
}
```

Here, both calls to `b` and `c` are in tail position. This is because each of them lies in the end of an if-branch, even though the first one is not syntactically at the end of bar's body. Another example is the tail call to `a` here:

```
function foo2(data) {
    var ret = a(data);
    return ret;
}
```

**Writing a tail recursive function often requires the use of an additional accumulator parameter as input to the function**. In theory, we can convert any non-tail recursive function to a tail recursive function by passing additional parameters to maintain the state of the recursive call.