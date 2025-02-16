#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <vector>
#include "list.hpp"

using alg::SListNode;
using ::testing::TestWithParam;
using ::testing::ElementsAre;
using ::testing::ValuesIn;

namespace {

const std::map<std::string, SListNode<int>*> testParams = {
    {"EmptyList", nullptr},
    {"SimpleList", alg::toList({1, 2, 3, 4, 5})},
    {"SingleElementList", alg::toList({42})},
    {"TwoElementsList", alg::toList({10, 20})},
    {"RepeatedElementsList", alg::toList({7, 7, 7, 7, 7})},
    {"NegativeAndPositiveList", alg::toList({-5, -1, 0, 1, 5})},
    {"DescendingList", alg::toList({9, 7, 5, 3, 1})},
    {"AscendingList", alg::toList({1, 2, 3, 4, 5})},
    {"AlternateSignList", alg::toList({10, -20, 30, -40, 50})},
};

} // namespace

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
    std::reverse(vec.begin(), vec.end());
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
        for (const auto& [_, list] : testLists) {
            deleteList(list);
        }
    }

    std::unordered_map<std::string, SListNode<int>*> testLists = {
        {"EmptyList", nullptr},
        {"SingleElementList", alg::toList({1})},
        {"SimpleList", alg::toList({1, 2, 3, 4, 5})},
        {"ListWithLoop", createCyclicList()}
    };

private:
    /* List: 1 -> 2 -> 3 -> 4 -> 5 -> 3 */
    SListNode<int>* createCyclicList() {
        SListNode<int>* head = new SListNode(1);
        SListNode<int>* tail = head;
        tail->next = new SListNode(2);
        tail = tail->next;
        SListNode<int>* cycleEntry = tail->next = new SListNode(3);
        tail = tail->next;
        tail->next = new SListNode(4);
        tail = tail->next;
        tail->next = new SListNode(5);
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
    auto* list = alg::toList<int>({});
    EXPECT_THAT(listToVector(list), ElementsAre());
    deleteList(list);

    list = alg::toList({1});
    EXPECT_THAT(listToVector(list), ElementsAre(1));
    deleteList(list);

    list = alg::toList({1, 2, 3, 4, 5});
    EXPECT_THAT(listToVector(list), ElementsAre(1, 2, 3, 4, 5));
    deleteList(list);
}
