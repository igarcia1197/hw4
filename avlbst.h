#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
 * A special kind of node for an AVL tree, which adds the balance as a data member.
 * No extra members needed.
 */
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's balance.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. Overridden to return AVLNodes.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
}

template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}

/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/

template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // Implemented below
    virtual void remove(const Key& key);  
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // dhelper functions for rotations.
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);

    // Reblancing after insertion uses a delta‐based approach
    void rebalanceAfterInsert(AVLNode<Key, Value>* node);

    // rebalancing after removal now implemented recursively
    void rebalanceAfterRemove(AVLNode<Key, Value>* node);

    // helper to compute the height of a subtree
    int height(AVLNode<Key, Value>* node);
};


template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if (this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }
    
    AVLNode<Key, Value>* parent = NULL;
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    while (current != NULL) {
        parent = current;
        if (new_item.first < current->getKey())
            current = current->getLeft();
        else if (new_item.first > current->getKey())
            current = current->getRight();
        else {
            // Key exists; update value.
            current->setValue(new_item.second);
            return;
        }
    }
    
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (new_item.first < parent->getKey())
         parent->setLeft(newNode);
    else
         parent->setRight(newNode);
    
    rebalanceAfterInsert(newNode);
}


template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (node == NULL) return;
    
    AVLNode<Key, Value>* parent = node->getParent();
    bool isLeftChild = (parent != NULL && parent->getLeft() == node);
    
    // If the node has two children, swap with its predecessor.
    if (node->getLeft() != NULL && node->getRight() != NULL) {
         AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key,Value>::predecessor(node));
         if (node->getLeft() == pred) {
             // Direct left child: perform a right rotation.
             rotateRight(node);
         } else {
             nodeSwap(node, pred);
         }
         parent = node->getParent();
         isLeftChild = (parent != NULL && parent->getLeft() == node);
    }
    
    // Node now has at most one child.
    AVLNode<Key, Value>* child = (node->getLeft() != NULL) ?
          static_cast<AVLNode<Key, Value>*>(node->getLeft()) :
          static_cast<AVLNode<Key, Value>*>(node->getRight());
    
    if (child != NULL)
         child->setParent(parent);
    if (parent == NULL)
         this->root_ = child;
    else {
         if (isLeftChild)
             parent->setLeft(child);
         else
             parent->setRight(child);
    }
    
    delete node;
    
    if (parent != NULL)
         rebalanceAfterRemove(parent);
}

/*
 * AVLTree::nodeSwap
 *
 * Swaps the positions of two AVLNodes. For non-adjacent nodes we call the base class's
 * nodeSwap then swap balance factors.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

/*
 * Helper function: rotateLeft
 *
 * Performs a left rotation at the given node and updates balance factors.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* r = node->getRight();
    node->setRight(r->getLeft());
    if (r->getLeft() != NULL)
        r->getLeft()->setParent(node);
    r->setParent(node->getParent());
    if (node->getParent() == NULL)
        this->root_ = r;
    else if (node == node->getParent()->getLeft())
        node->getParent()->setLeft(r);
    else
        node->getParent()->setRight(r);
    r->setLeft(node);
    node->setParent(r);
    
    int8_t rBalance = r->getBalance();
    node->setBalance(node->getBalance() - 1 - std::max(rBalance, (int8_t)0));
    r->setBalance(rBalance - 1 + std::min(node->getBalance(), (int8_t)0));
}

/*
 * Helper function: rotateRight
 *
 * Performs a right rotation at the given node and updates balance factors.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* l = node->getLeft();
    node->setLeft(l->getRight());
    if (l->getRight() != NULL)
        l->getRight()->setParent(node);
    l->setParent(node->getParent());
    if (node->getParent() == NULL)
        this->root_ = l;
    else if (node == node->getParent()->getLeft())
        node->getParent()->setLeft(l);
    else
        node->getParent()->setRight(l);
    l->setRight(node);
    node->setParent(l);
    
    int8_t lBalance = l->getBalance();
    node->setBalance(node->getBalance() + 1 - std::min(lBalance, (int8_t)0));
    l->setBalance(lBalance + 1 + std::max(node->getBalance(), (int8_t)0));
}

/*
 * Helper function: rebalanceAfterInsert
 *
 * Walks upward from the inserted node, updating balance factors and performing rotations as needed.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceAfterInsert(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* parent = node->getParent();
    while (parent != NULL) {
        if (node == parent->getLeft())
            parent->updateBalance(-1);
        else
            parent->updateBalance(1);
        
        int8_t balance = parent->getBalance();
        
        if (balance == 0)
            break;
        
        if (balance == 2 || balance == -2) {
            if (balance == 2) { // Right heavy
                AVLNode<Key, Value>* r = parent->getRight();
                if (r->getBalance() < 0) { // Right-Left case
                    rotateRight(r);
                    rotateLeft(parent);
                } else { // Right-Right case
                    rotateLeft(parent);
                }
            } else { // Left heavy
                AVLNode<Key, Value>* l = parent->getLeft();
                if (l->getBalance() > 0) { // Left-Right case
                    rotateLeft(l);
                    rotateRight(parent);
                } else { // Left-Left case
                    rotateRight(parent);
                }
            }
            break;
        }
        node = parent;
        parent = node->getParent();
    }
}

/*
 * Helper function: height
 *
 * Recursively computes the height of the subtree rooted at 'node'. An empty subtree has height 0.
 */
template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node)
{
    if (node == NULL) return 0;
    return std::max(height(node->getLeft()), height(node->getRight())) + 1;
}

/*
 * Helper function: rebalanceAfterRemove
 *
 * Recursively rebalances the tree from the given node upward.
 * At each node, we recompute the height of its left and right subtrees,
 * set its balance factor, and if the node is unbalanced, perform the appropriate rotations.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceAfterRemove(AVLNode<Key, Value>* node)
{
    if (node == NULL) return;
    
    int leftH = (node->getLeft() ? height(node->getLeft()) : 0);
    int rightH = (node->getRight() ? height(node->getRight()) : 0);
    node->setBalance(rightH - leftH);
    
    if (node->getBalance() == 2) {
         AVLNode<Key, Value>* r = node->getRight();
         int rLeftH = (r->getLeft() ? height(r->getLeft()) : 0);
         int rRightH = (r->getRight() ? height(r->getRight()) : 0);
         if (rLeftH > rRightH)
             rotateRight(r);
         rotateLeft(node);
    } else if (node->getBalance() == -2) {
         AVLNode<Key, Value>* l = node->getLeft();
         int lLeftH = (l->getLeft() ? height(l->getLeft()) : 0);
         int lRightH = (l->getRight() ? height(l->getRight()) : 0);
         if (lRightH > lLeftH)
             rotateLeft(l);
         rotateRight(node);
    }
    // Recurse upward.
    rebalanceAfterRemove(node->getParent());
}

#endif
