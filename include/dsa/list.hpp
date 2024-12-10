#pragma once

#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include "alg_concepts.hpp"
#include "common.hpp"

namespace alg {

/**
 * Node of a singly linked list with value of generic type.
 */ 
template <typename T>
struct SListNode {
    T val;
    SListNode* next;
    SListNode(T val) : val(val), next(nullptr) {}
    SListNode(T val, SListNode* next) : val(val), next(next) {}
};

/**
 * Reverses a singly linked list.
 */
template <typename T>
SListNode<T>* reverseList(SListNode<T>* head) {
    SListNode<T>* prev = nullptr;
    while (head) {
        SListNode<T>* next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    return prev;
}

/**
 * Deep copies a singly linked list.
 */
template <typename T>
SListNode<T>* copyList(const SListNode<T>* head) {
    SListNode<T>* newHead = nullptr;
    SListNode<T>* newTail = nullptr;
    while (head) {
        SListNode<T>* newNode = new SListNode<T>(head->val);
        if (!newHead) newHead = newNode;
        else newTail->next = newNode;
        newTail = newNode;
        head = head->next;
    }
    return newHead;
}

/**
 * Returns the middle node of a singly linked list.
 */
template <typename T>
SListNode<T>* getMiddleList(SListNode<T>* head) {
    const SListNode<T>* fast = head;
    while (fast && fast->next) { 
        head = head->next; 
        fast = fast->next->next; 
    }
    return head;
}

/**
 * Returns the length of a singly linked list.
 * Complexity: O(n)
 */
template <typename T>
std::size_t getListLen(const SListNode<T>* head) {
    std::size_t len = 0;
    while (head) { ++len; head = head->next; }
    return len;
}

/**
 * Prints a singly linked list.
 */
template <PrintableElement T>
void printList(SListNode<T>* head) {
    while (head) {
        std::cout << head->val;
        head = head->next;
        if (head) std::cout << " ";
    }
    std::cout << std::endl;
}

/**
 * Inserts an element into a singly linked list at a given position.
 * If the position is out of bounds, the element is inserted at the end.
 */
template <typename T>
SListNode<T>* insertList(SListNode<T>* head, std::size_t pos, const T& val) {
    SListNode<T>* prev = nullptr;
    SListNode<T>* curr = head;
    index cpos = pos;
    // Traverse to the position
    while (cpos-- && curr) {
        prev = curr;
        curr = curr->next;
    }
    // Insert the element
    SListNode<T>* new_node = new SListNode<T>(val, curr);
    // Handle empty list
    if (!prev) return new_node;
    prev->next = new_node;
    return head;
}

/**
 * Prepends an element to the beginning of a singly linked list.
 */
template <typename T>
SListNode<T>* insertListFront(SListNode<T>* head, const T& val) {
    return insertList(head, 0, val);
}

/**
 * Appends an element to the end of a singly linked list.
 */
template <typename T>
SListNode<T>* insertListBack(SListNode<T>* head, const T& val) {
    return insertList(head, getListLen(head), val);
}

/**
 * Removes an element from a singly linked list at a given position.
 * If the position is out of bounds, no element is removed.
 */
template <typename T>
SListNode<T>* removeList(SListNode<T>* head, std::size_t pos) {
    SListNode<T>* curr = head;
    SListNode<T>* prev = nullptr;
    index cpos = pos;
    while (cpos-- && curr) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) return head;
    SListNode<T>* nxt = curr->next;
    delete(curr);
    if (!prev) return nxt;
    prev->next = nxt;
    return head;
}

/**
 * Removes an element from the beginning of a singly linked list.
 */
template <typename T>
SListNode<T>* removeListFront(SListNode<T>* head) {
    return removeList(head, 0);
}

/**
 * Removes an element from the end of a singly linked list.
 */
template <typename T>
SListNode<T>* removeListBack(SListNode<T>* head) {
    std::size_t len = getListLen(head);
    return len ? removeList(head, len - 1) : head;
}

/**
 * Finds the first occurrence of an element in a singly linked list.
 * If the element is not found, returns nullptr.
 */
template <std::equality_comparable T>
SListNode<T>* findList(SListNode<T>* head, const T& val) {
    while (head) {
        if (head->val == val) return head;
        head = head->next;
    }
    return nullptr;
}

/**
 * Converts a singly linked list to a vector.
 * Useful for testing.
 */
template <typename T>
std::vector<T> listToVector(const SListNode<T>* head) {
    std::vector<T> vec;
    while (head) {
        vec.push_back(head->val);
        head = head->next;
    }
    return vec;
}

/**
 * Converts an iterator range to a singly linked list.
 */
template <typename T>
SListNode<typename std::iterator_traits<T>::value_type>* toList(T first, T last) {
    SListNode<typename std::iterator_traits<T>::value_type>* head = nullptr;
    size_t cnt = 0;
    while (first != last) head = insertList(head, cnt++, *first++);
    return head;
}

/**
 * Converts an initializer list to a singly linked list.
 */
template <typename T>
SListNode<T>* toList(std::initializer_list<T> initList) {
    SListNode<T>* head = nullptr;
    SListNode<T>* tail = nullptr;
    for (const auto &val : initList) {
        SListNode<T>* new_node = new SListNode<T>(val);
        if (!head) head = new_node;
        if (tail) tail->next = new_node;
        tail = new_node;
    }
    return head;
}

/**
 * Converts an initializer list to a singly linked list.
 */
template <typename T>
SListNode<T>* genList(std::initializer_list<T> initList) {
    return toList(initList.begin(), initList.end());
}

/**
 * Deletes a singly linked list.
 * If the list has a cycle, the cycle is broken before deleting the list.
 */
template <typename T>
void deleteList(SListNode<T>* head) {
    SListNode<T>* cycleStart = findCycle(head);
    // Break the cycle before deleting the list
    if (cycleStart) {
        SListNode<T>* curr = cycleStart;
        while (curr->next != cycleStart) curr = curr->next;
        curr->next = nullptr;  
    }
    // Delete the list
    while (head) {
        SListNode<T>* nxt = head->next;
        delete(head);
        head = nxt;
    }
}

/**
 * Detects the presence of a cycle in a list.
 * This uses the Floyd's cycle detection algorithm.
 * It works by moving a slow pointer one node at a time, and a fast pointer two nodes at a time.
 * If the two pointers meet before the end of the list, there is a cycle.
 * The advantage of this algorithm is that it keep a linear time complexity,
 * but uses O(1) memory.
 */
template <typename T>
bool hasCycle(const SListNode<T>* head) {
    // We can use head directly as slow pointer
    const SListNode<T>* fast = head;
    while (fast && fast->next) {
        head = head->next;
        fast = fast->next->next;
        if (head == fast) return true;
    }
    return false;
}

/**
 * Detects the origin node of a cycle in a list.
 * This is a more involved version of the Floyd's cycle detection algorithm.
 * Once a loop is detected, it resets the slow pointer to the head of the list.
 * The slow and fast pointers are then moved one node at a time.
 * Eventually, the pointers will meet at the origin of the cycle.
 */ 
template <typename T>
SListNode<T>* findCycle(SListNode<T>* head) {
    if (!head || !head->next) return nullptr;
    SListNode<T>* slow = head; 
    SListNode<T>* fast = head;
    // Check if the list has a loop
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) break;
    }
    // We didn't break out of the previous loop, so there is no cycle
    if (slow != fast) return nullptr;
    // Reset slow pointer and traverse the list again
    slow = head;
    // Find the origin of the cycle
    while(slow != fast) { slow = slow->next; fast = fast->next; }
    return slow;
}

} // namespace alg
