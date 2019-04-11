#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <iostream>
using namespace std;

template<typename Data>
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
    BST() : root(0), isize(0), iheight(-1) {  }


    /** Default destructor.
      * Delete every node in this BST.
      */
    virtual ~BST() {
        isize = 0;
        iheight = -1;
        deleteAll(root);
    }

    /** Given a reference to a Data item, insert a copy of it in this BST.
     *  Return true if the item was added to this BST
     *  as a result of this call to insert,
     *  false if an item equal to this one was already in this BST.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=)  For the reasoning
     *  behind this, see the assignment writeup.
     */
    virtual bool insert(const Data& item) {
        BSTNode<Data> * node = new BSTNode<Data>(item);
        if (root == nullptr)
        {
            // BST is empty
            root = node;
            isize++;
            iheight++;
            return true;
        }
        // traverse through the tree
        // use the to_insert var to preserve the ideal location
        BSTNode<Data> * to_traverse = root;
        BSTNode<Data> * to_insert = root;

        while (to_traverse != nullptr)
        {
            to_insert = to_traverse;
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
                return false;
            }
        }

        // comparing item with data in the new node
        if (item < to_insert->data)
        {
            // item is smaller, to the left
            to_insert->left = node;

        }
        else if (to_insert->data < item)
        {
            // item is bigger, to the right
            to_insert->right = node;
        }
        else
        {
            // root data == node data
            return false;
        }
        isize++;
        iheight = height();
        return true;

    }


    /** Find a Data item in the BST.
     *  Return an iterator pointing to the item, or pointing past
     *  the last node in the BST if not found.
     *  Note: This function should use only the '<' operator when comparing
     *  Data items. (should not use ==, >, <=, >=).  For the reasoning
     *  behind this, see the assignment writeup.
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
                BSTIterator<Data> * iterator = new BSTIterator<Data>(to_traverse);
                return *iterator;
            }
        }

        // did not find node with same data
        // return the last node, which is the rightmost node in the tree
        to_traverse = root;
        BSTNode<Data> * target = root;
        while (to_traverse != nullptr)
        {
            target = to_traverse;
            to_traverse = to_traverse->right;
        }
        BSTIterator<Data> * iterator = new BSTIterator<Data>(target);
        return *iterator;

    }

    /** Return the number of items currently in the BST.
    */
    unsigned int size() const {
        return getSize(root);
    }

    /** Return the height of the BST.
     * The height of an empty tree is -1 and the height of a tree
     * with only one node is 0.
     */
    unsigned int height() const {
        return getHeight(root);
    }


    /** Return true if the BST is empty, else false.
     */
    bool empty() const {
        return isize == 0;
    }

    /** Return an iterator pointing to the first item in the BST (not the root).
     */
    iterator begin() const {
        return BST::iterator(first(root));
    }

    /** Return an iterator pointing past the last item in the BST.
     */
    iterator end() const {
        return typename BST<Data>::iterator(0);
    }


    /** Inorder traverse BST, print out the data of each node in ascending order.
     * Implementing inorder and deleteAll base on the pseudo code is an easy way to get started.
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
     */
    static BSTNode<Data>* first(BSTNode<Data>* root) {
        BSTNode<Data> * to_traverse = root;
        while(to_traverse->left != nullptr)
        {
            to_traverse = to_traverse->left;
        }

        return to_traverse->data;
    }

    /** do a postorder traversal, deleting nodes
     */
    static void deleteAll(BSTNode<Data>* n) {
        /* Pseudo Code:
           if current node is null: return;
           recursively delete left sub-tree
           recursively delete right sub-tree
           delete current node
           */
        while(n != nullptr)
        {
            deleteAll(n->left);
            deleteAll(n->right);
            delete n;
        }
    }

    /** Helper method for size()
     *  calculate the number of elements in the tree recursively
     */
    unsigned int getSize(BSTNode<Data> * root) const
    {
        if (root == nullptr)
        {
            return 0;
        }
        return 1 + getSize(root->left) + getSize(root->right);
    }

    /** Helper method for size()
     *  calculate the number of elements in the tree recursively
     *  Return the height of the BST.
     *  The height of an empty tree is -1 and the height of a tree
     *  with only one node is 0.
     */
    unsigned int getHeight(BSTNode<Data> * root) const
    {
        if (root == nullptr)
        {
            return -1;
        }
        unsigned int left = getHeight(root->left);
        unsigned int right = getHeight(root->right);
        if( left > right )
        {
            return left + 1;
        } else
        {
            return right + 1;
        }
    }
    
    /** Helper method
     * Inorder traverse BST, print out the data of each node in ascending order.
     * Implementing inorder and deleteAll base on the pseudo code is an easy way to get started.
     * Pseudo Code:
     * if current node is null: return;
     * recursively traverse left sub-tree
     * print current node data
     * recursively traverse right sub-tree
     */
    void inorderRec(BSTNode<Data> * root) const {
        while (root != nullptr)
        {
            inorderRec(root->left);
            cout << root->data << endl;
            inorderRec(root->right);
        }
    }


};


#endif //BST_HPP
