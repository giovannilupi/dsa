#pragma once

#include <unordered_map>
#include <string>

namespace alg {

/**
 * Implementation of the Trie (prefix tree) data structure.
 * For simplicity, this implementation uses raw pointers instead of unique pointers.
 * This is meant to be a simple implementation, which uses no memory saving techniques.
 */
class Trie {
    /** Trie node, contains a map of children and a flag indicating if it is the end of a word. */
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        bool isEndOfWord = false;
    };

    /** Root of the Trie, representing an empty string. */
    TrieNode* root = new TrieNode();

    /** 
     * Helper function for the remove operation.
     * It marks the end of word flag to false for the word to be removed.
     * Then, it recursively removes unused nodes, if any.
     */
    bool removeHelper(TrieNode* node, const std::string& word, int depth) {
        // Reached end of word
        if (depth == word.size()) {
            // Word not found, nothing to delete
            if (!node->isEndOfWord) return false;
            node->isEndOfWord = false;
            // If the node has no children, it can be deleted
            return node->children.empty();
        }
        char c = word[depth];
        if (!node->children.count(c)) return false;
        bool shouldDeleteChild = removeHelper(node->children[c], word, depth + 1);
        if (shouldDeleteChild) {
            delete node->children[c];
            node->children.erase(c);
            // If the node has no children and is not the end of a word, it can be deleted
            return node->children.empty() && !node->isEndOfWord;
        }
        return false;
    }

    /**
     * Deletes the Trie recursively.
     */
    void deleteTrie(TrieNode* node) {
        for (auto& child : node->children) {
            deleteTrie(child.second);
        }
        delete node;
    }
public:

    /**
     * Deletes the Trie.
     */
    ~Trie() {
        deleteTrie(root);    
    }

    /**
     * Inserts a word into the Trie.
     * If the word already exists, no change is made.
     */
    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c)) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    /**
     * Searches for a word in the Trie.
     * Returns true if the word exists, false otherwise.
     */
    bool search(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children.count(c)) return false;
            node = node->children[c];
        }
        return node->isEndOfWord;
    }

    /**
     * Checks if the Trie contains a word that starts with the given prefix.
     * Returns true if the prefix exists, false otherwise.
     */
    bool startsWith(const std::string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children.count(c)) return false;
            node = node->children[c];
        }
        return true;
    }

    /**
     * Removes a word from the Trie.
     * If the word does not exist, no change is made.
     */
    void remove(const std::string& word) {
        removeHelper(root, word, 0);
    }
};

} // namespace alg
