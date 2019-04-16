/**
 * Name: Yingshan Guo
 * Email: yig152@ucsd.edu
 * Reference: Piazza
 * Date: 4/12/2019
 */
#ifndef BSA_HPP
#define BSA_HPP

#include <vector>
using namespace std;

template<typename Data>

/** This class implements a binary search array */
class BSA {

private:
    vector<Data> v;

    /** Helper method used to either find an element if it exists
     * or else find where it should be inserted.
     * @param item The target data to be searched
     * @return index of element in the list, or the position that it should be 
     * inserted
     */
    virtual size_t binarySearch(const Data& item) const {
        const int MIDDLE_DIVISOR = 2;
        size_t rc = 0;
        // base case
        if (v.empty())
        {
            return rc;
        }
        int start = 0;
        int end = v.size() - 1;
        // modify the value of start and end
        while (start + 1 < end)
        {
            int mid = start + (end - start) / MIDDLE_DIVISOR;
            // target is in the second part
            if (v.at(mid) < item)
            {
                start = mid;
            }
            // target is in the first part
            else
            {
                end = mid;
            }
        }
        
        if (item < v.at(start))
        {
            // smaller
            rc = start;
        }
        else if (v.at(start) < item && item < v.at(end))
        {
            // in between
            rc = start + 1;
        }
        else if (v.at(end) < item)
        {
            // greater
            rc = end + 1;
        }
        else
        {
            // equal to end or start, duplicated
            rc = SIZE_MAX;
        }
        return rc;
    }

    /** Helper method used to either find an element if it exists
     * or else find
     * @param toSearch The target data to be searched
     * @return index of element in the list, or the position that it should be
     * inserted
     */
    virtual int binarySearchFind(const Data& toSearch) const {
        const int MIDDLE_DIVISOR = 2;
        // base case
        if (v.empty())
        {
            return 0;
        }
        int start = 0;
        int end = v.size() - 1;
        // modify the value of start and end
        while (start + 1 < end)
        {
            int mid = start + (end - start) / MIDDLE_DIVISOR;
            // target is in the second part
            if (v.at(mid) < toSearch)
            {
                start = mid;
            }
            // target is in the first part
            else
            {
                end = mid;
            }
        }
        //check if item is equal to start/ end
        if (equalsData(v.at(start), toSearch) )
        {
            return start;
        } else if (equalsData(v.at(end), toSearch))
        {
            return end;
        }
        return -1;

    }

    /**
     * Helper method to see if two data are equal
     * @param a Data one
     * @param b Data two
     * @return true if a and b are equal, false if they are not
     */
    bool static equalsData(const Data& a, const Data& b)
    {
        return !(a < b || b < a);
    }


public:
    
    /** Return the position of item, otherwise v.cend() 
     * @param item The target data to be searched
     * @return iterator that points to target data in the array
     */
    virtual typename vector<Data>::const_iterator find(const Data& item)
        const
    {
        int idx = binarySearchFind(item);
        if( idx != -1)
        {
            auto nth = v.begin() + idx;
            return nth;
        }

        return v.cend();
    }

    /** Insert item into sorted position 
     * @param item The target data to be inserted
     * @return true if insert is successful, false if duplicated
     */
    virtual bool insert(const Data& item) {
        // traverse the vector and find target item
        // if exists, duplicated
        size_t position = binarySearch(item);
        if( position == SIZE_MAX)
        {
            return false;
        }
        v.insert(v.begin() + position, item);
        return true;
    }

    /**
     * get iterator that points to the start of array
     * @return iterator
     */
    typename vector<Data>::iterator begin() const {
        return v.begin();
    }

    /**
     * get iterator that points to the end of array
     * @return iterator
     */
    typename vector<Data>::iterator end() const {
        return v.end();
    }
};

#endif
