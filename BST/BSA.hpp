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
    

    /** Helper method used to either find an element if it exists
     * or else find where it should be inserted.
     */
    virtual size_t binarySearch(const Data& item) const {
        return binarySearchHelper(item);
    }

    virtual int binarySearchHelper(const Data& toSearch) const {
        size_t rc = 0;
        if(v.size() == 0)
        {
            return rc;
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
            // smaller
            rc = start;
        } else if (v.at(start)<toSearch && toSearch < v.at(end))
        {
            // in between
            rc = start + 1;
        } else if (v.at(end) < toSearch)
        {
            rc = end + 1;
        } else
        {
            rc = SIZE_MAX;
        }

        return rc;

    }

    virtual int binarySearchFind(const Data& toSearch) const {
        if (v.empty())
        {
            return 0;
        }
        int start = 0;
        int end = v.size() - 1;
        while (start + 1 < end)
        {
            int mid = start + (end - start) / 2;
            if (v.at(mid) < toSearch)
            {
                start = mid;
            }
            else
            {
                end = mid;
            }
        }
        if (equalsData(v.at(start), toSearch) )
        {
            return start;
        } else if (equalsData(v.at(end), toSearch))
        {
            return end;
        }
        return -1;

    }
    bool static equalsData(const Data& a, const Data& b)
    {
        return !(a < b || b < a);
    }


public:
    vector<Data> v;
    /** Return the position of item, otherwise v.cend() */
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

    /** Insert item into sorted position */
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

    typename vector<Data>::iterator begin() const {
        return v.begin();
    }

    typename vector<Data>::iterator end() const {
        return v.end();
    }
};

#endif
