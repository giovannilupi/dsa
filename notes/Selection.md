**A selection algorithm is an algorithm for finding the $k$th smallest value in a collection of values**, such as numbers. The value that it finds is called the $kth$ order statistic. In other terms, given a set of $n$ elements, its $k$th order statistics is the $k$th smallest element. So the minimum of a set is the first order statistic, while the maximum is the $n$th order statistic. The median, is the $n/2$th order statistic (although if $n$ is even, we should distinguish between lower and upper median).  **Minimum, maximum, and median are therefore special cases of the selection problem**.

**The selection problem can be solved in $Θ(n log n)$ time by simply sorting the numbers and outputting the $k$th element of the sorted array.** This method is very simple and also produces a sorted version of the input collection, which may be useful for further computations. **However, specialized selection algorithms exist with an everage complexity of $O(n)$**.

### Minimum and maximum

For determining minimum and maximum, there is a a lower bound of $n-1$ comparisons. This means the best way to solve the problem is to simply examine each element in the array in turn and keep track of the smallest/biggest value seen thus far.

If we want to find both the minimum and maximum from a collection, there is a more efficient algorithm than determining each value independently (the asymptotic complexity is the same, but we can improve on the constants). While scanning the array, we can keep track of the min and max seen thus far. The trick is to process the elements of the array in pairs. We first compare the elements in the input pair between each other, and then copare the smaller with the current minimum and the larger with the current maximum. This costs 3 comparisons every two elements, instead of 4 as in the naive algorithm.
More in detail, if $n$ is odd, we set both min and max to the value of the first element, and then start processing the remaining input in pairs. If $n$ is even, we set the initial value of min and max using the first pair, and then loop through the remaining pairs. 

### Linear selection

The selection problem can be solved with average linear time by leveraging technique similar to the ones employed for sorting algorithms.

#### Quick select

Quickselect uses the same overall approach as quicksort, choosing one element as a pivot and partitioning the data in two based on the pivot, accordingly as less than or greater than the pivot. However, instead of recursing into both sides, as in quicksort, **quickselect only recurses into one side – the side with the element it is searching for**. 

As in quicksort, the core of the algorithms is the partitioning procedure. **After partitioning the container, the pivot will be in its final position. If that position happens to be equal to $k$, it means that element is the one we're looking for**. Otherwise, we can compare the position of the pivot with $k$. If $k$ is smaller than the pivot index, it means we have to look in the left partition. Otherwise, we look in the right one. 

Notice that, **because we focus only on one partition at a time**, it's easy to make the implementation tail recursive. Even better, **we can easily make the  implementation iterative**.

The worst case running time for quick select is the same as quicksort, which is $Θ(n^2)$. This is however very unlikely if the pivot is chosen randomly. Normally, if a good pivot is chosen, the search set decreases in size exponentially at each iteration. Since the cost of the partition procedure is $n$, if good pivots are chosen, we  will end up with a cost of $C n$, where $C$ is a constant.

#### Heap select

Similartly to quick select, we can leverage the heap sort extraction mecahnism to solve the selection problem in linear time. The fundamental idea is the same: achieve a speedup by partially sorting the array, which achieves a speedup compared to full sorting. We can use `build-heap` to create a heap, and the apply `extract` $k$ times to select the first $k$ elements.