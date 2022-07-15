// AVLTree.h
#pragma once
#include <utility>
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
    // left - pointer to left child
    // right - pointer to right child
    // parent - pointer to parent
    AVLTreeNode *left, *right, *parent;

    // Constructors ...
    AVLTreeNode();
    AVLTreeNode(Key key, Value value, AVLTreeNode* parent = nullptr);
    AVLTreeNode(Key key, Value value, unsigned height, AVLTreeNode* left, AVLTreeNode* right,
        AVLTreeNode* parent = nullptr);
};

// AVL Tree Node Methods go here

template <class Key, class Value>
AVLTreeNode<Key, Value>::AVLTreeNode()
    : AVLTreeNode(Key {}, Value {})
{ }

template <class Key, class Value>
AVLTreeNode<Key, Value>::AVLTreeNode(Key key, Value value, AVLTreeNode* parent)
    : AVLTreeNode(std::move(key), std::move(value), 0, nullptr, nullptr, parent)
{ }

template <class Key, class Value>
AVLTreeNode<Key, Value>::AVLTreeNode(Key key, Value value, unsigned height, AVLTreeNode* left,
    AVLTreeNode* right, AVLTreeNode* parent)
    : key(std::move(key))
    , value(std::move(value))
    , height(height)
    , left(left)
    , right(right)
    , parent(parent)
{ }

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
    unsigned size_;
    // Your tree MUST have a root node named root
    AVLTreeNode<Key, Value>* root_;
};

// Helper functions for AVLTree.
namespace impl {

template <class Node>
int height(const Node* node) {
    return (node == nullptr) ? -1 : node->height;
}

} // namespace impl

// AVL Tree Methods go here

template <class Key, class Value>
AVLTree<Key, Value>::AVLTree()
    : size_(0)
    , root_(nullptr)
{ }

template <class Key, class Value>
AVLTree<Key, Value>::AVLTree(const AVLTree& other) {

	// TODO
}

template <class Key, class Value>
AVLTree<Key, Value>& AVLTree<Key, Value>::operator=(AVLTree other) {
	this->swap(other);
	return *this;
}

template <class Key, class Value>
AVLTree<Key, Value>::~AVLTree() {

	// TODO
}

template <class Key, class Value>
bool AVLTree<Key, Value>::insert(Key key, Value value) {
    auto* next = root_;
    auto* current = next;
    // Find insertion point.
    while (next != nullptr) {
        current = next;
        if (key < next->key) {
            next = next->left;
        } else if (key > next->key) {
            next = next->right;
        } else {
            return false; // Indicating duplicate keys.
        }
    }
    // Insert.
    auto* const newNode = new AVLTreeNode<Key, Value>(std::move(key), std::move(value), current);
    if (current == nullptr) { // current == root_ == nullptr.
        current = root_ = newNode;
    } else if (newNode->key < current->key) {
        current->left = newNode;
    } else {
        current->right = newNode;
    }
    size_++;
    // Update heights and rebalance.
    // Traversing back up.
    current->height = 1 + std::max(impl::height(current->left), impl::height(current->right));
    next = current->parent;
    while (next != nullptr) {
        current = next;
        next = next->parent;
        current->height = 1 + std::max(impl::height(current->left), impl::height(current->right));

        // TODO: Balance as needed.
    }
    return true;
}

template <class Key, class Value>
bool AVLTree<Key, Value>::remove(const Key& key) {

	// TODO
    return {};
}

template <class Key, class Value>
void AVLTree<Key, Value>::swap(AVLTree& other) {
    std::swap(size_, other.size_);
    std::swap(root_, other.root_);
}

template <class Key, class Value>
Value AVLTree<Key, Value>::search(const Key& key) const {

	// TODO
    return {};
}

template <class Key, class Value>
std::vector<Value> AVLTree<Key, Value>::values() const {

    // TODO
    return {};
}

template <class Key, class Value>
std::vector<Key> AVLTree<Key, Value>::keys() const {

	// TODO
    return {};
}

template <class Key, class Value>
unsigned AVLTree<Key, Value>::size() const {

	// TODO
    return {};
}
