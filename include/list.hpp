#pragma once

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
 * Detects the presence of a cycle in a list.
 * This uses the Floyd's cycle detection algorithm.
 * The advantage of this algorithm is that it keep a linear time complexity,
 * but uses O(1) memory.
 */
template <typename T>
bool checkCycle(SListNode<T>* head) {
    // We can use head directly as slow pointer
    SListNode<T>* fast = head;
    while(fast && fast->next) {
        head = head->next;
        fast = fast->next->next;
        if (head == fast) return true;
    }
    return false;
}

/**
 * Detects the origin node of a cycle in a list.
 * This is a more involved version of the Floyd's cycle detection algorithm.
 * Once a loop is detected, it moves the slow and fast pointer one move at a time.
 * Eventually, the two pointers will meet at the origin of the loop.
 */ 
template <typename T>
SListNode<T>* findCycle(SListNode<T>* head) {
    SListNode<T>* slow = head; 
    SListNode<T>* fast = head;
    // Check if the list has a loop
    while(fast && fast->next) {
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

}
