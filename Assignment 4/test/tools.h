// tools.h
#pragma once
#include "AVLTree.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace tools {

// https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html
constexpr auto RESET = "\033[0m";
constexpr auto RED = "\033[31m";
constexpr auto YELLOW = "\033[33m";
constexpr auto MAGENTA = "\033[35m";
constexpr auto CYAN = "\033[36m";
constexpr auto WHITE = "\033[37m";
constexpr auto BG_RED = "\033[41m";

template <class T>
using HashSet = std::unordered_set<T>;

template <class K, class V>
using HashMap = std::unordered_map<K, V>;

// A root with its type.
template <class Key, class Value, class Node = AVLTreeNode<Key, Value>>
const Node* getRoot(const AVLTree<Key, Value>& tree) {
    return static_cast<Node*>(tree.getRoot());
}

template <class Node>
int realSize(const Node* node) {
    return (node == nullptr) ? 0 : realSize(node->left) + realSize(node->right) + 1;
}

template <class Node>
int realHeight(const Node* node) {
    return (node == nullptr) ? -1 : std::max(realHeight(node->left), realHeight(node->right)) + 1;
}

template <class Node>
bool areSameTrees(const Node* x, const Node* y) {
    auto haveSameContents = [=] {
        return (x->key == y->key) && (x->value == y->value) && (x->height == y->height);
    };
    return (x == nullptr && y == nullptr)
        || (x != nullptr && y != nullptr
            && haveSameContents()
            && areSameTrees(x->left, y->left)
            && areSameTrees(x->right, y->right));
}

template <class Node>
bool hasChildrenLinkedToParents(const Node* node, const Node* parent = nullptr) {
    return (node == nullptr)
        || (node->parent == parent
            && hasChildrenLinkedToParents(node->left, node)
            && hasChildrenLinkedToParents(node->right, node));
}

template <class Key, class Value, class HashSet = std::unordered_set<Key>>
bool hasNoDuplicateKeys(const AVLTreeNode<Key, Value>* node, HashSet&& set = {}) {
    return (node == nullptr)
        || (set.insert(node->key).second
            && hasNoDuplicateKeys(node->left, set)
            && hasNoDuplicateKeys(node->right, std::forward<HashSet>(set)));
}

// Pre: node->height is correct.
template <class Node>
bool isBalancedAt(const Node* node) {
    auto nodeHeight = [](const Node* node) -> int { return (node == nullptr) ? -1 : node->height; };
    return (node == nullptr) || ((std::abs(nodeHeight(node->left) - nodeHeight(node->right)) < 2));
}

template <class Node>
bool isBST(const Node* node, const Node* parent = nullptr, bool isLeftChild = false) {
    if (node == nullptr) {
        return true;
    }
    if (parent == nullptr) {
        return isBST(node->left, node, true) && isBST(node->right, node, false);
    }
    return ((isLeftChild && node->key < parent->key) || (!isLeftChild && node->key > parent->key))
        && isBST(node->left, node, true) && isBST(node->right, node, false);
}

template <class Node>
bool isAVL(const Node* node) {
    return (node == nullptr)
        || (isBST(node) && (realHeight(node) == node->height) && isBalancedAt(node)
            && isAVL(node->left)
            && isAVL(node->right));
}

template <class Tree>
bool isValidAVL(const Tree& tree) {
    const auto root = getRoot(tree);
    return isAVL(root) && hasNoDuplicateKeys(root) && hasChildrenLinkedToParents(root)
        && (realSize(root) == tree.size());
}

// Inspired by BandishBandit.
// Cite: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
template <class Node>
void print(std::ostream& os, const std::string& prefix, const Node* node, bool isLeft) {
    if (node != nullptr) {
        // os << prefix << (isLeft ? "├──" : "└──");
        if (isLeft) os << CYAN << prefix << MAGENTA << "├──" << RESET;
        else        os << CYAN << prefix            << "└──" << RESET;

        // print the value of the node: (Key, Value | Height)
        os << "(" << node->key << ", " << node->value << " | "
           << (isBalancedAt(node) && (realHeight(node) == node->height) ? "" : BG_RED)
           << node->height << RESET << ")\n";

        // enter the next tree level - left and right branch
        print(os, prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(os, prefix + (isLeft ? "│   " : "    "), node->right, false);
    } else {
        // os << prefix << (isLeft ? "├──" : "└──") << "NULL\n";
        if (isLeft) os << CYAN << prefix << MAGENTA << "├──" << WHITE << "NULL\n" << RESET;
        else        os << CYAN << prefix            << "└──" << WHITE << "NULL\n" << RESET;
    }
}

} // namespace tools

template <class Key, class Value>
bool operator==(const AVLTree<Key, Value>& x, const AVLTree<Key, Value>& y) {
    if (x.size() != y.size()) {
        return false;
    }
    const auto rootX = tools::getRoot(x);
    const auto rootY = tools::getRoot(y);
    return (&x == &y && rootX == rootY)
        || (&x != &y
            && ((rootX == nullptr && rootY == nullptr)
                || (rootX != rootY && tools::areSameTrees(rootX, rootY))));
}

template <class Key, class Value>
bool operator!=(const AVLTree<Key, Value>& x, const AVLTree<Key, Value>& y) {
    return !(x == y);
}

// (Key, Value | Height)
template <class Key, class Value>
std::ostream& operator<<(std::ostream& os, const AVLTreeNode<Key, Value>* node) {
    tools::print(os, "", node, false);
    return os;
}

// (Key, Value | Height)
template <class Key, class Value>
std::ostream& operator<<(std::ostream& os, const AVLTree<Key, Value>& tree) {
    using namespace tools;
    os << "\n.------------------.\n"
       << "|" << MAGENTA << "------" << RESET << "Up-Left" << MAGENTA << "-----" << RESET << "|\n"
       << "|" << CYAN << "----" << RESET << "Down-Right" << CYAN << "----" << RESET << "|\n"
       << YELLOW << "*" << RESET << "------------------*\n"
       << tools::getRoot(tree) << "^Size: " << tree.size() << "^" << std::endl;
    return os;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "{ ";
    for (const auto& elem : vec) {
        os << elem << ' ';
    }
    os << "}";
    return os;
}

template <class T>
bool operator==(const std::vector<T>& x, const std::vector<T>& y) {
    if (x.size() != y.size()) {
        return false;
    }
    auto xIt = x.begin();
    auto yIt = y.begin();
    while (xIt < x.end()) {
        if (*xIt++ != *yIt++) {
            return false;
        }
    }
    return true;
}

namespace tools {

template <class Tree>
void print(const Tree& tree) {
    std::cout << tree;
}

template <class Key = int, class Value = char /*Just random default types*/>
void removeTree(const std::vector<Key>& keys, AVLTree<Key, Value>& tree) {
    for (const Key& key : keys) {
        tree.remove(key);
    }
}

template <class Key = int, class Value = char /*Just random default types*/>
void appendTree(const std::vector<Key>& keys, AVLTree<Key, Value>& tree) {
    for (const Key& key : keys) {
        tree.insert(key, {});
    }
}
template <class Key = int, class Value = char /*Just random default types*/>
AVLTree<Key, Value> makeTree(const std::vector<Key>& keys) {
    AVLTree<Key, Value> tree;
    appendTree(keys, tree);
    return tree;
}

// n radom unique integers between 0 and n exclusive.
// Note: rand seed is unchanged.
std::vector<int> randUniqueInts(int n) {
    std::vector<int> range(n);
    std::iota(range.begin(), range.end(), 0);
    // Should replace with mersenne twister sometime.
    std::random_shuffle(range.begin(), range.end());
    return range;
}

} // namespace tools
