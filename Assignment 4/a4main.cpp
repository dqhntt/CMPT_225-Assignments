// a4main.cpp
#include "AVLTree.h"
#include <iostream>
#include <unordered_set>
#include <utility>

namespace {

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//      Functions taking trees instead of nodes:                                                 //
//      - getRoot(tree)                                                                          //
//      - print(tree)  // Nodes are printed as (Key, Value | Height), left on top, right below.  //
//      - isValidAVL(tree)                                                                       //
//                                                                                               //
//      Functions that can operate on substree rooted at `node` parameter:                       //
//      Assuming `root` was obtained using getRoot(tree):                                        //
//      - realSize(root)                                                                         //
//      - realHeight(root)                                                                       //
//      - hasChildrenLinkedToParents(root)  // For testing copy ctor.                            //
//      - hasNoDuplicateKeys(root)                                                               //
//      - isBalancedAt(root)  // Pre: node->height == realHeight(node).                          //
//      - isBST(root)                                                                            //
//      - isAVL(root)  // Less comprehensive than isValidAVL(tree).                              //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

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

// Don't call this directly.
// Cite: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
template <class Node>
void print(const std::string& prefix, const Node* node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << "(" << node->key << ", " << node->value << " | " << node->height << ")\n";
        assert(realHeight(node) == static_cast<int>(node->height));

        // enter the next tree level - left and right branch
        print(prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(prefix + (isLeft ? "│   " : "    "), node->right, false);
    } else {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "NULL\n";
    }
}

// (Key, Value | Height)
template <class Tree>
void print(const Tree& tree) {
    std::cout << ".------------------.\n"
                 "|------Up-Left-----|\n"
                 "|----Down-Right----|\n"
                 "*------------------*\n";
    print("", getRoot(tree), false);
    std::cout << "^Size: " << tree.size() << "^\n" << std::endl;
    assert(realSize(getRoot(tree)) == static_cast<int>(tree.size()));
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
    return (node == nullptr) || (std::abs(nodeHeight(node->left) - nodeHeight(node->right)) < 2);
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

template <class Tree>
void insertTest(Tree& tree) {
    char c = 'a';
    for (int i = 0; i <= 5; i++) {
        std::cout << "Insert key " << i << ":\n";
        assert(tree.insert(i, c++));
        // print(tree);
    }
    for (int i = -2; i <= 8; i++) {
        std::cout << "Insert key " << i << ":\n";
        tree.insert(i, c++);
        // print(tree);
    }
    for (int i = 10; i >= -5; i--) {
        std::cout << "Insert key " << i << ":\n";
        tree.insert(i, c++);
        print(tree);
    }
}

template <class Tree>
void removeTest(Tree& tree) {
    for (int i = 10; i >= 7; i--) {
        std::cout << "Remove key " << i << ":\n";
        assert(tree.remove(i));
        // print(tree);
    }
    for (int i = -10; i <= -2; i++) {
        std::cout << "Remove key " << i << ":\n";
        tree.remove(i);
        // print(tree);
    }
    for (int i = -1; i < 7; i += (rand() % 2 + 1)) {
        std::cout << "Remove key " << i << ":\n";
        tree.remove(i);
        print(tree);
    }
}

template <class Key, class Value>
void insertRemoveStressTest(AVLTree<Key, Value>& tree, unsigned maxSize = 2000) {
    static_assert(std::is_convertible<int, Key>::value,
        "int to Key conversion is needed for this test function.");
    tree = {};
    tree.insert(0, {});
    tree.insert(1, {});
    tree.insert(2, {});
    tree.remove(1);
    tree.remove(0);
    tree.remove(2);
    while (tree.size() < maxSize) {
        tree.insert(rand() % maxSize, {});
    }
    while (tree.size() > 0) {
        tree.remove(rand() % maxSize);
    }
    while (tree.size() < maxSize) {
        tree.insert(rand() % maxSize, {});
    }
}

} // namespace

int main() {
    using std::cout;

    AVLTree<int, char> tree;

    insertTest(tree);
    assert(isValidAVL(tree));

    cout << "Size: " << tree.size() << '\n';
    cout << "Keys:\n";
    for (const auto& key : tree.keys()) {
        cout << key << " --search-- " << tree.search(key) << '\n';
    }
    cout << "Values:\n";
    for (const auto& value : tree.values()) {
        cout << value << '\n';
    }

    cout << '\n';

    cout << "New copied tree:\n";
    const auto copy = tree;
    print(copy);
    assert(isValidAVL(copy));

    removeTest(tree);
    assert(isValidAVL(tree));

    cout << "Size: " << tree.size() << '\n';
    cout << "Keys:\n";
    for (const auto& key : tree.keys()) {
        cout << key << " --search-- " << tree.search(key) << '\n';
    }
    cout << "Values:\n";
    for (const auto& value : tree.values()) {
        cout << value << '\n';
    }

    for (int i = 1; i < 3000; i = i * 2 + rand() % 2) {
        insertRemoveStressTest(tree, i);
    }
    // print(tree);
    assert(isValidAVL(tree));
    cout << "Size: " << tree.size() << '\n';

    try {
        const int naKey = -1;
        cout << "Search for non-existent key (" << naKey << "): " << tree.search(naKey) << '\n';
        assert(false && "Expected exception not thrown.");
    } catch (const std::runtime_error& re) {
        cout << "Expected exception caught: " << re.what() << '\n';
    }
}
