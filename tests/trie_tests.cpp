#include <gtest/gtest.h>
#include <vector>
#include "trie.hpp"

using namespace alg;

class TrieTest : public ::testing::Test {
protected:
    Trie trie;
    inline static const std::vector<std::string> words = 
        {"apple", "app", "banana", "bat"};

    void SetUp() override {
        for (const auto& word : words) 
            trie.insert(word);
    }
};

TEST_F(TrieTest, InsertAndSearchWork) {
    // Should exist
    EXPECT_TRUE(trie.search("apple"));
    EXPECT_TRUE(trie.search("app"));
    EXPECT_TRUE(trie.search("banana"));

    // Should not exist
    EXPECT_FALSE(trie.search("appl"));
    EXPECT_FALSE(trie.search("ban"));
}

TEST_F(TrieTest, StartsWithWorks) {
    EXPECT_TRUE(trie.startsWith("app"));
    EXPECT_TRUE(trie.startsWith("bana"));
    // Non-existent prefixes
    EXPECT_FALSE(trie.startsWith("car")); 
    EXPECT_FALSE(trie.startsWith("batb"));
}

TEST_F(TrieTest, RemoveWorks) {
    // Remove "apple" but keep "app"
    EXPECT_TRUE(trie.search("apple"));
    trie.remove("apple");
    EXPECT_FALSE(trie.search("apple"));
    EXPECT_FALSE(trie.startsWith("apple"));
    EXPECT_TRUE(trie.search("app"));
    EXPECT_TRUE(trie.startsWith("app"));

    // Remove "banana" completely
    EXPECT_TRUE(trie.search("banana"));
    trie.remove("banana");
    EXPECT_FALSE(trie.search("banana"));
    EXPECT_FALSE(trie.startsWith("banana"));
    EXPECT_TRUE(trie.search("bat"));
    EXPECT_TRUE(trie.startsWith("bat"));

    // Remove non-existent word
    trie.remove("abc");
    EXPECT_FALSE(trie.search("abc"));
    EXPECT_FALSE(trie.startsWith("abc"));
}
