/**
 * Name: Yingshan Guo
 * Email: yig152@ucsd.edu
 * Reference: Piazza
 * Date: 4/12/2019
 */

#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <iostream>
#include <algorithm>
using namespace std;

template<typename Data>

/**
 * Class Description: The driver of building a binary search tree, where
 * we record the root, height and size of the tree as class variables.
 * The important features of a BST are that we can do insert and search
 * easily with tree structure
 */
class BST {

protected:

    /** Pointer to the root of this BST, or 0 if the BST is empty */
    BSTNode<Data>* root;

    /** Number of Data items stored in this BST. */
    unsigned int isize;

    /** Height of this BST. */
    unsigned int iheight;
  

public:

    /** define iterator as an aliased typename for BSTIterator<Data>. */
    typedef BSTIterator<Data> iterator;

    /** Default constructor.
      * Initialize an empty BST.
      */
    BST() : root(0), isize(0), iheight(0){  }


    /** Default destructor.
      * Delete every node in this BST.
      */
    virtual ~BST() {
      
        deleteAll(root);
    }

    bool static equalsData(const Data& a, const Data& b)
    {
        return !(a < b || b < a);
    }


    /** Given a reference to a Data item, insert a copy of it in this BST.
     *  Return true if the item was added to this BST
     *  as a result of this call to insert,
     *  false if an item equal to this one was already in this BST.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=)  For the reasoning
     *  behind this, see the assignment writeup.
     *  @param item The data of the node that is being inserted
     *  @return result of inserting
     */
    virtual bool insert(const Data& item) {
        BSTNode<Data> * node = nullptr;
        if (root == nullptr)
        {
            // BST is empty
            node = new BSTNode<Data>(item);
            root = node;
            isize++;
            return true;
        }
        // traverse through the tree
        BSTNode<Data> * ops = root;
        unsigned int localHeight = 0;
        while(ops)
        {
            if (equalsData(ops->data, item))
                return false;
            // item is smaller than ops
            // should be in the left subtree
            if (item < ops->data)
            {
                if (ops->left)
                {
                    // left node exists, dive deeper
                    ops = ops->left;
                }
                else
                {
                    // left node is empty
                    // create new node and attach it to the tree
                    node = new BSTNode<Data>(item);
                    ops->left = node;
                    node->parent = ops;
                    localHeight++;
                    break;
                }
            }
            else
            {
                // item is greater than ops
                // should be in the right subtree
                if (ops->right)
                {
                    // right node exists, dive deeper
                    ops = ops->right;
                }
                else
                {
                    // right node is empty
                    // create new node and attach it to the tree
                    node = new BSTNode<Data>(item);
                    ops->right = node;
                    node->parent = ops;
                    localHeight++;
                    break;
                }
            }     
            localHeight++;
        }

        // update height, make it the current longest path in the tree
        iheight = max(iheight, localHeight);
        isize++;
        return true;
    }


    /** Find a Data item in the BST.
     *  Return an iterator pointing to the item, or pointing past
     *  the last node in the BST if not found.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=).  For the reasoning
     *  behind this, see the assignment writeup.
     *  @param item The data of the node that is being searched
     *  @return the iterator that is pointing to the result or pointing past
     *  the last node in the BST if not found.
     */
    virtual iterator find(const Data& item) const {
        BSTNode<Data> * to_traverse = root;
        // traverse through the tree to compare the data field
        while (to_traverse != nullptr)
        {
            if (item < to_traverse->data)
            {
                // target subtree is to the left of to_traverse
                to_traverse = to_traverse->left;
            }
            else if (to_traverse->data < item)
            {
                // target subtree is to the right of to_traverse
                to_traverse = to_traverse->right;
            }
            else
            {
                // data are equal, node is already in the tree
                BSTIterator<Data> iterator(to_traverse);
                return iterator;
            }
   
        }
        // did not find node with same data
        return iterator(nullptr);
    }

    /** @return The number of items currently in the BST.
    */
    unsigned int size() const {
        return getSize(root);
    }

    /**
     * The height of an empty tree is -1 and the height of a tree
     * with only one node is 0.
     * @return The height of the BST.
     */
    unsigned int height() const {
        return getHeight(root);
    }


    /** @return if the BST is empty, else false.
     */
    bool empty() const {
        return isize == 0;
    }

    /** @return an iterator pointing to the first item in the BST 
     * (not the root).
     */
    iterator begin() const {
        return BST::iterator(first(root));
    }

    /** @return an iterator pointing past the last item in the BST.
     */
    iterator end() const {
        return typename BST<Data>::iterator(0);
    }


    /** Inorder traverse BST, print out the data of each node in ascending 
     * order.
     * Implementing inorder and deleteAll base on the pseudo code is an easy 
     * way to get started.
     * Pseudo Code:
     * if current node is null: return;
     * recursively traverse left sub-tree
     * print current node data
     * recursively traverse right sub-tree
     */
    void inorder() const {
        inorderRec();
    }


private:

    /** Find the first element of the BST
     * @param root Root of the target tree
     * @return first element of the tree
     */
    static BSTNode<Data>* first(BSTNode<Data>* root) {
        BSTNode<Data> * to_traverse = root;
        while (to_traverse->left != nullptr)
        {
            // find the leftmost element
            to_traverse = to_traverse->left;
        }

        return to_traverse;
    }

    /** do a postorder traversal, deleting nodes
     * @param n Root of tree that is to be deleted
     */
    static void deleteAll(BSTNode<Data>* n) {
        /* Pseudo Code:
           if current node is null: return;
           recursively delete left sub-tree
           recursively delete right sub-tree
           delete current node
           */
        if (n == nullptr)
        {
            return;
        }
        // post order deleting
        if (n->left != nullptr)
        {
            deleteAll(n->left);
        }
        if (n->right != nullptr)
        {
            deleteAll(n->right);
        }
        delete n;
    }

    /** Helper method for size()
     *  calculate the number of elements in the tree recursively
     *  @param root Root of the tree
     *  @return size of the tree in unsigned int
     */
    unsigned int getSize(BSTNode<Data> * root) const
    {
        // base case
        if (root == nullptr)
        {
            return 0;
        }
        // run through all the nodes and do incrementation
        return 1 + getSize(root->left) + getSize(root->right);
    }

    /** Helper method for size()
     *  calculate the number of elements in the tree recursively
     *  Return the height of the BST.
     *  The height of an empty tree is -1 and the height of a tree
     *  with only one node is 0.
     *  @param root Root of the tree
     *  @return height of the tree in unsigned int
     */
    unsigned int getHeight(BSTNode<Data> * root) const
    {
        // base case
        if (root == nullptr)
        {
            return -1;
        }
        else
        {
            /* calculate the height by recursively calling on right/left
            subtrees */
            int leftdep = getHeight(root->left);
            int rightdep = getHeight(root->right);
            // get the longer path
            if (leftdep > rightdep)
            {
                return leftdep + 1;
            }
            else
            {
                return rightdep + 1;
            }
        }

    }

    /** Helper method
     * Inorder traverse BST, print out the data of each node in ascending order.
     * Implementing inorder and deleteAll base on the pseudo code is an easy 
     * way to get started.
     * Pseudo Code:
     * if current node is null: return;
     * recursively traverse left sub-tree
     * print current node data
     * recursively traverse right sub-tree
     * @param root Root of the tree
     */
    void inorderRec(BSTNode<Data> * root) const {
        while (root != nullptr)
        {
            // inorderly traverse and print out the data inside nodes
            inorderRec(root->left);
            cout << root->data << endl;
            inorderRec(root->right);
        }
    }

};


#endif //BST_HPP
