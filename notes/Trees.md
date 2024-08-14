# Trees

A tree is a hierarchical structure with a set of connected nodes. Each node in the tree can be connected to many children but must be connected to exactly one parent, except for the root node, which has no parent. This means that **each child can be treated like the root node of its own subtree, making recursion a useful technique for tree traversal**.

A node is a structure which may contain data and connections to other nodes, sometimes called edges or links. Each node in a tree has zero or more child nodes, which are below it in the tree. A node that has a child is called the child's parent node. All nodes have exactly one parent, except the topmost root node, which has none. A node might have many ancestor nodes, such as the parent's parent. Child nodes with the same parent are sibling nodes. The height of a node is the length of the longest downward path to a leaf from that node. The height of the root is the height of the tree. The depth of a node is the length of the path to its root. 

The typical way to represent trees is to have nodes as dynamically allocated records with pointers to their children, their parents, or both, as well as any associated data. In particular cases, other representations are possible. For example, nodes may be store in an array, which is the case for binary heaps.

# Binary trees

A binary tree is a tree data structure in which each node has at most two children, referred to as the left child and the right child.

Types of binary trees:
- A full binary tree is a tree in which every node has either 0 or 2 children.
- A perfect binary tree is a binary tree in which all interior nodes have two children and all leaves have the same depth or same level.
- A complete binary tree is a binary tree in which every level, except possibly the last, is completely filled, and all nodes in the last level are as far left as possible.
- A balanced binary tree is a binary tree structure in which the left and right subtrees of every node differ in height by no more than 1.
- A degenerate (or pathological) tree is where each parent node has only one associated child node. This means that the tree will behave like a linked list data structure.

## Traversal

**Tree traversal refers to the process of visiting (e.g. retrieving, updating, or deleting) each node in a tree data structure, exactly once**. **Such traversals are classified by the order in which the nodes are visited**.

In fact, unlike linear data structures, which are canonically traversed in linear order, trees may be traversed in multiple ways. **The broadest distinction is between depth-first or breadth-first order**.

**Traversing a tree involves iterating over all nodes in some manner. Because from a given node there is more than one possible next node (it is not a linear data structure), then, some nodes must be deferred—stored in some way for later visiting**. This is often done via a stack (LIFO) or queue (FIFO). **As a tree is a self-referential (recursively defined) data structure, traversal can be defined by recursion in a natural and clear fashion; in these cases the deferred nodes are stored implicitly in the call stack**. 

Depth-first search is easily implemented via a stack, including recursively (via the call stack), while breadth-first search is easily implemented via a queue, including corecursively.

### Depth-first search (DFS)

**In depth-first search (DFS), the search tree is deepened as much as possible before going to the next sibling**.  To traverse binary trees with depth-first search, perform the following operations at each node:

1. If the current node is empty then return.
2. Execute the following three operations in a certain order:
	- N: Visit the current node.
	- L: Recursively traverse the current node's left subtree.
	- R: Recursively traverse the current node's right subtree.

**According to the order in which these operations are performed, we can distiguish three different DFS methods: in-order, pre-order and post-order**.

The traversal trace is a list of each visited node. Given the same three, each traversal method, in general, produces a different trace.

Notice that, by convention, given the root of a tree, we always visit the left subtree before the right subtree. For each traversal method, we can always devise a reversed version, where the right subtree is explored before the left one.

The time complexity for all these procedures is $Θ(n)$, since we must visit each node exactly once. However, the memory complexity changes between BFS and DFS.

#### Pre-order DFS

Preorder traversal visits the node in the order: Root -> Left -> Right . This translates into the sequence of operations NLR.

![f6c979b2bcab30a53b7ae88903f50192.png](../_resources/f6c979b2bcab30a53b7ae88903f50192.png)

Here is a simple C++ program to print each node with pre-order traversal:
```C++
template <typename T>
void preorderTreePrint(const TreeNode<T>* root) {
    if (root) {
        std::cout << root->val << " ";
        inorderTreePrint(root->left);
        inorderTreePrint(root->right);
    }
}
```

#### In-order DFS

In-order traversal visits the node in the order: Left -> Root -> Right. This translates into the sequence of operations LNR.

![9fefb22ff9cbb1985e79f4c08bd29d91.png](../_resources/9fefb22ff9cbb1985e79f4c08bd29d91.png)

Here is a simple C++ program to print each node with in-order traversal:
```C++
template <typename T>
void inorderTreePrint(TreeNode<T>* root) {
    if (root) {
        inorderTreePrint(root->left);
        std::cout << root->val << " ";
        inorderTreePrint(root->right);
    }
}
```

#### Post-order DFS

In-order traversal visits the node in the order: Left -> Right -> Root. This translates into the sequence of operations LRN.

![dc3eee05b1592b741b02ada52f3ce6f0.png](../_resources/dc3eee05b1592b741b02ada52f3ce6f0.png)

Here is a simple C++ program to print each node with post-order traversal:
```C++
template <typename T>
void postorderTreePrint(TreeNode<T>* root) {
    if (root) {
        inorderTreePrint(root->left);
        inorderTreePrint(root->right);
        std::cout << root->val << " ";
    }
}
```

## Operations

Here are some interesting operations that could be performed on a tree.

### Lowest Common Ancestor (LCA)

The lowest common ancestor (LCA) of two nodes $v$ and $w$ in a tree $T$ is the lowest (i.e. deepest) node that has both $v$ and $w$ as descendants, where we define each node to be a descendant of itself (so if $v$ has a direct connection from $w$, $w$ is the lowest common ancestor). 




### Breadth-first search (BFS)

**In breadth-first search (BFS), the search tree is broadened as much as possible before going to the next depth**. It starts at the tree root and explores all nodes at the present depth prior to moving on to the nodes at the next depth level. For this reason, it is also known as Level Order Traversal.

![700eb8e34e6ebfc5e21dbc6bf1bf9fd5.png](../_resources/700eb8e34e6ebfc5e21dbc6bf1bf9fd5.png)

Unlike DFS, BFS uses a queue instead of a stack. This is because, when visiting a node, we need to keep track of its children to visit them later. Since BFS requires a queue, we cannot leverage the function call stack as we did for DFS. This means that this algorithms is typically implemented iteratively by manually managing the queue.

Here is a simple C++ program to print each node with BFS traversal:
```C++
template <typename T>
void bfsTreePrint(TreeNode<T>* root) {
    if (!root) return;
    std::queue<TreeNode<T>*> child_queue;
	  child_queue.push(root);
    while (!child_queue.empty()) {
        TreeNode<T>* node = child_queue.front();
        std::cout << node->val << " ";
        child_queue.pop();
        if (node->left) child_queue.push(node->left);
        if (node->right) child_queue.push(node->right);
    }    
}
```

At one time, the queue will contain all the nodes on one level of the tree. If the tree is perfect, the queue reachest its largest capacity when inpsecting the leaf level, holding $n/2$ elements. This means the memory complexity of this procedure is $O(n)$. The best space complexity is instead achieved in a pathological tree, where the queue only stores one element for each level. Notice this is the exact opposite of DFS. In DFS, when the tree is degenerate, the depth of recursion becomes equal to $n$, which is the worst case. For a complete tree instead, the depth of recursion become $log n$, which is the ideal case.

# Binary search trees

A binary search tree (BST) is a particular type of rooted binary tree. In particular, the keys of a BST are stored in such a way as to satsify the binary search tree property:
1. All nodes of  the left subtree are less than the root node
2. All nodes of the right subtree are more than the root node
3. Both subtrees of each node are also BSTs i.e. they have the above two properties

![b44fe43c88be45693a81a774ea8a9378.png](../_resources/b44fe43c88be45693a81a774ea8a9378.png)

This means BSTs are arranged in a strict total order.

The easiest way to represent a BST is by using a linked data structure. 

