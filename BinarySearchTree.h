//
// Created by MARK FONTENOT on 6/22/22.
//

#ifndef FINALPROJEXAMPLES_BINARYSEARCHTREE_H
#define FINALPROJEXAMPLES_BINARYSEARCHTREE_H

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;

template<typename K, typename V>
class BinarySearchTree {
private:
    class TreeNode {
    public:
        K key;
        V value;
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
    };

    TreeNode* root;

    //private insert function
    void insert(TreeNode*& node, const K& k, const V& v);

    void preorder_print(TreeNode* curr, int level) {
        if(curr != nullptr) {
            preorder_print(curr -> left, level + 1);
            cout << setw(level * 2) << curr->key << ": " << curr->value << endl;
            preorder_print(curr -> right, level + 1);
        }
    }
public:

    BinarySearchTree(): root(nullptr) {}

    //public insert function
    void insert(const K&, const V&);

    void print() { preorder_print(root, 0); }

};

template<typename K, typename V>
void BinarySearchTree<K, V>::insert(const K& k, const V& v) {
    insert(root, k, v);
}

template<typename K, typename V>
void BinarySearchTree<K, V>::insert(BinarySearchTree<K, V>::TreeNode*& node, const K& k, const V&v) {

    if(node == nullptr) {
        //time to insert
        node = new BinarySearchTree<K, V>::TreeNode();
        node -> key = k;
        node -> value = v;
    } else if (node->key < k) {
        insert(node -> right, k, v);
    } else if (k < node->key) {
        insert(node -> left, k, v);
    } else
        ; //handle duplicates
}


#endif //FINALPROJEXAMPLES_BINARYSEARCHTREE_H
