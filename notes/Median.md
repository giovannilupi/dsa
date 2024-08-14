The median of a set of numbers is the value separating the higher half from the lower half. It can be thought of as the **middle number found by ordering all data points and picking out the one in the middle** (**or if there are two middle numbers, taking the mean of those two numbers**). This means that half of the data points are smaller than the median and half of the data points are larger.

Formally, for a data set ${\displaystyle x}$ of ${\displaystyle n}$ elements, ordered from smallest to greatest:

- If ${\displaystyle n}$ is odd: $med ⁡ ( x ) =  {\displaystyle \operatorname x_{(n+1)/2}}$
- If ${\displaystyle n}$ is even: $med ⁡ ( x ) = {\displaystyle \operatorname {\frac {x_{(n/2)}+x_{((n/2)+1)}}{2}}}$

This means that, if the number of elements in the set is even, the value of the result may not belong to the set.

The problem of finding the median is customarily connected with that of sorting because one way to find the median is to sort the n elements and pick the item in the middle. However, the problem can be solved much more efficiently by partitioning. In fact, **finding the median is a special case of a more general problem, which is that of finding the kth smallest of n items. The median represents the special case where k  = n / 2**. 

One possible algorithm to solve this problem is the Hoare's FIND algorithm. This algorithm leverages the same partitioning operation of quicksort. In the average case, it only requires 2n operations. As quicksort, if the pivot is chosen incorrectly, each partition may reduce the size of the array by only one element. In such cases, the complexity reaches $O(n^2 )$.