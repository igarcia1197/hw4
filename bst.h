#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>
#include <cmath>
#include <algorithm>
#include <functional>

/**
 * A templated class for a Node in a search tree.
 * The getters for parent/left/right are virtual so
 * that they can be overridden for future kinds of
 * search trees, such as Red Black trees, Splay trees,
 * and AVL trees.
 */
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<const Key, Value>& getItem() const;
    std::pair<const Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<const Key, Value> item_;
    Node<Key, Value>* parent_;
    Node<Key, Value>* left_;
    Node<Key, Value>* right_;
};

/*
  -----------------------------------------
  Begin implementations for the Node class.
  -----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent) :
    item_(key, value),
    parent_(parent),
    left_(NULL),
    right_(NULL)
{
}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed by the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{
}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<const Key, Value>& Node<Key, Value>::getItem() const
{
    return item_;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<const Key, Value>& Node<Key, Value>::getItem()
{
    return item_;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return item_.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return item_.second;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return item_.second;
}

/**
* An implementation of the virtual function for retrieving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return parent_;
}

/**
* An implementation of the virtual function for retrieving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return left_;
}

/**
* An implementation of the virtual function for retrieving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return right_;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    parent_ = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    left_ = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    right_ = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    item_.second = value;
}

/*
  ---------------------------------------
  End implementations for the Node class.
  ---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree(); //TODO
    virtual ~BinarySearchTree(); //TODO
    virtual void insert(const std::pair<const Key, Value>& keyValuePair); //TODO
    virtual void remove(const Key& key); //TODO
    void clear(); //TODO
    bool isBalanced() const; //TODO
    void print() const;
    bool empty() const;

    template<typename PPKey, typename PPValue>
    friend void prettyPrintBST(BinarySearchTree<PPKey, PPValue> & tree);
public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator  // TODO
    {
    public:
        iterator();

        std::pair<const Key,Value>& operator*() const;
        std::pair<const Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        iterator& operator++();

    protected:
        friend class BinarySearchTree<Key, Value>;
        iterator(Node<Key,Value>* ptr);
        Node<Key, Value> *current_;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;
    Value& operator[](const Key& key);
    Value const & operator[](const Key& key) const;

protected:
    // Mandatory helper functions
    Node<Key, Value>* internalFind(const Key& k) const; // TODO
    Node<Key, Value> *getSmallestNode() const;  // TODO
    static Node<Key, Value>* predecessor(Node<Key, Value>* current); // TODO
    // Provided helper functions
    virtual void printRoot (Node<Key, Value> *r) const;
    virtual void nodeSwap( Node<Key,Value>* n1, Node<Key,Value>* n2);

protected:
    Node<Key, Value>* root_;
};

/*
--------------------------------------------------------------
Begin implementations for the BinarySearchTree::iterator class.
---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
{
    current_ = ptr;
}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::iterator::iterator() 
{
    current_ = NULL;
}

/**
* Provides access to the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> &
BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return current_->getItem();
}

/**
* Provides access to the address of the item.
*/
template<class Key, class Value>
std::pair<const Key,Value> *
BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(current_->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator==(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ == rhs.current_;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<class Key, class Value>
bool
BinarySearchTree<Key, Value>::iterator::operator!=(
    const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return current_ != rhs.current_;
}

/**
* Advances the iterator's location using an in-order sequencing
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator&
BinarySearchTree<Key, Value>::iterator::operator++()
{
    current_ = successor(current_);
    return *this;
}

/*
-------------------------------------------------------------
End implementations for the BinarySearchTree::iterator class.
-------------------------------------------------------------
*/

/*
-----------------------------------------------------
Begin implementations for the BinarySearchTree class.
-----------------------------------------------------
*/

/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() 
{
    root_ = NULL;
}

template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
    clear();
}

/**
 * Returns true if tree is empty
*/
template<class Key, class Value>
bool BinarySearchTree<Key, Value>::empty() const
{
    return root_ == NULL;
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
    printRoot(root_);
    std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::begin() const
{
    BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
    return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::end() const
{
    BinarySearchTree<Key, Value>::iterator end(NULL);
    return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<class Key, class Value>
typename BinarySearchTree<Key, Value>::iterator
BinarySearchTree<Key, Value>::find(const Key & k) const
{
    Node<Key, Value> *curr = internalFind(k);
    BinarySearchTree<Key, Value>::iterator it(curr);
    return it;
}

/**
 * @precondition The key exists in the map
 * Returns the value associated with the key
 */
template<class Key, class Value>
Value& BinarySearchTree<Key, Value>::operator[](const Key& key)
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}
template<class Key, class Value>
Value const & BinarySearchTree<Key, Value>::operator[](const Key& key) const
{
    Node<Key, Value> *curr = internalFind(key);
    if(curr == NULL) throw std::out_of_range("Invalid key");
    return curr->getValue();
}

/**
* An insert method to insert into a Binary Search Tree.
* The tree will not remain balanced when inserting.
* Recall: If key is already in the tree, you should 
* overwrite the current value with the updated value.
*/
template<class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<const Key, Value> &keyValuePair)
{
    Node<Key, Value>* newNode = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
    
    if(root_ == NULL) {
        root_ = newNode;
        return;
    }
    
    Node<Key, Value>* current = root_;
    Node<Key, Value>* parent = NULL;
    
    while(current != NULL) {
        parent = current;
        if(keyValuePair.first < current->getKey())
            current = current->getLeft();
        else if(keyValuePair.first > current->getKey())
            current = current->getRight();
        else {
            current->setValue(keyValuePair.second);
            delete newNode;
            return;
        }
    }
    
    newNode->setParent(parent);
    if(keyValuePair.first < parent->getKey())
         parent->setLeft(newNode);
    else
         parent->setRight(newNode);
}

/**
* A remove method to remove a specific key from a Binary Search Tree.
* Recall: If a node has 2 children you should swap with its predecessor 
* and then remove.
*/
template<typename Key, class Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    Node<Key, Value>* nodeToRemove = internalFind(key);
    if(nodeToRemove == NULL) return;
    
    // If node has two children, swap with predecessor before removal
    if(nodeToRemove->getLeft() != NULL && nodeToRemove->getRight() != NULL) {
        Node<Key, Value>* pred = predecessor(nodeToRemove);
        nodeSwap(nodeToRemove, pred);
    }
    
    // Now proceed with removal after (possibly) having swapped
    Node<Key, Value>* child = (nodeToRemove->getLeft() != NULL) 
        ? nodeToRemove->getLeft() 
        : nodeToRemove->getRight();
    Node<Key, Value>* parent = nodeToRemove->getParent();
    
    if(child != NULL) {
        child->setParent(parent);
    }
    if(parent == NULL) {
        root_ = child;
    } 
    else {
        if(parent->getLeft() == nodeToRemove) parent->setLeft(child);
        else                                  parent->setRight(child);
    }
    
    delete nodeToRemove;
}

/**
 * Implementation for nodeSwap.
 * Swaps the positions of two nodes in the tree using getters/setters 
 * rather than directly accessing protected members.
 */
template<typename Key, class Value>
void BinarySearchTree<Key, Value>::nodeSwap(Node<Key, Value>* n1, Node<Key, Value>* n2)
{
    if (n1 == n2) return;

    // Get original pointers for n1
    Node<Key, Value>* n1Parent = n1->getParent();
    Node<Key, Value>* n1Left   = n1->getLeft();
    Node<Key, Value>* n1Right  = n1->getRight();

    // Get original pointers for n2
    Node<Key, Value>* n2Parent = n2->getParent();
    Node<Key, Value>* n2Left   = n2->getLeft();
    Node<Key, Value>* n2Right  = n2->getRight();

    // Fix each node's parent to point to the other
    if (n1Parent) 
    {
        if (n1Parent->getLeft() == n1) n1Parent->setLeft(n2);
        else                           n1Parent->setRight(n2);
    }
    if (n2Parent) 
    {
        if (n2Parent->getLeft() == n2) n2Parent->setLeft(n1);
        else                           n2Parent->setRight(n1);
    }

    // Swap their parent pointers
    n1->setParent(n2Parent);
    n2->setParent(n1Parent);

    // Fix the left-child pointers
    n1->setLeft(n2Left);
    if (n2Left) n2Left->setParent(n1);
    n2->setLeft(n1Left);
    if (n1Left) n1Left->setParent(n2);

    // Fix the right-child pointers
    n1->setRight(n2Right);
    if (n2Right) n2Right->setParent(n1);
    n2->setRight(n1Right);
    if (n1Right) n1Right->setParent(n2);

    // If either node was the root, update root_
    if (root_ == n1)      root_ = n2;
    else if (root_ == n2) root_ = n1;
}

/**
* A method to remove all contents of the tree and
* reset the values in the tree for use again.
*/
template<typename Key, class Value>
void BinarySearchTree<Key, Value>::clear()
{
    std::function<void(Node<Key, Value>*)> clearRecursive = [&](Node<Key, Value>* node) {
        if(node == NULL) return;
        clearRecursive(node->getLeft());
        clearRecursive(node->getRight());
        delete node;
    };
    clearRecursive(root_);
    root_ = NULL;
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
    Node<Key, Value>* current = root_;
    if(current == NULL) return NULL;
    
    while(current->getLeft() != NULL) {
        current = current->getLeft();
    }
    return current;
}

/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key exists.
*/
template<typename Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    Node<Key, Value>* current = root_;
    while(current != NULL) {
        if(key < current->getKey())       current = current->getLeft();
        else if(key > current->getKey())  current = current->getRight();
        else                              return current;
    }
    return NULL;
}

/**
 * Return true iff the BST is balanced.
 */
template<typename Key, class Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    std::function<int(Node<Key, Value>*)> checkHeight = [&](Node<Key, Value>* node) -> int {
        if(node == NULL) return 0;

        int leftHeight  = checkHeight(node->getLeft());
        if(leftHeight == -1) return -1;

        int rightHeight = checkHeight(node->getRight());
        if(rightHeight == -1) return -1;

        if(std::abs(leftHeight - rightHeight) > 1) return -1;
        return std::max(leftHeight, rightHeight) + 1;
    };
    return (checkHeight(root_) != -1);
}

/**
 * Predecessor helper function.
 */
template<typename Key, class Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::predecessor(Node<Key, Value>* current)
{
    if(current == NULL) return NULL;

    if(current->getLeft() != NULL)
    {
         Node<Key, Value>* temp = current->getLeft();
         while(temp->getRight() != NULL)
              temp = temp->getRight();
         return temp;
    }
    else
    {
         Node<Key, Value>* parent = current->getParent();
         while(parent != NULL && current == parent->getLeft())
         {
              current = parent;
              parent = parent->getParent();
         }
         return parent;
    }
}

/**
 * Successor helper function.
 */
template<typename Key, class Value>
Node<Key, Value>* successor(Node<Key, Value>* current)
{
    if(current == NULL) return NULL;

    if(current->getRight() != NULL) {
        current = current->getRight();
        while(current->getLeft() != NULL) {
            current = current->getLeft();
        }
        return current;
    } else {
        Node<Key, Value>* parent = current->getParent();
        while(parent != NULL && current == parent->getRight()) {
            current = parent;
            parent = parent->getParent();
        }
        return parent;
    }
}

/*
---------------------------------------------------
End implementations for the BinarySearchTree class.
---------------------------------------------------
*/

#include "print_bst.h"

#endif
