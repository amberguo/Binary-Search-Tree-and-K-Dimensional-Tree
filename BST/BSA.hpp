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
     */
    virtual size_t binarySearch(const Data& item) const {
        return binarySearchHelper(this->v, item);
        //if (result == INT_MAX)
        //{
        //    return whereToInsert(v, v.size(), item);
        //}
        //return result;
        
    }

    virtual size_t binarySearchHelper(vector<Data> v, 
        const Data& toSearch) const {
        if(v.size() == 0)
        {
            return 0;
        }
        int start = 0;
        int end = v.size() - 1;
        while(start + 1 < end )
        {
            int mid = start + (end - start) / 2;
            if (v.at(mid) < toSearch)
            {
                start = mid;
            } else
            {
                end = mid;
            } 
        }
        if( toSearch < v.at(start) ) 
        {
            return start;
        } else if (v.at(start)<toSearch && toSearch < v.at(end))
        {
            return start + 1;
        } else if (v.at(end) < toSearch)
        {
            return end + 1;
        }

    }

    /*size_t whereToInsert(vector<Data> v, size_t size,
        const Data& toSearch) const {
        if (size == 0)
        {
            return 0;
        }
        if (size == 1)
        {
            if (v.at(0) < toSearch)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        int k = 0;
        for (; k < size - 1; k++)
        {
            if ((v.at(k) < toSearch) && toSearch < v.at(k + 1))
            {
                return k + 1;
            }
        }
        return k + 1;
    }*/

public:
    /** Return the position of item, otherwise v.cend() */
    virtual typename vector<Data>::const_iterator find(const Data& item)
        const
    {
        for (auto it = v.cbegin(); it != v.cend(); ++it)
        {
            if (*it < item || item < *it)
            {

            }
            else
            {
                // item = *it
                return it;
            }
        }
        return v.cend();
    }

    /** Insert item into sorted position */
    virtual bool insert(const Data& item) {
        // traverse the vector and find target item
        // if exists, duplicated
        for (auto it = v.begin(); it != v.end(); ++it)
        {
            if (*it < item || item < *it)
            {

            }
            else
            {
                // item = *it
                // duplicate 
                return false;
            }
        }

        size_t position = binarySearch(item);
        auto it = v.begin();
        for (size_t i = 0; i < position; i++)
        {
            ++it;
        }
        v.insert(it, item);
        return true;
    }

    typename vector<Data>::iterator begin() const {
        return v.begin();
    }

    typename vector<Data>::iterator end() const {
        return v.end();
    }
};

#endif
