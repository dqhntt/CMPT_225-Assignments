// a4main.cpp
#include "AVLTree.h"
#include <iostream>

namespace {

// A root with its type.
template <class Key, class Value, class Node = AVLTreeNode<Key, Value>>
const Node* getRoot(const AVLTree<Key, Value>& tree) {
    return static_cast<Node*>(tree.getRoot());
}

// Real size.
template <class Node>
int size(const Node* node) {
    return (node == nullptr) ? 0 : size(node->left) + size(node->right) + 1;
}

// Real height.
template <class Node>
int height(const Node* node) {
    return (node == nullptr) ? -1 : std::max(height(node->left), height(node->right)) + 1;
}

// Cite: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
template <class Node>
void print(const std::string& prefix, const Node* node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << "(" << node->key << ", " << node->value << " | " << node->height << ")\n";
        assert(height(node) == static_cast<int>(node->height));

        // enter the next tree level - left and right branch
        print(prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(prefix + (isLeft ? "│   " : "    "), node->right, false);
    } else {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "NULL\n";
    }
}

template <class Tree>
void print(const Tree& tree) {
    std::cout << ".------------------.\n"
                 "|------Up-Left-----|\n"
                 "|----Down-Right----|\n"
                 "*------------------*\n";
    print("", getRoot(tree), false);
    std::cout << std::endl;
    assert(size(getRoot(tree)) == static_cast<int>(tree.size()));
}

template <class Node>
bool hasChildrenLinkedToParents(const Node* child, const Node* parent = nullptr) {
    return (child == nullptr)
        || (child->parent == parent
            && hasChildrenLinkedToParents(child->left, child)
            && hasChildrenLinkedToParents(child->right, child));
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
    srand(static_cast<unsigned>(time(nullptr))); // srand(time(0));

    AVLTree<int, char> tree;

    insertTest(tree);

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
    assert(hasChildrenLinkedToParents(getRoot(copy)));

    removeTest(tree);

    cout << "Size: " << tree.size() << '\n';
    cout << "Keys:\n";
    for (const auto& key : tree.keys()) {
        cout << key << " --search-- " << tree.search(key) << '\n';
    }
    cout << "Values:\n";
    for (const auto& value : tree.values()) {
        cout << value << '\n';
    }

    insertRemoveStressTest(tree);
    // print(tree);
    cout << "Size: " << tree.size() << '\n';

    try {
        const int naKey = -1;
        cout << "Search for non-existent key (" << naKey << "): " << tree.search(naKey) << '\n';
        assert(false && "Expected exception not thrown.");
    } catch (const std::runtime_error& re) {
        cout << "Expected exception caught: " << re.what() << '\n';
    }
}
