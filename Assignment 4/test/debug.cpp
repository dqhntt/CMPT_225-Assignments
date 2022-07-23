// debug.cpp
#include "AVLTree.h"
#include "tools.h"
#include <iostream>
#include <string>

template <class Tree>
std::string treeSpec(const Tree& tree) {
    if (tools::isValidAVL(tree)) {
        return "^Is a valid AVL tree.\n";
    }
    return std::string("^Is ") + tools::RED + "NOT" + tools::RESET + " a valid AVL tree.\n";
}

// For quickly visualizing tree with some test data.
// https://visualgo.net/en/avl

int main() {
    using std::cout;
    using namespace tools;

    // Sample usage.

    // tools::makeTree initialize values to blank characters.

    auto tree = makeTree({ 3, 1, 2 });

    cout << tree << treeSpec(tree) << "\n";

    tree.insert(5, {});
    tree.remove(1);

    cout << tree << treeSpec(tree) << "\n";

    // Multiple keys.

    appendTree({ 7, 9, 8 }, tree);
    removeTree({ 2, 4, 6 }, tree);

    cout << tree << treeSpec(tree) << "\n";
}
