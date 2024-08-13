#include <algorithm>
#include <gtest/gtest.h>
#include <map>
#include <numeric>
#include "bin_search_tree.hpp"
#include "binary_tree.hpp"

using namespace alg;

static std::map<std::string, TreeNode<int>*> testTrees = {
    {"EmptyTree", nullptr},
    {"OneElementTree", toBinaryTree({12})},
    {"TwoElementsTree", toBinaryTree({10, 20})},
    {"RepeatedElementsTree", toBinaryTree({7, 7, 7, 7, 7})},
    {"SimpleTree", toBinaryTree({4, 2, 6, 1, 3, 5, 7})},
};

class BinTreeTest : 
    public ::testing::TestWithParam<std::pair<const std::string, TreeNode<int>*>>
{};

TEST_P(BinTreeTest, ApplyWorks) {
    TreeNode<int>* tree = copyTree(GetParam().second);
    auto vec = toVector(tree);
    //Increment all the elements of the tree by 1
    auto func = [](int& val) { val += 1; };
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
    TreeNode<int>* tree = GetParam().second;
    TreeNode<int>* copy = copyTree(GetParam().second);
    EXPECT_TRUE(checkEqualTrees(tree, copy));
    deleteTree(copy);
}

TEST_P(BinTreeTest, EqualWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto vec = toVector(tree);
    vec.push_back(1);
    EXPECT_FALSE(checkEqualTrees(tree, toBinaryTree(vec)));
    EXPECT_TRUE(checkEqualTrees(tree, tree));
}

TEST_P(BinTreeTest, FindWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto vec = toVector(tree);
    for (const auto& val : vec) {
        auto node = findTreeNode(tree, val);
        EXPECT_TRUE(node);
        EXPECT_EQ(node->val, val);
    }
    auto maxv = vec.empty() ? 0 : *std::max_element(vec.begin(), vec.end());
    EXPECT_FALSE(findTreeNode(tree, maxv + 1));
}

TEST_P(BinTreeTest, GetMinMaxWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto min = findMinTreeNode(tree);
    auto max = findMaxTreeNode(tree);
    if (!tree) {
        EXPECT_FALSE(min);
        EXPECT_FALSE(max);
    }
    else {
        auto vec = toVector(tree);
        EXPECT_EQ(min->val, *std::min_element(vec.begin(), vec.end()));
        EXPECT_EQ(max->val, *std::max_element(vec.begin(), vec.end()));
    }
}

TEST_P(BinTreeTest, AccumulateWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto sum = accumulateTree(tree);
    if (!tree) {
        EXPECT_EQ(sum, 0);
    }
    else {
        auto vec = toVector(tree);
        EXPECT_EQ(sum, std::accumulate(vec.begin(), vec.end(), 0));
    }
}

TEST_P(BinTreeTest, SizeWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto vec = toVector(tree);
    EXPECT_EQ(vec.size(), getTreeSize(tree));
}

INSTANTIATE_TEST_SUITE_P(BinTreeTestsGenerator, BinTreeTest,
    ::testing::ValuesIn(testTrees),
    [](const testing::TestParamInfo<BinTreeTest::ParamType>& info) {
        return info.param.first;
    });

static TreeNode<int>* createSimpleTree() {
    // Construct the following tree:
    //        4
    //       / \
    //      2   6
    //     / \ / \
    //    1  3 5  7
    return new TreeNode<int>(4, 
            new TreeNode<int>(2, new TreeNode<int>(1), new TreeNode<int>(3)),
            new TreeNode<int>(6, new TreeNode<int>(5), new TreeNode<int>(7)));
}

static TreeNode<int>* createDegenerateTree() {
    // Construct the following tree:
    //        4
    //       /
    //      2
    //     /
    //    1
    return new TreeNode<int>(4, new TreeNode<int>(2, new TreeNode<int>(1), nullptr), nullptr);
}

TEST(BinTreeTest, ToBinaryTreeWorks) {
    auto expected = createSimpleTree();
    auto root = toBinaryTree({4, 2, 6, 1, 3, 5, 7});
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
    root = new TreeNode<int>(1);
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

TEST(BinTreeTest, LeastCommonAncestorWorks) {
    // Construct the following tree
    //        0
    //       / \
    //      1   2
    //     / \
    //    3  4 
    TreeNode<int>* root = new TreeNode<int>(0);
    TreeNode<int>* node1 = new TreeNode<int>(1);
    TreeNode<int>* node2 = new TreeNode<int>(2);
    TreeNode<int>* node3 = new TreeNode<int>(3);
    TreeNode<int>* node4 = new TreeNode<int>(4);
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