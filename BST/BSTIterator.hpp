/**
 * Name: Yingshan Guo
 * Email: yig152@ucsd.edu
 * Reference: Piazza
 * Date: 4/12/2019
 */
#ifndef BSTITERATOR_HPP
#define BSTITERATOR_HPP
#include "BSTNode.hpp"
#include <list>
#include <iterator>
using namespace std;

template<typename Data>
/*
 * Class Description: This class builds iterator, which is a useful tool for
 * BST so that programmers can easily traverse the tree. Plus we have 
 * redefined the operators for iterators(==, !=, *), makes it easier for us
 * to test
 */
class BSTIterator : public iterator<input_iterator_tag, Data> {

private:

    BSTNode<Data>* curr;

public:

    /** Constructor.  Use the argument to initialize the current BSTNode
     *  in this BSTIterator.
     */
    BSTIterator(BSTNode<Data>* curr) : curr(curr) { }

    /** Dereference operator.
     * @return data in the node
     */
    Data operator*() const {
        return curr->data;
    }

    /** Pre-increment operator.
     * @return iterator that pointing to the successor of current node
     */
    BSTIterator<Data>& operator++() {
        curr = curr->successor();
        return *this;
    }

    /** Post-increment operator. 
     * @return iterator that pointing to the successor of current node
     */
    BSTIterator<Data> operator++(int) {
        BSTIterator before = BSTIterator(curr);
        ++(*this); // increment the value pointed by this, which is successor
        return before;
    }

    /** Equality test operator.
     * Notice that other is a reference and not a pointer, thus it
     * cannot be null. Return true if other is equal to the calling
     * object. Two iterators are equal if they point to the same
     * BSTNode in the same BST.
     * @param other Iterator that is to be compared with this
     * @return result of comparision between data in two nodes pointed by
     * two iterators
     */
    bool operator==(BSTIterator<Data> const & other) const {
        // check if calling obj and other are pointing to the same node
        if( other.curr == this->curr)
        {
            return true;
        }
        return false;
        /*if((*other < **this)|| (**this < *other)) 
        {
            return false;
        }
        return true;*/
    }

    /** Inequality test operator. 
     *  @param other Iterator that is to be compared with this
     *  @return result of comparision between data in two nodes pointed by
     *  two iterators */
    bool operator!=(BSTIterator<Data> const & other) const {
        // check if calling obj and other are not pointing to the same node
        if (other.curr != this->curr)
        {
            return true;
        }
        return false;
    }

};

#endif //BSTITERATOR_HPP
