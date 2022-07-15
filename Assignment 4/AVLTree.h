// AVLTree.h
#pragma once
#include <vector>

// AVL Node Class
template <class Key, class Value>
class AVLTreeNode {
public:
    // Should have attributes named:
    // key - node's key
    Key key;
    // value - node's value
    Value value;
    // height - node's height
    unsigned height;
    // parent - pointer to parent
    // left - pointer to left child
    // right - pointer to right child
    AVLTreeNode *parent, *left, *right;

    // Constructors ...
    AVLTreeNode();
    AVLTreeNode(Key key, Value value, AVLTreeNode* parent = nullptr);
    AVLTreeNode(Key key, Value value, unsigned height, AVLTreeNode* left, AVLTreeNode* right,
        AVLTreeNode* parent = nullptr);
};

// AVL Tree class
template <class Key, class Value>
class AVLTree {
public:
    AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree& operator=(AVLTree other);
    ~AVLTree();
    bool insert(Key key, Value value);
    bool remove(const Key& key);
    void swap(AVLTree& other);
    Value search(const Key& key) const;
    std::vector<Value> values() const;
    std::vector<Key> keys() const;
    unsigned size() const;
    AVLTreeNode<Key, Value>* getRoot() const { return root_; }; // DO NOT REMOVE

private:
    // Tree attributes go here
    unsigned size_ { 0 };
    // Your tree MUST have a root node named root
    AVLTreeNode<Key, Value>* root_ { nullptr };
};






// AVL Tree Methods go here