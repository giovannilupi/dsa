#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "list.hpp"

using namespace alg;

static std::map<std::string, SListNode<int>*> testLists = {
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

class ListTestParam : 
    public ::testing::TestWithParam<std::pair<const std::string, SListNode<int>*>>  
{};

class ListTestFixture : public ::testing::Test {
protected:

    void TearDown() override {
        for (auto& list : testLists) {
            deleteList(list.second);
        }
    }

    std::map<std::string, SListNode<int>*> testLists = {
        {"EmptyList", nullptr},
        {"SingleElementList", toList({1})},
        {"SimpleList", toList({1, 2, 3, 4, 5})},
        {"ListWithLoop", createCyclicList()}
    };

private:
    // List: 1 -> 2 -> 3 -> 4 -> 5 -> 3
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

TEST_P(ListTestParam, CopyList) {
    const auto& list = GetParam().second;
    auto vec = listToVector(list);
    auto copy = copyList(list);
    EXPECT_EQ(listToVector(copy), vec);
}

TEST_P(ListTestParam, ReverseList) {
    const auto& param = GetParam().second;
    auto list = copyList(param);
    auto vec = listToVector(list);
    std::reverse(vec.begin(), vec.end());
    list = reverseList(list);
    EXPECT_EQ(listToVector(list), vec);
}

TEST_P(ListTestParam, MiddleListElement) {
    const auto& list = GetParam().second;
    if (list) {
        auto vec = listToVector(list);
        auto expected = vec[vec.size() / 2];
        EXPECT_EQ(getMiddleList(list)->val, expected);
    }
    else 
        EXPECT_EQ(getMiddleList(list), nullptr);
}

TEST_P(ListTestParam, ListLength) {
    const auto& list = GetParam().second;
    auto vec = listToVector(list);
    auto expected = vec.size();
    EXPECT_EQ(getListLen(list), expected);
}

TEST_P(ListTestParam, FindListElement) {
    const auto& list = GetParam().second;
    if (list) {
        auto vec = listToVector(list);
        auto expected = vec[vec.size() / 2];
        auto found = findList(list, expected);
        ASSERT_TRUE(found);
        EXPECT_EQ(found->val, expected);
    } 
    else 
        EXPECT_EQ(findList(list, 5), nullptr);
}

TEST_P(ListTestParam, InsertListElement) {
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
    auto mid = vec.size() / 2;
    list = insertList(list, mid, 10);
    vec.insert(vec.begin() + mid, 10);
    EXPECT_EQ(listToVector(list), vec);
}

TEST_P(ListTestParam, RemoveListElement) {
    const auto& param = GetParam().second;
    auto list = copyList(param);
    auto vec = listToVector(list);
    // Remove an element at the beginning
    list = removeListFront(list);
    if (vec.size()) vec.erase(vec.begin());
    EXPECT_EQ(listToVector(list), vec);
    // Remove an element at the end
    list = removeListBack(list);
    if (vec.size()) vec.pop_back();
    EXPECT_EQ(listToVector(list), vec);
    // Remove an element in the middle
    auto mid = vec.size() / 2;
    list = removeList(list, mid);
    if (vec.size()) vec.erase(vec.begin() + mid);
    EXPECT_EQ(listToVector(list), vec);
}

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
    EXPECT_EQ(listToVector(testLists["EmptyList"]), std::vector<int>());
    EXPECT_EQ(listToVector(testLists["SingleElementList"]), std::vector<int>({1}));
    EXPECT_EQ(listToVector(testLists["SimpleList"]), std::vector<int>({1, 2, 3, 4, 5}));
}

TEST_F(ListTestFixture, ToList) {
    auto vec = toList<int>({});
    EXPECT_EQ(listToVector(vec), std::vector<int>());
    vec = toList({1});
    EXPECT_EQ(listToVector(vec), std::vector<int>({1}));
    vec = toList({1, 2, 3, 4, 5});
    EXPECT_EQ(listToVector(vec), std::vector<int>({1, 2, 3, 4, 5}));
}

INSTANTIATE_TEST_SUITE_P(ListTestsGenerator, ListTestParam,
    ::testing::ValuesIn(testLists),
    [](const testing::TestParamInfo<ListTestParam::ParamType>& info) {
        return info.param.first;
    });
