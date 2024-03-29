/**
 * Name: Yingshan Guo
 * Email: yig152@ucsd.edu
 * Reference: Piazza
 * Date: 4/12/2019
 */
#ifndef BSTNODE_HPP
#define BSTNODE_HPP
#include <iostream>
#include <iomanip>
using namespace std;

template<typename Data>

/*
 * Class Description: This class builds individual components of a tree, which
 * we call "node". The important variables of a node are the data stored,
 * the parent of this node, and the left- and right- child of that node.
 */
class BSTNode {

public:

    /** Constructor.  Initialize a BSTNode with the given Data item,
     *  no parent, and no children.
     */
    BSTNode(const Data & d) : data(d) {
        left = right = parent = nullptr;
    }

    BSTNode<Data>* left;
    BSTNode<Data>* right;
    BSTNode<Data>* parent;
    Data const data;   // the const Data in this node.

    /** Return the successor of this BSTNode in a BST, or 0 if none.
     *  PRECONDITION: this BSTNode is a node in a BST.
     *  POSTCONDITION:  the BST is unchanged.
     *  @return the BSTNode that is the successor of this BSTNode,
     *  or 0 if there is none.
     */
    BSTNode<Data>* successor() {
        // assume that current node is in a BST
        BSTNode<Data>* current = this;
        // right child exists
        if (current->right != nullptr) {
            // find the leftmost child in the right part of tree
            BSTNode<Data>* temp = current->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            return temp;
        }

        // right child does not exists
        BSTNode<Data>* pa = current->parent;
        // find the node which left child is the parent of curr
        while (pa != nullptr && (current == pa->right)) {
            current = pa;
            pa = pa->parent;
        }
        if (parent == nullptr) {
            // the node is the last node of BST, i.e. rightmost
            return 0;
        }
        else {
            return pa;
        }

    }

};

/** Overload operator<< to print a BSTNode's fields to an ostream. */
template <typename Data>
ostream & operator<<(ostream& stm, const BSTNode<Data> & n) {
    stm << '[';
    stm << setw(10) << &n;                 // address of the BSTNode
    stm << "; p:" << setw(10) << n.parent; // address of its parent
    stm << "; l:" << setw(10) << n.left;   // address of its left child
    stm << "; r:" << setw(10) << n.right;  // address of its right child
    stm << "; d:" << n.data;               // its data field
    stm << ']';
    return stm;
}

#endif // BSTNODE_HPP
