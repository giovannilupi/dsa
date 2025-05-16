#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <string_view>
#include <ranges>
#include <unordered_map>
#include <map>
#include <vector>
#include "list.hpp"

namespace alg {
namespace {

using ::testing::TestWithParam;
using ::testing::ElementsAre;
using ::testing::ValuesIn;

const std::map<std::string, SListNode<int>*> testParams = {
    {"EmptyList", nullptr},
    {"SimpleList", toList({1, 2, 3, 4, 5})},
    {"SingleElementList", toList({42})},
    {"TwoElementsList", toList({10, 20})},
    {"RepeatedElementsList", toList({7, 7, 7, 7, 7})},
    {"NegativeAndPositiveList", toList({-5, -1, 0, 1, 5})},
    {"DescendingList", toList({9, 7, 5, 3, 1})},
    {"AscendingList", toList({1, 2, 3, 4, 5})},
    {"AlternateSignList", toList({10, -20, 30, -40, 50})},
};

using ListTestParamT = decltype(testParams)::value_type;

class ListTestValParam : public TestWithParam<ListTestParamT> {
protected:
    /* Per-test-suite teardown. 
     * Called after once after the last test in this test suite. 
     * This is used to cleanup the dynamically allocated lists shared by the parameterized tests.
     */
    static void TearDownTestSuite() {
        for (const auto& [_, list] : testParams) {
            deleteList(list);
        }
    }
};

TEST_P(ListTestValParam, CopyList) {
    const auto& list = GetParam().second;
    const auto vec = listToVector(list);
    const auto copy = copyList(list);
    EXPECT_EQ(listToVector(copy), vec);
}

TEST_P(ListTestValParam, ReverseList) {
    const auto& param = GetParam().second;
    auto list = copyList(param);
    auto vec = listToVector(list);
    std::ranges::reverse(vec);
    list = reverseList(list);
    EXPECT_EQ(listToVector(list), vec);
    deleteList(list);
}

TEST_P(ListTestValParam, MiddleListElement) {
    const auto& list = GetParam().second;
    if (list) {
        const auto& vec = listToVector(list);
        const auto& expected = vec[vec.size() / 2];
        EXPECT_EQ(getMiddleList(list)->val, expected);
    }
    else 
        EXPECT_EQ(getMiddleList(list), nullptr);
}

TEST_P(ListTestValParam, ListLength) {
    const auto& list = GetParam().second;
    const auto expected = listToVector(list).size();
    EXPECT_EQ(getListLen(list), expected);
}

TEST_P(ListTestValParam, FindListElement) {
    const auto& list = GetParam().second;
    if (list) {
        const auto vec = listToVector(list);
        const auto& expected = vec[vec.size() / 2];
        const auto* found = findList(list, expected);
        ASSERT_TRUE(found);
        EXPECT_EQ(found->val, expected);
    }
    else {
        EXPECT_EQ(findList(list, 5), nullptr);
    }
}

TEST_P(ListTestValParam, InsertListElement) {
    // Get the parameters for the current test case
    const auto& param = GetParam().second;
    auto list = copyList(param);
    auto vec = listToVector(list);

    // Insert an element at the beginning
    list = insertListFront(list, 42);
    vec.insert(vec.begin(), 42);
    EXPECT_EQ(listToVector(list), vec);

    // Insert an element at the end
    list = insertListBack(list, 20);
    vec.push_back(20);
    EXPECT_EQ(listToVector(list), vec);

    // Insert an element in the middle
    const auto mid = vec.size() / 2;
    list = insertList(list, mid, 10);
    vec.insert(vec.begin() + mid, 10);
    EXPECT_EQ(listToVector(list), vec);

    // Cleanup
    deleteList(list);
}

TEST_P(ListTestValParam, RemoveListElement) {
    // Get the parameters for the current test case
    const auto& param = GetParam().second;
    auto list = copyList(param);
    auto vec = listToVector(list);

    // Remove an element at the beginning
    list = removeListFront(list);
    if (!vec.empty()) vec.erase(vec.begin());
    EXPECT_EQ(listToVector(list), vec);

    // Remove an element at the end
    list = removeListBack(list);
    if (!vec.empty()) vec.pop_back();
    EXPECT_EQ(listToVector(list), vec);

    // Remove an element in the middle
    const auto mid = vec.size() / 2;
    list = removeList(list, mid);
    if (!vec.empty()) vec.erase(vec.begin() + mid);
    EXPECT_EQ(listToVector(list), vec);

    // Cleanup
    deleteList(list);
}

INSTANTIATE_TEST_SUITE_P(ListTestsGenerator, ListTestValParam,
    ValuesIn(testParams),
    [](const auto& info) { return info.param.first; });

class ListTestFixture : public ::testing::Test {
protected:

    void TearDown() override {
        for (SListNode<int>* list : std::views::values(testLists)) {
            deleteList(list);
        }
    }

    std::unordered_map<std::string_view, SListNode<int>*> testLists = {
        {"EmptyList", nullptr},
        {"SingleElementList", toList({1})},
        {"SimpleList", toList({1, 2, 3, 4, 5})},
        {"ListWithLoop", createCyclicList()}
    };

private:
    /* List: 1 -> 2 -> 3 -> 4 -> 5 -> 3 */
    SListNode<int>* createCyclicList() {
        SListNode<int>* head = new SListNode<int>{.val = 1};
        SListNode<int>* tail = head;
        tail->next = new SListNode<int>{.val = 2};
        tail = tail->next;
        SListNode<int>* cycleEntry = tail->next = new SListNode<int>{.val = 3};
        tail = tail->next;
        tail->next = new SListNode<int>{.val = 4};
        tail = tail->next;
        tail->next = new SListNode<int>{.val = 5};
        tail = tail->next;
        tail->next = cycleEntry;
        return head;
    }
};

TEST_F(ListTestFixture, HasCycle) {
    EXPECT_FALSE(hasCycle(testLists["EmptyList"]));
    EXPECT_FALSE(hasCycle(testLists["SingleElementList"]));
    EXPECT_FALSE(hasCycle(testLists["SimpleList"]));
    EXPECT_TRUE(hasCycle(testLists["ListWithLoop"]));
}

TEST_F(ListTestFixture, FindCycle) {
    EXPECT_EQ(findCycle(testLists["EmptyList"]), nullptr);
    EXPECT_EQ(findCycle(testLists["SingleElementList"]), nullptr);
    EXPECT_EQ(findCycle(testLists["SimpleList"]), nullptr);
    EXPECT_EQ(findCycle(testLists["ListWithLoop"]), testLists["ListWithLoop"]->next->next);
}

TEST_F(ListTestFixture, ToVector) {
    EXPECT_THAT(listToVector(testLists["EmptyList"]), ElementsAre());
    EXPECT_THAT(listToVector(testLists["SingleElementList"]), ElementsAre(1));
    EXPECT_THAT(listToVector(testLists["SimpleList"]), ElementsAre(1, 2, 3, 4, 5));
}

TEST_F(ListTestFixture, ToList) {
    SListNode<int>* emptyList = toList<int>({});
    EXPECT_THAT(listToVector(emptyList), ElementsAre());
    deleteList(emptyList);

    SListNode<int>* singleList = toList({1});
    EXPECT_THAT(listToVector(singleList), ElementsAre(1));
    deleteList(singleList);

    SListNode<int>* longList = toList({1, 2, 3, 4, 5});
    EXPECT_THAT(listToVector(longList), ElementsAre(1, 2, 3, 4, 5));
    deleteList(longList);
}

}  // namespace
}  // namespace alg
