// Basic test program for AVL Tree
// Author: John Edgar
// Date: July 2022

#include <vector>
#include <iostream>
#include "AVLTree.h"
using namespace std;

void a4marking();
template <class K, class V>
bool checkAVLTree(const AVLTree<K, V>& tree);

int main()
{
    a4marking();

    cout << '\n';
}

void a4marking()
{
    AVLTree<int, int> aspen;
    checkAVLTree<int, int>(aspen);

    // Method return values
    bool success = aspen.insert(1, 2);
    success = aspen.remove(1);
    int result = aspen.search(1);
    vector<int> v1 = aspen.search(0, 10);
    vector<int> v2 = aspen.values();
    vector<int> k1 = aspen.keys();
    AVLTree<int, int> larch(aspen);
    aspen = larch;

    cout << endl << "end a4 test" << endl;
}

template <class K, class V>
bool checkAVLTree(const AVLTree<K, V>& tree)
{
    AVLTreeNode<int, int>* nd = (AVLTreeNode<int, int>*)tree.getRoot();
    cout << "height = " << nd->height << '\n';

    return false;
}