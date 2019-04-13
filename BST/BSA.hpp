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
        size_t result = binarySearchHelper(this->v, 0, v.size() - 1, item);
        if(result == ((unsigned int)-1))
        {
            return whereToInsert(v, v.size(), item);
        }
        return result;
        
    }

    virtual size_t binarySearchHelper(vector<Data> v, int left, int right, 
        const Data& toSearch) const {
        int mid = (left + right) / 2;
        if( left <= right )
        {
            
            if (v.at(mid) == toSearch)
                return mid;
            if (v.at(mid) < toSearch)
                return binarySearchHelper(v, mid + 1, right, toSearch);
            return binarySearchHelper(v, left,mid-1, toSearch);
        }

        return -1;
        //whereToInsert(v, v.size(), toSearch);

        //if(v.size() == 0)
        //{
        //    return 0;
        //}

        //while (min <= max)
        //{
        //    mid = (max + min) / 2;
        //    // if at the middle point
        //    if (v.at(mid) == toSearch)
        //    {
        //        return mid;
        //    }
        //    // if at the second half
        //    else if (v.at(mid) < toSearch)
        //    {
        //        min = mid + 1;
        //    }
        //    // if at the first half
        //    else
        //    {
        //        max = mid - 1;
        //    }
        //}
        // not in the array, find where it should be inserted


    }

    size_t whereToInsert(vector<Data> v, size_t size,
                                 const Data& toSearch) const {
        if(size == 0 )
        {
            return 0;
        }
        if( size == 1)
        {
            if (v.at(0) < toSearch)
            {
                return 1;
            } else
            {
                return 0;
            }
        }
        int k = 0;
        for (; k < size-1 ; k++)
        {
            if ((v.at(k) < toSearch) && toSearch < v.at(k + 1))
            {
                return k + 1;
            }
        }
        return k+1;
    }

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
