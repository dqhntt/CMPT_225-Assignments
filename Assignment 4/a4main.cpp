// a4main.cpp
#include "AVLTree.h"
#include <iostream>
using namespace std;

namespace {

// Real height.
template <class Node>
int height(const Node* node) {
    if (node == nullptr) {
        return -1;
    }
    if (node->left == nullptr) {
        return height(node->right) + 1;
    }
    if (node->right == nullptr) {
        return height(node->left) + 1;
    }
    return std::max(height(node->left), height(node->right)) + 1;
}

template <class Node>
void print(const Node* node) {
    if (node != nullptr) {
        print(node->left);
        cout << "(" << node->key << ", " << node->value << " | " << node->height << ")\n";
        print(node->right);
    }
}

// Cite: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
template <class Node>
void print(const std::string& prefix, const Node* node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << "(" << node->key << ", " << node->value << " | " << node->height << "; "
                  << height(node) << ")\n";

        // enter the next tree level - left and right branch
        print(prefix + (isLeft ? "│   " : "    "), node->left, true);
        print(prefix + (isLeft ? "│   " : "    "), node->right, false);
    } else {
        std::cout << prefix << (isLeft ? "├──" : "└──") << "NULL\n";
    }
}

template <class Tree>
void print(const Tree& tree) {
    cout << "----------\n";
    // print(tree.getRoot());
    print("", tree.getRoot(), false);
    cout << "----------\n";
}

} // namespace

int main() {
    AVLTree<int, char> tree;
    char c = 'a';
    for (int i = 0; i < 10; i++) {
        tree.insert(i, c++);
        print(tree);
    }
    cout << "Size: " << tree.size() << '\n';
    cout << "Keys:\n";
    for (auto key : tree.keys()) {
        cout << key << '\n';
    }
    cout << "Values:\n";
    for (auto value : tree.values()) {
        cout << value << '\n';
    }
    cout << "Search for 0: " << tree.search(0) << '\n';
}
