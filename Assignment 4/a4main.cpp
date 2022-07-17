// a4main.cpp
#include "AVLTree.h"
#include <iostream>

namespace {

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
    print("", tree.getRoot(), false);
    std::cout << std::endl;
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

// Cite:
// https://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters
template <template <class, class> class Tree, class Key, class Value>
void insertRemoveStressTest(Tree<Key, Value>& tree, int maxSize = 20) {
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
    srand(time(0));

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
    print(tree);
    cout << "Size: " << tree.size() << '\n';

    const int naKey = -1;
    cout << "Search for non-existent key (" << naKey << "): " << tree.search(naKey) << '\n';
}
