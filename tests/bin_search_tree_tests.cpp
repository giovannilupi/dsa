#include <gtest/gtest.h>
#include <format>
#include <algorithm>
#include <limits>
#include <functional>
#include <memory>
#include "bin_search_tree.hpp"
#include "binary_tree.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::Combine;
using ::testing::ValuesIn;

using InsertFunc = std::function<TreeNode<int>*(TreeNode<int>*, int)>;
using FindFunc = std::function<const TreeNode<int>*(const TreeNode<int>*, int)>;
using RemoveFunc = std::function<TreeNode<int>*(TreeNode<int>*, int)>;

/*
 * This test has multiple fixtures that share the same input containing dynamically allocated memory.
 * Using a per-test-suite teardown to cleanup the memory is not possible.
 * This is because that memory is used by multiple test suites.
 * There is no built-in way to guarantee the cleanup after execution of all test suites.
 * One way to enforce it is to wrap the trees into shared_ptrs to automate deletion.
 * An alternative would be to create and delete the input map in each fixture.
 */
struct TestBSTInput {
    TestBSTInput(TreeNode<int>* root) : 
        root(std::shared_ptr<TreeNode<int>>(root,
            [](TreeNode<int>* root) { deleteTree(root); })) {}
    
    TreeNode<int>* tree() const { return root.get(); }

private:
    std::shared_ptr<TreeNode<int>> root;
};

const std::map<std::string, TestBSTInput> testTrees = {
    {"EmptyBST", nullptr},
    {"OneElementBST", toBinaryTree({12})},
    {"TwoElementsBST", toBinaryTree({20, 10})},
    {"SimpleBST", toBinaryTree({4, 2, 6, 1, 3, 5, 7})},
    {"LargeBST", toBinaryTree({16, 8, 24, 4, 12, 20, 28, 2, 6, 10, 14, 18, 22, 26, 30})},
};

const std::map<std::string, InsertFunc> insertFunctions = {
    {"InsertBSTRecursive", insertBSTRec<int>},
    {"InsertBSTIterative", insertBSTIter<int>},
};

const std::map<std::string, FindFunc> findFunctions = {
    {"FindBSTRecursive", findBSTRec<int>},
    {"FindBSTIterative", findBSTIter<int>},
};

const std::map<std::string, RemoveFunc> removeFunctions = {
    {"RemoveBSTRecursive", removeBSTRec<int>},
    {"RemoveBSTIterative", removeBSTIter<int>},
};

using BSTInsertTestParamT = std::tuple<decltype(insertFunctions)::value_type, decltype(testTrees)::value_type>;

class BSTInsertTest : public TestWithParam<BSTInsertTestParamT> {};

TEST_P(BSTInsertTest, InsertWorks) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& insertFunc = std::get<0>(param).second;
    TreeNode<int>* inputTree = std::get<1>(param).second.tree();

    TreeNode<int>* copiedTree = copyTree(inputTree);
    if (!inputTree) {
        copiedTree = insertFunc(copiedTree, 10);
        copiedTree = insertFunc(copiedTree, 20);
        copiedTree = insertFunc(copiedTree, 0);
        EXPECT_TRUE(checkBST(copiedTree));
    }
    else {
        const std::vector<int> vec = toVector(inputTree);
        // Handle duplicate values
        for (const int val : vec) copiedTree = insertFunc(copiedTree, val);
        EXPECT_TRUE(checkEqualTrees(inputTree, copiedTree));
        // Handle non-duplicate values
        const int max = *std::max_element(vec.cbegin(), vec.cend());
        const int min = *std::min_element(vec.cbegin(), vec.cend());
        insertFunc(copiedTree, max + 1);
        insertFunc(copiedTree, min - 1);
        EXPECT_TRUE(checkBST(copiedTree));
    }
    deleteTree(copiedTree);
}

INSTANTIATE_TEST_SUITE_P(BSTInsertTestsGenerator, BSTInsertTest,
    Combine(ValuesIn(insertFunctions), ValuesIn(testTrees)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using BSTFindTestParamT = std::tuple<decltype(findFunctions)::value_type, decltype(testTrees)::value_type>;

class BSTFindTest : public TestWithParam<BSTFindTestParamT> {};

TEST_P(BSTFindTest, FindWorks) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& insertFunc = std::get<0>(param).second;
    TreeNode<int>* tree = std::get<1>(param).second.tree();

    const std::vector<int> vec = toVector(tree);
    for (const int val : vec) {
        const TreeNode<int>* node = findBSTIter(tree, val);
        EXPECT_TRUE(node);
        EXPECT_EQ(node->val, val);
    }
    const int maxv = vec.empty() ? 0 : *std::max_element(vec.cbegin(), vec.cend());
    EXPECT_FALSE(findBSTIter(tree, maxv + 1));
}

INSTANTIATE_TEST_SUITE_P(BSTFindTestsGenerator, BSTFindTest,
    Combine(ValuesIn(findFunctions), ValuesIn(testTrees)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using BSTRemoveTestParamT = std::tuple<decltype(removeFunctions)::value_type, decltype(testTrees)::value_type>;

class BSTRemoveTest : public TestWithParam<BSTRemoveTestParamT> {};

TEST_P(BSTRemoveTest, RemoveWorks) {
    // Get the parameters for the current test case
    const auto& param = GetParam();
    const auto& removeFunc = std::get<0>(param).second;
    
    TreeNode<int>* tree = copyTree(std::get<1>(param).second.tree());
    if (!tree) {
        tree = removeFunc(tree, 0);
        EXPECT_EQ(tree, nullptr);
        EXPECT_TRUE(checkBST(tree));
    }
    else {
        const auto vec = toVector(tree);
        auto sz = vec.size();
        for (const auto& val : vec) {
            tree = removeFunc(tree, val);
            const auto currvec = toVector(tree);
            EXPECT_EQ(std::find(currvec.cbegin(), currvec.cend(), val), currvec.cend());
            EXPECT_EQ(currvec.size(), --sz);
            EXPECT_TRUE(checkBST(tree));
        }
    }
    deleteTree(tree);
}

INSTANTIATE_TEST_SUITE_P(BSTRemoveTestsGenerator, BSTRemoveTest,
    Combine(ValuesIn(removeFunctions), ValuesIn(testTrees)),
    [](const auto& info) { 
        return std::format("{}_{}", std::get<0>(info.param).first, std::get<1>(info.param).first); 
    });

using BSTTestParamT = decltype(testTrees)::value_type;

class BSTTest : public TestWithParam<BSTTestParamT> {};

TEST_P(BSTTest, GetMinMaxWorks) {
    TreeNode<int>* tree = GetParam().second.tree();
    const TreeNode<int>* min = getBSTMin(tree);
    const TreeNode<int>* max = getBSTMax(tree);
    if (!tree) {
        EXPECT_FALSE(min);
        EXPECT_FALSE(max);
    }
    else {
        EXPECT_TRUE(min);
        EXPECT_TRUE(max);
        const std::vector<int> vec = toVector(tree);
        EXPECT_EQ(min->val, *std::min_element(vec.cbegin(), vec.cend()));
        EXPECT_EQ(max->val, *std::max_element(vec.cbegin(), vec.cend()));
    }
}

TEST_P(BSTTest, GetKthMinWorks) {
    TreeNode<int>* tree = GetParam().second.tree();
    if (!tree) {
        EXPECT_FALSE(getBSTKthMin(tree, 0));
    }
    else {
        std::vector<int> vec = toVector(tree);
        std::ranges::sort(vec);
        auto sz = vec.size();
        const TreeNode<int>* kthMin = getBSTKthMin(tree, sz / 2);
        EXPECT_EQ(kthMin->val, vec[sz / 2]);
    }
}

TEST_P(BSTTest, CheckBSTWorks) {
    const TreeNode<int>* tree = copyTree(GetParam().second.tree());
    EXPECT_TRUE(checkBST(tree));
    if (tree && tree->left) {
        tree->left->val = std::numeric_limits<int>::max();
        EXPECT_FALSE(checkBST(tree));
    }
    deleteTree(tree);
}

INSTANTIATE_TEST_SUITE_P(BSTTestsGenerator, BSTTest,
    ::testing::ValuesIn(testTrees),
    [](const auto& info) { return info.param.first; });

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

TEST(LCATest, LowestCommonAncestorWorks) {
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
    const TreeNode<int>* lca = lcaBST(root, fourNode, eightNode); 
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

}  // namespace
}  // namespace alg
