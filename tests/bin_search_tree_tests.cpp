#include <algorithm>
#include <climits>
#include <functional>
#include <gtest/gtest.h>
#include "bin_search_tree.hpp"
#include "binary_tree.hpp"

using namespace alg;
using ::testing::Combine;
using ::testing::ValuesIn;
using InsertFunc = std::function<TreeNode<int>*(TreeNode<int>*, int)>;
using FindFunc = std::function<const TreeNode<int>*(const TreeNode<int>*, int)>;
using RemoveFunc = std::function<TreeNode<int>*(TreeNode<int>*, int)>;

static std::map<std::string, TreeNode<int>*> testTrees = {
    {"EmptyBST", nullptr},
    {"OneElementBST", toBinaryTree({12})},
    {"TwoElementsBST", toBinaryTree({20, 10})},
    {"SimpleBST", toBinaryTree({4, 2, 6, 1, 3, 5, 7})},
    {"LargeBST", toBinaryTree({16, 8, 24, 4, 12, 20, 28, 2, 6, 10, 14, 18, 22, 26, 30})},
};

static std::map<std::string, InsertFunc> insertFunctions = {
    {"InsertBSTRecursive", insertBSTRec<int>},
    {"InsertBSTIterative", insertBSTIter<int>},
};

static std::map<std::string, FindFunc> findFunctions = {
    {"FindBSTRecursive", findBSTRec<int>},
    {"FindBSTIterative", findBSTIter<int>},
};

static std::map<std::string, RemoveFunc> removeFunctions = {
    {"RemoveBSTRecursive", removeBSTRec<int>},
    {"RemoveBSTIterative", removeBSTIter<int>},
};

class BSTInsertTest : 
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, InsertFunc>,
    std::pair<const std::string, TreeNode<int>*>>>
{};

TEST_P(BSTInsertTest, InsertWorks) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto insertFunc = std::get<0>(param).second;
    TreeNode<int>* tree = copyTree(std::get<1>(param).second);
    if (!tree) {
        tree = insertFunc(tree, 10);
        tree = insertFunc(tree, 20);
        tree = insertFunc(tree, 0);
        EXPECT_TRUE(checkBST(tree));
    }
    else {
        auto vec = toVector(tree);
        // Handle duplicate values
        TreeNode<int>* res = copyTree(tree);
        for (const auto& val : vec) res = insertFunc(res, val);
        EXPECT_TRUE(checkEqualTrees(tree, res));
        deleteTree(res);
        // Handle non-duplicate values
        int max = *std::max_element(vec.begin(), vec.end());
        int min = *std::min_element(vec.begin(), vec.end());
        insertFunc(tree, max + 1);
        insertFunc(tree, min - 1);
        EXPECT_TRUE(checkBST(tree));
    }
    deleteTree(tree);
}

INSTANTIATE_TEST_SUITE_P(BSTInsertTestsGenerator, BSTInsertTest,
    Combine(ValuesIn(insertFunctions), ValuesIn(testTrees)),
    [](const testing::TestParamInfo<BSTInsertTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

class BSTFindTest : 
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, FindFunc>,
    std::pair<const std::string, TreeNode<int>*>>>
{};

TEST_P(BSTFindTest, FindWorks) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto insertFunc = std::get<0>(param).second;
    TreeNode<int>* tree = std::get<1>(param).second;
    auto vec = toVector(tree);
    for (const auto& val : vec) {
        auto node = findBSTIter(tree, val);
        EXPECT_TRUE(node);
        EXPECT_EQ(node->val, val);
    }
    auto maxv = vec.empty() ? 0 : *std::max_element(vec.begin(), vec.end());
    EXPECT_FALSE(findBSTIter(tree, maxv + 1));
}

INSTANTIATE_TEST_SUITE_P(BSTFindTestsGenerator, BSTFindTest,
    Combine(ValuesIn(findFunctions), ValuesIn(testTrees)),
    [](const testing::TestParamInfo<BSTFindTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

class BSTRemoveTest : 
    public ::testing::TestWithParam<std::tuple<
    std::pair<const std::string, RemoveFunc>,
    std::pair<const std::string, TreeNode<int>*>>>
{};

TEST_P(BSTRemoveTest, RemoveWorks) {
    // Get the parameters for the current test case
    auto param = GetParam();
    auto removeFunc = std::get<0>(param).second;
    TreeNode<int>* tree = copyTree(std::get<1>(param).second);
    if (!tree) {
        tree = removeFunc(tree, 0);
        EXPECT_EQ(tree, nullptr);
        EXPECT_TRUE(checkBST(tree));
    }
    else {
        auto vec = toVector(tree);
        auto sz = vec.size();
        for (const auto& val : vec) {
            tree = removeFunc(tree, val);
            auto currvec = toVector(tree);
            EXPECT_EQ(std::find(currvec.begin(), currvec.end(), val), currvec.end());
            EXPECT_EQ(currvec.size(), --sz);
            EXPECT_TRUE(checkBST(tree));
        }
    }
    deleteTree(tree);
}

INSTANTIATE_TEST_SUITE_P(BSTRemoveTestsGenerator, BSTRemoveTest,
    Combine(ValuesIn(removeFunctions), ValuesIn(testTrees)),
    [](const testing::TestParamInfo<BSTRemoveTest::ParamType>& info) {
        return std::get<0>(info.param).first + "_" + std::get<1>(info.param).first;
    });

class BSTTest : 
    public ::testing::TestWithParam<std::pair<const std::string, TreeNode<int>*>>
{};

TEST_P(BSTTest, GetMinMaxWorks) {
    TreeNode<int>* tree = GetParam().second;
    auto min = getBSTMin(tree);
    auto max = getBSTMax(tree);
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

TEST_P(BSTTest, GetKthMinWorks) {
    TreeNode<int>* tree = GetParam().second;
    if (!tree) {
        EXPECT_FALSE(getBSTKthMin(tree, 0));
    }
    else {
        auto vec = toVector(tree);
        std::sort(vec.begin(), vec.end());
        auto sz = vec.size();
        auto kthMin = getBSTKthMin(tree, sz / 2);
        EXPECT_EQ(kthMin->val, vec[sz / 2]);
    }
}

TEST_P(BSTTest, CheckBSTWorks) {
    TreeNode<int>* tree = copyTree(GetParam().second);
    EXPECT_TRUE(checkBST(tree));
    if (tree && tree->left) {
        tree->left->val = INT_MAX;
        EXPECT_FALSE(checkBST(tree));
    }
    deleteTree(tree);
}

INSTANTIATE_TEST_SUITE_P(BSTTestsGenerator, BSTTest,
    ::testing::ValuesIn(testTrees),
    [](const testing::TestParamInfo<BSTTest::ParamType>& info) {
        return info.param.first;
    });

TEST(BSTTest, ToBSTWorks) {
    auto tree = toBinaryTree({4, 6, 2, 7, 3, 5, 1});
    auto expected = toBinaryTree({4, 2, 6, 1, 3, 5, 7});
    toBST(tree);
    EXPECT_TRUE(checkEqualTrees(tree, expected));
    EXPECT_TRUE(checkBST(tree));
    deleteTree(tree);
    deleteTree(expected);
    // Work with an empty tree
    tree = nullptr;
    expected = nullptr;
    toBST(tree);
    EXPECT_TRUE(checkEqualTrees(tree, expected));
    EXPECT_TRUE(checkBST(tree));
}

TEST(LCATest, FindLowestCommonAncestorInBST) {
    // Tree structure:
    //        6
    //       / \
    //      4   8
    //     / \   \
    //    3   5   9
    // Create a binary search tree manually
    TreeNode<int>* root = new TreeNode<int>(6);
    TreeNode<int>* fourNode = root->left = new TreeNode<int>(4);
    TreeNode<int>* eightNode = root->right = new TreeNode<int>(8);
    TreeNode<int>* threeNode = fourNode->left = new TreeNode<int>(3);
    TreeNode<int>* fiveNode = fourNode->right = new TreeNode<int>(5);
    TreeNode<int>* nineNode = eightNode->right = new TreeNode<int>(9);

    // LCA of 4 and 8 is 6
    auto lca = lcaBST(root, fourNode, eightNode); 
    EXPECT_EQ(lca->val, 6);
    // LCA of 3 and 5 is 4
    lca = lcaBST(root, root->left->left, root->left->right); 
    EXPECT_EQ(lca->val, 4); 
    // LCA of 4 and 5 is 4
    lca = lcaBST(root, root->left, root->left->right); 
    EXPECT_EQ(lca->val, 4);
    // LCA of 8 and 9 is 8
    lca = lcaBST(root, root->right, root->right->right); 
    EXPECT_EQ(lca->val, 8);

    // Clean up
    deleteTree(root);
}
