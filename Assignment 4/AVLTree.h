// AVLTree.h
#pragma once
// #define NDEBUG
#include <cassert>
#include <stdexcept>
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
    const Value& search(const Key& key) const;
    std::vector<Value> values() const;
    std::vector<Key> keys() const;
    std::size_t size() const;
    AVLTreeNode<Key, Value>* getRoot() const { return root_; }; // DO NOT REMOVE

private:
    // Tree attributes go here
    std::size_t size_;
    // Your tree MUST have a root node named root
    AVLTreeNode<Key, Value>* root_;
};

// Helper functions for AVLTree.
namespace impl {

template <class Node>
int height(const Node* node) {
    return (node == nullptr) ? -1 : node->height;
}

// Update node's height based on its children's heights.
// Pre: node != nullptr
template <class Node>
void updateHeight(Node* node) {
    assert(node != nullptr);
    node->height = 1 + std::max(height(node->left), height(node->right));
}

// Pre: node != nullptr && node->right != nullptr
// Cite: Lecture slide (John Edgar).
template <class Node>
void leftRotate(Node* const node) {
    assert(node != nullptr && node->right != nullptr);
    Node* const rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }
    rightChild->parent = node->parent;
    if (node->parent != nullptr) {
        if (node == node->parent->left) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }
    }
    rightChild->left = node;
    node->parent = rightChild;
    updateHeight(node);
    updateHeight(rightChild);
    if (rightChild->parent != nullptr) {
        updateHeight(rightChild->parent);
    }
}

// Pre: node != nullptr && node->left != nullptr
// Cite: Lecture slide (John Edgar).
template <class Node>
void rightRotate(Node* const node) {
    assert(node != nullptr && node->left != nullptr);
    Node* const leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }
    leftChild->parent = node->parent;
    if (node->parent != nullptr) {
        if (node == node->parent->right) {
            node->parent->right = leftChild;
        } else {
            node->parent->left = leftChild;
        }
    }
    leftChild->right = node;
    node->parent = leftChild;
    updateHeight(node);
    updateHeight(leftChild);
    if (leftChild->parent != nullptr) {
        updateHeight(leftChild->parent);
    }
}

// Pre: node != nullptr
template <class Node>
bool isBalanced(const Node* node) {
    assert(node != nullptr);
    return std::abs(height(node->left) - height(node->right)) < 2;
}

//  Pre: !isBalanced(node)
// Post: isBalanced(node)
template <class Node>
void balance(Node* node) {
    assert(node != nullptr);
    auto isLeftHeavy = [](const Node* node) { return height(node->left) > height(node->right); };
    if (isLeftHeavy(node)) {
        if (!isLeftHeavy(node->left)) {
            leftRotate(node->left);
        }
        rightRotate(node);
    } else {
        if (isLeftHeavy(node->right)) {
            rightRotate(node->right);
        }
        leftRotate(node);
    }
}

template <class Node>
void destroy(Node* node) {
    if (node != nullptr) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

template <class Node, class UnaryFunction = void(Node*)>
void traverseInOrder(Node* node, UnaryFunction func) {
    if (node != nullptr) {
        traverseInOrder(node->left, func);
        func(node);
        traverseInOrder(node->right, func);
    }
}

template <class Key, class Node>
Node* nodeMatching(const Key& key, Node* node) {
    while (node != nullptr) {
        if (key < node->key) {
            node = node->left;
        } else if (key > node->key) {
            node = node->right;
        } else {
            return node;
        }
    }
    throw std::runtime_error("Key not found.");
}

} // namespace impl

// AVL Tree Methods go here

template <class Key, class Value>
AVLTree<Key, Value>::AVLTree()
    : size_(0)
    , root_(nullptr)
{ }

template <class Key, class Value>
AVLTree<Key, Value>::AVLTree(const AVLTree& other)
    : AVLTree()
{
    // TODO

    // An AVL tree but not the same.
    impl::traverseInOrder(other.root_,
        [this](const AVLTreeNode<Key, Value>* node) { this->insert(node->key, node->value); });
}

template <class Key, class Value>
AVLTree<Key, Value>& AVLTree<Key, Value>::operator=(AVLTree other) {
    this->swap(other);
    return *this;
}

template <class Key, class Value>
AVLTree<Key, Value>::~AVLTree() {
    impl::destroy(root_);
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
    impl::updateHeight(current);
    next = current->parent;
    while (next != nullptr) {
        current = next;
        next = next->parent;
        impl::updateHeight(current);
        if (!impl::isBalanced(current)) {
            impl::balance(current);
            if (current == root_) {
                root_ = current->parent;
            }
        }
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
const Value& AVLTree<Key, Value>::search(const Key& key) const {
    return impl::nodeMatching(key, root_)->value;
}

template <class Key, class Value>
std::vector<Value> AVLTree<Key, Value>::values() const {
    std::vector<Value> values;
    values.reserve(size_);
    impl::traverseInOrder(
        root_, [&values](const AVLTreeNode<Key, Value>* node) { values.push_back(node->value); });
    return values;
}

template <class Key, class Value>
std::vector<Key> AVLTree<Key, Value>::keys() const {
    std::vector<Key> keys;
    keys.reserve(size_);
    impl::traverseInOrder(
        root_, [&keys](const AVLTreeNode<Key, Value>* node) { keys.push_back(node->key); });
    return keys;
}

template <class Key, class Value>
std::size_t AVLTree<Key, Value>::size() const {
    return size_;
}
