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
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
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

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
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
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    void rotateLeft(AVLNode<Key,Value>* x);
    void rotateRight(AVLNode<Key,Value>* x);
    void rebalanceInsert(AVLNode<Key,Value>* node);
    void rebalanceRemove(AVLNode<Key,Value>* node, int8_t diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
    if(this->root_ == nullptr)
    {
         this->root_ = newNode;
         return;
    }
    AVLNode<Key,Value>* parent = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key,Value>* curr = parent;
    while(curr != nullptr)
    {
         parent = curr;
         if(new_item.first < curr->getKey())
              curr = curr->getLeft();
         else if(new_item.first > curr->getKey())
              curr = curr->getRight();
         else
         {
              curr->setValue(new_item.second);
              delete newNode;
              return;
         }
    }
    newNode->setParent(parent);
    if(new_item.first < parent->getKey())
         parent->setLeft(newNode);
    else
         parent->setRight(newNode);
    rebalanceInsert(newNode);
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    AVLNode<Key,Value>* nodeToRemove = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if(nodeToRemove == nullptr)
         return;
    if(nodeToRemove->getLeft() != nullptr && nodeToRemove->getRight() != nullptr)
    {
         AVLNode<Key,Value>* pred = static_cast<AVLNode<Key,Value>*>(BinarySearchTree<Key, Value>::predecessor(nodeToRemove));
         this->nodeSwap(nodeToRemove, pred);
    }
    AVLNode<Key,Value>* child = (nodeToRemove->getLeft() != nullptr) ? nodeToRemove->getLeft() : nodeToRemove->getRight();
    AVLNode<Key,Value>* parent = nodeToRemove->getParent();
    if(child != nullptr)
         child->setParent(parent);
    if(parent == nullptr)
         this->root_ = child;
    else
    {
         if(parent->getLeft() == nodeToRemove)
              parent->setLeft(child);
         else
              parent->setRight(child);
    }
    delete nodeToRemove;
    if(parent != nullptr)
    {
         if(child == parent->getLeft())
              rebalanceRemove(parent, -1);
         else
              rebalanceRemove(parent, 1);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* x)
{
    AVLNode<Key,Value>* y = x->getRight();
    x->setRight(y->getLeft());
    if(y->getLeft() != nullptr)
         y->getLeft()->setParent(x);
    y->setParent(x->getParent());
    if(x->getParent() == nullptr)
         this->root_ = y;
    else if(x == x->getParent()->getLeft())
         x->getParent()->setLeft(y);
    else
         x->getParent()->setRight(y);
    y->setLeft(x);
    x->setParent(y);
    x->setBalance(x->getBalance() + 1 - std::min(y->getBalance(), (int8_t)0));
    y->setBalance(y->getBalance() + 1 + std::max(x->getBalance(), (int8_t)0));
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* x)
{
    AVLNode<Key,Value>* y = x->getLeft();
    x->setLeft(y->getRight());
    if(y->getRight() != nullptr)
         y->getRight()->setParent(x);
    y->setParent(x->getParent());
    if(x->getParent() == nullptr)
         this->root_ = y;
    else if(x == x->getParent()->getRight())
         x->getParent()->setRight(y);
    else
         x->getParent()->setLeft(y);
    y->setRight(x);
    x->setParent(y);
    x->setBalance(x->getBalance() - 1 - std::max(y->getBalance(), (int8_t)0));
    y->setBalance(y->getBalance() - 1 + std::min(x->getBalance(), (int8_t)0));
}

template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceInsert(AVLNode<Key,Value>* node)
{
    AVLNode<Key,Value>* parent = node->getParent();
    while(parent != nullptr)
    {
         if(node == parent->getLeft())
              parent->updateBalance(1);
         else
              parent->updateBalance(-1);
         if(parent->getBalance() == 0)
              break;
         if(parent->getBalance() == 2 || parent->getBalance() == -2)
         {
              if(parent->getBalance() == 2)
              {
                   if(parent->getLeft()->getBalance() < 0)
                        rotateLeft(parent->getLeft());
                   rotateRight(parent);
              }
              else
              {
                   if(parent->getRight()->getBalance() > 0)
                        rotateRight(parent->getRight());
                   rotateLeft(parent);
              }
              break;
         }
         node = parent;
         parent = parent->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rebalanceRemove(AVLNode<Key,Value>* node, int8_t diff)
{
    while(node != nullptr)
    {
         node->updateBalance(diff);
         int8_t balance = node->getBalance();
         if(balance == 2 || balance == -2)
         {
              if(balance == 2)
              {
                   if(node->getLeft() != nullptr && node->getLeft()->getBalance() < 0)
                        rotateLeft(node->getLeft());
                   rotateRight(node);
              }
              else
              {
                   if(node->getRight() != nullptr && node->getRight()->getBalance() > 0)
                        rotateRight(node->getRight());
                   rotateLeft(node);
              }
         }
         if(node->getBalance() != 0)
              break;
         AVLNode<Key,Value>* parent = node->getParent();
         if(parent != nullptr)
         {
              if(node == parent->getLeft())
                   diff = -1;
              else
                   diff = 1;
         }
         node = parent;
    }
}

#endif
