#include <algorithm>
#include <gtest/gtest.h>
#include <map>
#include <numeric>
#include "binary_tree.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::ValuesIn;

const std::map<std::string, TreeNode<int>*> testTrees = {
    {"EmptyTree", nullptr},
    {"OneElementTree", toBinaryTree({12})},
    {"TwoElementsTree", toBinaryTree({10, 20})},
    {"RepeatedElementsTree", toBinaryTree({7, 7, 7, 7, 7})},
    {"SimpleTree", toBinaryTree({4, 2, 6, 1, 3, 5, 7})},
};

using BinTreeTestParamT = decltype(testTrees)::value_type;

class BinTreeTest : public TestWithParam<BinTreeTestParamT> {
protected:
    /* Per-test-suite teardown. 
     * Called after once after the last test in this test suite. 
     * This is used to cleanup the dynamically allocated trees shared by the parameterized tests.
     */
    static void TearDownTestSuite() {
        for (const auto& [_, tree] : testTrees) {
            deleteTree(tree);
        }
    }
};

TEST_P(BinTreeTest, ApplyWorks) {
    TreeNode<int>* tree = copyTree(GetParam().second);
    auto vec = toVector(tree);
    // Increment all the elements of the tree by 1
    static auto func = [](int& val) { val += 1; };
    // BFS
    bfsTreeApply<int>(tree, func);
    std::transform(vec.begin(), vec.end(), vec.begin(), [](int val) { return val + 1; });
    EXPECT_EQ(toVector(tree), vec);
    // DFS pre-order
    preorderTreeApply<int>(tree, func);
    std::transform(vec.begin(), vec.end(), vec.begin(), [](int val) { return val + 1; });
    EXPECT_EQ(toVector(tree), vec);
    // DFS in-order
    inorderTreeApply<int>(tree, func);
    std::transform(vec.begin(), vec.end(), vec.begin(), [](int val) { return val + 1; });
    EXPECT_EQ(toVector(tree), vec);
    // DFS post-order
    postorderTreeApply<int>(tree, func);
    std::transform(vec.begin(), vec.end(), vec.begin(), [](int val) { return val + 1; });
    EXPECT_EQ(toVector(tree), vec);
    // Delete the copy
    deleteTree(tree);
}

TEST_P(BinTreeTest, CopyWorks) {
    const TreeNode<int>* tree = GetParam().second;
    const TreeNode<int>* copy = copyTree(GetParam().second);
    EXPECT_TRUE(checkEqualTrees(tree, copy));
    deleteTree(copy);
}

TEST_P(BinTreeTest, EqualWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto vec = toVector(tree);
    EXPECT_TRUE(checkEqualTrees(tree, toBinaryTree(vec)));
    vec.push_back(1);
    EXPECT_FALSE(checkEqualTrees(tree, toBinaryTree(vec)));
    EXPECT_TRUE(checkEqualTrees(tree, tree));
}

TEST_P(BinTreeTest, FindWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto vec = toVector(tree);
    for (const auto& val : vec) {
        const auto* node = findTreeNode(tree, val);
        EXPECT_TRUE(node);
        EXPECT_EQ(node->val, val);
    }
    auto maxv = vec.empty() ? 0 : *std::max_element(vec.cbegin(), vec.cend());
    EXPECT_FALSE(findTreeNode(tree, maxv + 1));
}

TEST_P(BinTreeTest, GetMinMaxWorks) {
    TreeNode<int>* tree = GetParam().second;
    const auto* min = findMinTreeNode(tree);
    const auto* max = findMaxTreeNode(tree);
    if (!tree) {
        EXPECT_FALSE(min);
        EXPECT_FALSE(max);
    }
    else {
        EXPECT_TRUE(min);
        EXPECT_TRUE(max);
        const auto vec = toVector(tree);
        EXPECT_EQ(min->val, *std::min_element(vec.cbegin(), vec.cend()));
        EXPECT_EQ(max->val, *std::max_element(vec.cbegin(), vec.cend()));
    }
}

TEST_P(BinTreeTest, AccumulateWorks) {
    TreeNode<int>* tree = GetParam().second;
    const auto sum = accumulateTree(tree);
    if (!tree) {
        EXPECT_EQ(sum, 0);
    }
    else {
        const auto vec = toVector(tree);
        EXPECT_EQ(sum, std::accumulate(vec.cbegin(), vec.cend(), 0));
    }
}

TEST_P(BinTreeTest, SizeWorks) {
    TreeNode<int>* tree = GetParam().second;
    const auto vec = toVector(tree);
    EXPECT_EQ(vec.size(), getTreeSize(tree));
}

INSTANTIATE_TEST_SUITE_P(BinTreeTestsGenerator, BinTreeTest,
    ValuesIn(testTrees),
    [](const auto& info) { return info.param.first; });

namespace {

TreeNode<int>* createSimpleTree() {
    // Construct the following tree:
    //        4
    //       / \
    //      2   6
    //     / \ / \
    //    1  3 5  7
    return new TreeNode<int>{
        .val = 4,
        .left = new TreeNode<int>{
            .val = 2,
            .left = new TreeNode<int>{.val = 1},
            .right = new TreeNode<int>{.val = 3}
        },
        .right = new TreeNode<int>{
            .val = 6,
            .left = new TreeNode<int>{.val = 5},
            .right = new TreeNode<int>{.val = 7}
        }
    };
}

TreeNode<int>* createDegenerateTree() {
    // Construct the following tree:
    //        4
    //       /
    //      2
    //     /
    //    1
    return new TreeNode<int>{
        .val = 4, 
        .left = new TreeNode<int>{
            .val = 2, 
            .left = new TreeNode<int>{
                .val = 1
            }
        }
    };
}

} // namespace

TEST(BinTreeTest, ToBinaryTreeWorks) {
    auto* expected = createSimpleTree();
    auto* root = toBinaryTree({4, 2, 6, 1, 3, 5, 7});
    EXPECT_TRUE(checkEqualTrees(root, expected));
    deleteTree(root);
    deleteTree(expected);
    root = toBinaryTree<int>({});
    expected = nullptr;
    EXPECT_TRUE(checkEqualTrees(root, expected));
}

TEST(BinTreeTest, ToVectorWorks) {
    TreeNode<int>* root = nullptr;
    auto vec = toVector(root);
    EXPECT_TRUE(vec.empty());

    root = createSimpleTree();
    vec = toVector(root);
    EXPECT_EQ(vec, std::vector<int>({4, 2, 6, 1, 3, 5, 7}));
    deleteTree(root);

    root = createDegenerateTree();
    vec = toVector(root);
    EXPECT_EQ(vec, std::vector<int>({4, 2, 1}));
    deleteTree(root);
}

TEST(BinTreeTest, HeightWorks) {
    TreeNode<int>* root = nullptr;
    EXPECT_EQ(getTreeHeight(root), 0);
    root = new TreeNode<int>{.val = 1};
    EXPECT_EQ(getTreeHeight(root), 1);
    delete root;
    root = createSimpleTree();
    EXPECT_EQ(getTreeHeight(root), 3);
    deleteTree(root);
    root = createDegenerateTree();
    EXPECT_EQ(getTreeHeight(root), 3);
    deleteTree(root);
}

TEST(BinTreeTest, MirrorWorks) {
    TreeNode<int>* original = toBinaryTree({1, 2, 3, 4, 5, 6, 7});
    TreeNode<int>* expected = toBinaryTree({1, 3, 2, 7, 6, 5, 4});
    mirrorTree(original);
    EXPECT_TRUE(checkEqualTrees(original, expected));
    deleteTree(original);
    deleteTree(expected);
    // Work with an empty tree
    original = nullptr;
    expected = nullptr;
    mirrorTree(original);
    EXPECT_TRUE(checkEqualTrees(original, expected));
}

TEST(BinTreeTest, MirrorCheckWorks) {
    TreeNode<int>* original = toBinaryTree({1, 2, 3, 4, 5, 6, 7});
    TreeNode<int>* expected = toBinaryTree({1, 3, 2, 7, 6, 5, 4});
    EXPECT_TRUE(checkMirrorTree(original, expected));
    deleteTree(original);
    deleteTree(expected);
    // Work with an empty tree
    original = nullptr;
    expected = nullptr;
    EXPECT_TRUE(checkMirrorTree(original, expected));
    // Work with trees that are not mirrors
    original = toBinaryTree({1, 2, 3, 4, 5, 6, 7});
    expected = toBinaryTree({1, 2, 3, 4, 5, 6, 7});
    EXPECT_FALSE(checkMirrorTree(original, expected));
    deleteTree(original);
    deleteTree(expected);
}

TEST(BinTreeTest, NodeLevelWorks) {
    TreeNode<int>* root = nullptr;
    EXPECT_EQ(getTreeNodeLevel(root, 0), -1);

    root = createSimpleTree();
    EXPECT_EQ(getTreeNodeLevel(root, 4), 0);
    EXPECT_EQ(getTreeNodeLevel(root, 2), 1);
    EXPECT_EQ(getTreeNodeLevel(root, 6), 1);
    EXPECT_EQ(getTreeNodeLevel(root, 1), 2);
    EXPECT_EQ(getTreeNodeLevel(root, 3), 2);
    EXPECT_EQ(getTreeNodeLevel(root, 5), 2);
    EXPECT_EQ(getTreeNodeLevel(root, 7), 2);
    deleteTree(root);

    root = createDegenerateTree();
    EXPECT_EQ(getTreeNodeLevel(root, 4), 0);
    EXPECT_EQ(getTreeNodeLevel(root, 2), 1);
    EXPECT_EQ(getTreeNodeLevel(root, 1), 2);
    deleteTree(root);
}

TEST(BinTreeTest, LowestCommonAncestorWorks) {
    // Construct the following tree
    //        0
    //       / \
    //      1   2
    //     / \
    //    3  4 
    TreeNode<int>* root = new TreeNode<int>{.val = 0};
    TreeNode<int>* node1 = new TreeNode<int>{.val = 1};
    TreeNode<int>* node2 = new TreeNode<int>{.val = 2};
    TreeNode<int>* node3 = new TreeNode<int>{.val = 3};
    TreeNode<int>* node4 = new TreeNode<int>{.val = 4};
    root->left = node1;
    root->right = node2;
    node1->left = node3;
    node1->right = node4;
    EXPECT_EQ(lcaTree(root, node1, node4), node1);
    EXPECT_EQ(lcaTree(root, node3, node4), node1);
    EXPECT_EQ(lcaTree(root, node3, root), root);
    EXPECT_EQ(lcaTree(root, node2, node3), root);
    deleteTree(root);
    // Work with an empty tree
    root = nullptr;
    EXPECT_EQ(lcaTree(root, root, root), nullptr);
}

TEST(BinTreeTest, DiameterWorks) {
    TreeNode<int>* root = nullptr;
    EXPECT_EQ(getTreeDiameter(root), 0);

    root = createSimpleTree();
    EXPECT_EQ(getTreeDiameter(root), 5);
    deleteTree(root);

    root = createDegenerateTree();
    EXPECT_EQ(getTreeDiameter(root), 3);
    deleteTree(root);
}

TEST(BinTreeTest, isHeightBalancedWorks) {
    auto* completeTree = createSimpleTree();
    EXPECT_TRUE(isHeightBalanced(completeTree));
    // Make the tree not balanced
    const auto* newRoot = new TreeNode<int>{.val = 0, .left = completeTree};
    EXPECT_FALSE(isHeightBalanced(newRoot));
    deleteTree(newRoot);
    auto* notCompleteTree = createDegenerateTree();
    EXPECT_FALSE(isHeightBalanced(notCompleteTree));
    // Diff 1 between subtrees
    notCompleteTree->right = new TreeNode<int>{.val = 1};
    EXPECT_TRUE(isHeightBalanced(notCompleteTree));
    deleteTree(notCompleteTree);
}

}  // namespace
}  // namespace alg
