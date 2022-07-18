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

// Pre: child != nullptr &&
//      child->parent != nullptr
// Post: child is still connected to child->parent,
//       child->parent is no longer connected to child, replaced with newNode.
template <class Node>
void replaceLinkFromParentOf(const Node* child, Node* newNode) {
    assert(child != nullptr && child->parent != nullptr);
    if (child->parent->left == child) {
        child->parent->left = newNode;
    } else {
        child->parent->right = newNode;
    }
}

// Pre: x != nullptr &&
//      y != nullptr &&
//      x->parent != y &&
//      y->parent != x
template <class Node>
void swapNonAdjNodes(Node* x, Node* y) {
    assert(x != nullptr && y != nullptr && x->parent != y && y->parent != x);
    std::swap(x->height, y->height);
    // Swap parents.
    if (x->parent != nullptr) {
        replaceLinkFromParentOf(x, y);
    }
    if (y->parent != nullptr) {
        replaceLinkFromParentOf(y, x);
    }
    std::swap(x->parent, y->parent);
    // Swap left children.
    if (x->left != nullptr && y->left != nullptr) {
        std::swap(x->left->parent, y->left->parent);
    } else if (x->left != nullptr) {
        x->left->parent = y;
    } else {
        y->left->parent = x;
    }
    std::swap(x->left, y->left);
    // Swap right children.
    if (x->right != nullptr && y->right != nullptr) {
        std::swap(x->right->parent, y->right->parent);
    } else if (x->right != nullptr) {
        x->right->parent = y;
    } else {
        y->right->parent = x;
    }
    std::swap(x->right, y->right);
}

// Pre: child != nullptr &&
//      parent != nullptr &&
//      child->parent == parent
template <class Node>
void swapAdjNodes(Node* child, Node* parent) {
    assert(child != nullptr && parent != nullptr && child->parent == parent);
    std::swap(child->height, parent->height);
    // Swap parents.
    if (parent->parent != nullptr) {
        replaceLinkFromParentOf(parent, child);
    }
    child->parent = parent->parent;
    parent->parent = child;
    // Swap children.
    if (parent->left == child) {
        parent->left = child->left;
        if (child->left != nullptr) {
            child->left->parent = parent;
        }
        child->left = parent;
        if (child->right != nullptr && parent->right != nullptr) {
            std::swap(child->right->parent, parent->right->parent);
        } else if (child->right != nullptr) {
            child->right->parent = parent;
        } else {
            parent->right->parent = child;
        }
        std::swap(child->right, parent->right);
    } else {
        // parent->right == child
        parent->right = child->right;
        if (child->right != nullptr) {
            child->right->parent = parent;
        }
        child->right = parent;
        if (child->left != nullptr && parent->left != nullptr) {
            std::swap(child->left->parent, parent->left->parent);
        } else if (child->left != nullptr) {
            child->left->parent = parent;
        } else {
            parent->left->parent = child;
        }
        std::swap(child->left, parent->left);
    }
}

// Pre: x != nullptr && y != nullptr
template <class Node>
void swapNodes(Node& x, Node& y) {
    assert(x != nullptr && y != nullptr);
    if (x == y) {
        return;
    }
    // Swap shells.
    if (x->parent != y && y->parent != x) {
        swapNonAdjNodes(x, y);
    } else if (x->parent == y) {
        swapAdjNodes(x, y);
    } else {
        swapAdjNodes(y, x);
    }
    // Swap caller's handles.
    std::swap(x, y);
}

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
        replaceLinkFromParentOf(node, rightChild);
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
        replaceLinkFromParentOf(node, leftChild);
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
    auto isRightHeavy = [](const Node* node) { return height(node->right) > height(node->left); };
    if (isLeftHeavy(node)) {
        if (isRightHeavy(node->left)) {
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

// Pre: node != nullptr
template <class Node>
Node* predecessor(Node* node) {
    assert(node != nullptr);
    Node* next = node->left;
    while (next != nullptr) {
        node = next;
        next = next->right;
    }
    return node;
}

// Pre: node != nullptr
template <class Node>
Node* successor(Node* node) {
    assert(node != nullptr);
    Node* next = node->right;
    while (next != nullptr) {
        node = next;
        next = next->left;
    }
    return node;
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

// Returns a clone of root.
// Cite: https://stackoverflow.com/questions/49796568
template <class Node>
Node* cloneAVL(const Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    Node* const newNode = new Node(
        root->key, root->value, root->height, cloneAVL(root->left), cloneAVL(root->right));
    if (newNode->left != nullptr) {
        newNode->left->parent = newNode;
    }
    if (newNode->right != nullptr) {
        newNode->right->parent = newNode;
    }
    return newNode;
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
    : size_(other.size_)
    , root_(impl::cloneAVL(other.root_))
{ }


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
    size_++;
    return true;
}

template <class Key, class Value>
bool AVLTree<Key, Value>::remove(const Key& key) {
    decltype(root_) node = nullptr;
    // Check if key exists.
    try {
        node = impl::nodeMatching(key, root_);
    } catch (const std::runtime_error&) {
        return false;
    }
    auto* next = node;
    if (node->left != nullptr && node->right != nullptr) {
        // 2 children.
        const bool usePredecessor = rand() % 2;
        next = usePredecessor ? impl::predecessor(node) : impl::successor(node);
        impl::swapNodes(node, next);
        // Swapped nodes instead of data.
        if (root_ == next) {
            root_ = node;
        }
        node = next->parent;
        // Destroy and reconnect.
        if (usePredecessor) {
            // Predecessor can't have right child.
            if (node->right == next) {
                node->right = next->left;
            } else {
                node->left = next->left;
            }
            if (next->left != nullptr) {
                next->left->parent = node;
            }
        } else {
            // Successor can't have left child.
            if (node->left == next) {
                node->left = next->right;
            } else {
                node->right = next->right;
            }
            if (next->right != nullptr) {
                next->right->parent = node;
            }
        }
        delete next;
        next = nullptr;
    } else {
        // 0 or 1 child.
        if (node->left == nullptr) {
            next = node->right;
        } else {
            next = node->left;
        }
        auto* const parent = node->parent;
        // Reconnect.
        if (parent != nullptr) {
            if (parent->left == node) {
                parent->left = next;
            } else {
                parent->right = next;
            }
        } else { // Already at the top.
            root_ = next;
        }
        if (next != nullptr) {
            next->parent = parent;
        }
        delete node;
        node = parent;
    }
    // Update heights and rebalance.
    // Traversing back up.
    next = node;
    while (next != nullptr) {
        node = next;
        next = next->parent;
        impl::updateHeight(node);
        if (!impl::isBalanced(node)) {
            impl::balance(node);
            if (node == root_) {
                root_ = node->parent;
            }
        }
    }
    size_--;
    return true;
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
