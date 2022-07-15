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
    if (node == nullptr) {
        return;
    }
    print(node->left);
    cout << "(" << node->key << ", " << node->value << " | " << node->height << ")\n";
    print(node->right);
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
    tree.insert(1, 'a');
    print(tree);
    tree.insert(-5, 'n');
    print(tree);
    tree.insert(1, 'b');
    print(tree);
    tree.insert(2, 'b');
    print(tree);
    tree.insert(0, 'o');
    print(tree);
    tree.insert(3, 'c');
    print(tree);
}
