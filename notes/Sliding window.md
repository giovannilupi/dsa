Sliding Window problems are problems in which **a fixed or variable-size window is moved through a data structure** to solve problems efficiently based on continuous subsets of elements. This type of problem strictly **pertains to data structures with contiguous elements**. The idea is to slide a window over the data structure to **find a contiguous subset that verifies a given set of conditions**. Typical examples of conditions we could look for are min, max, longer, shortest, a specific sum, ecc.

There are two main types of sliding window problems:

1.  Fixed size window.
2.  Dynamically resizable window.

### Fixed size window

In the second, the window expands on the right side and restricts on the left side. You can imagine it moving like a caterpillar. Sometimes, these dynamic variants require the use of an auxiliary data structure. This can arise from the presence of multiple constraints. For example, finding the longest substring with no more than k distinct characters. This problem requires an additional map to count the occurences of each character.

The naive way to solve this problem is using a bruteforce approach. This implies repeating a lot work, rescanning elements that were already visited. The bruteforce approach has a complexity of *O($n^2$)*. The sliding window technique allow to solve problems with linear complexity.

A typical problem is the maximum sum of a contiguous subarray of size 3.

![73cc7f02c27dbc6ebfd0f47e38cfac42.png](../_resources/73cc7f02c27dbc6ebfd0f47e38cfac42.png)

###  Variable Size Window  
For example: Find length of the longest substring without repeating characters.

&nbsp;

Smallest subarray with the given sum.