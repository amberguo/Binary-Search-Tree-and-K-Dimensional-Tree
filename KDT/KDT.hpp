#ifndef KDT_HPP
#define KDT_HPP

#include <algorithm> // sort, max, min
#include <math.h>    // pow, abs
#include <vector>    // vector<typename>
#include <iostream>
#include "Point.hpp"
#include <queue>


using namespace std;

/** A KD tree that can output K nearest neighbors of given query point */
class KDT {

protected:

    /** Inner class that defines a KDNode with certain data point
     *  and pointers to its children and parent
     */
    class KDNode {

    public:

        KDNode* left;
        KDNode* right;
        KDNode* parent;
        Point point;

        KDNode() {}

        KDNode(Point point) : point(point) {}
    };


    KDNode* root; // root of KD tree
    unsigned int numDim; // number of dimension of data points
    unsigned int k; // number of nearest neighbors to find
    double threshold; // largest distance to query point in current KNN
    unsigned int isize;
    unsigned int iheight;

    // using priority queue
    struct distanceComp
    {
        bool operator()(const Point& lhs, const Point& rhs) const
        {
            return lhs.squareDistToQuery < lhs.squareDistToQuery;
        }
    };
    std::priority_queue<Point, std::vector<Point>, distanceComp > KNeighbors;
    //priority_queue<Point> KNeighbors;

public:

    /** TODO: Default constructor of KD tree */
    KDT()
    {
        root = nullptr;
        isize = 0;
        iheight = 0;
        threshold = std::numeric_limits<double>::infinity();
        k = 0;
        numDim = 0;
    }
    ~KDT()
    {
        root = nullptr;
        isize = 0;
        iheight = 0;
        threshold = std::numeric_limits<double>::infinity();
        k = 0;
        numDim = 0;
        deleteAll(root);
    }


    /** Build the KD tree from the given vector of Point references */
    void build(vector<Point>& points) {
        if(points.empty())
        {
            numDim = 0;
            root = nullptr;
            isize = 0;
            iheight = 0;
            threshold = std::numeric_limits<double>::infinity();
            k = 0;
            numDim = 0;
            return;

        } else
        {
            numDim = points.at(0).numDim;
        }
        // build the tree by building left/right subtrees using recursion
        root = buildSubtree(points, 0, points.size(), 0, 0);
        // decrement the height to exclude the null node at the end of paths
        iheight--;
    }

    /** Find k nearest neighbors of the given query point */
    vector<Point> findKNearestNeighbors(Point queryPoint, unsigned int k) {
        threshold = std::numeric_limits<double>::infinity();
        findKNNHelper(root, queryPoint, 0);
        if(KNeighbors.size() > k)
        {
            unsigned int count = KNeighbors.size() - k;
            // to many points in it, delete some nodes with large distance
            for( unsigned int i = 0; i< count; i++)
            {
                KNeighbors.pop();
            }
        }
        vector<Point> res;
        while (!KNeighbors.empty()) {
            res.push_back(std::move(const_cast<Point&>(KNeighbors.top())));
            KNeighbors.pop();
        }
        return res;
    }

    /** Return the size of the KD tree */
    unsigned int size() {
        return isize;
    }

    /** Return the height of the KD tree */
    unsigned int height() {
        return iheight;
    }

private:

    /** Helper method to recursively build the subtree of KD tree. */
    KDNode* buildSubtree(vector<Point>& points, unsigned int start,
        unsigned int end, unsigned int d, unsigned int height) {
        if (points.empty())
        {
            // null check
            return nullptr;
        }
        if (height > iheight)
        {
            // update max height
            iheight = height;
        }
        if (d >= numDim)
        {
            // toggle the dimension
            d = 0;
        }
        if (start >= end)
        {
            // base case for recursion
            return nullptr;
        }
        CompareValueAt comparator = CompareValueAt(d);
        // initial sorting for all the points
        sort(points.begin() + start, points.begin() + end, comparator);
        int median = (end + start) / 2;

        KDNode* newRoot = new KDNode(points.at(median));
        cout << "median is " << median << endl;
        isize++;
        // recursively call, start is inclusive, end is exclusive
        newRoot->left = buildSubtree(points, start, median, d + 1, height + 1);
        if (newRoot->left != nullptr) {
            newRoot->left->parent = newRoot;
        }

        newRoot->right = buildSubtree(points, median + 1, end, d + 1, height + 1);

        if (newRoot->right != nullptr) {
            newRoot->right->parent = newRoot;
        }

        return newRoot;
    }

    /** Helper method to recursively find the K nearest neighbors */
    void findKNNHelper(KDNode * node, const Point & queryPoint, unsigned int d) {
        // null check
        if (node == nullptr) {
            // no neighbor!! 
            // sad lonely guy
            return;
        }
        // leaf node situation
        if (node->left == nullptr && node->right == nullptr) {
            // checking square distance
            node->point.setSquareDistToQuery(queryPoint);
            if (node->point.squareDistToQuery < threshold) {
                // square distance is less than threshold
                // update threshold + update this node into queue
                threshold = node->point.squareDistToQuery;
                updateKNN(node->point);
            }
        }
        // check to go right or left
        if (queryPoint.features[d] < node->point.features[d] ) {
            // dimensional distance of node < that of query, go left
            if (node->left) {
                // recursively go left
                d = incrementD(d);
                findKNNHelper(node->left, queryPoint, d);
                d = decrementD(d);

                if (pow((node->point.features[d] - queryPoint.features[d]), 2) < threshold)
                {
                    d = incrementD(d);
                    findKNNHelper(node->right, queryPoint, d);
                    d = decrementD(d);
                }

                // check square distance
                node->point.setSquareDistToQuery(queryPoint);
                if (node->point.squareDistToQuery < threshold) {
                    // square distance is less than threshold
                    // update threshold + update this node into queue
                    threshold = node->point.squareDistToQuery;
                    updateKNN(node->point);
                }
            }
        }
        // go right
        else {
            if (node->right) {
                // recursively go right
                d = incrementD(d);
                findKNNHelper(node->right, queryPoint, d);
                d = decrementD(d);

                if (pow((node->point.features[d] - queryPoint.features[d]), 2) < threshold)
                {
                    d = incrementD(d);
                    findKNNHelper(node->left, queryPoint, d);
                    d = decrementD(d);
                }
                // check square distance
                node->point.setSquareDistToQuery(queryPoint);
                if (node->point.squareDistToQuery < threshold) {
                    // square distance is less than threshold
                    // update threshold + update this node into queue
                    threshold = node->point.squareDistToQuery;
                    updateKNN(node->point);
                }
            }

        }

    }

    unsigned int incrementD(unsigned int d) {
        if (d == numDim - 1) {
            return 0;
        }
        return d+1;
    }

    unsigned int decrementD(unsigned int d) {
        if (d == 0)
        {
            return numDim - 1;
        }
        else
        {
            return d-1;
        }
    }

    /** Helper method to update your data structure storing KNN using
     *  the given point.
     */
    void updateKNN(Point & point) {
        KNeighbors.push(point);
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
    void inorderRec(KDNode * root) const {
        while (root != nullptr)
        {
            // inorderly traverse and print out the data inside nodes
            inorderRec(root->left);
            for (unsigned int i = 0; i < (root->point.features.size()); i++)
            {
                cout << root->point.features.at(i) << endl;
            }

            inorderRec(root->right);
        }
    }

    /** do a postorder traversal, deleting nodes
     * @param n Root of tree that is to be deleted
     */
    static void deleteAll(KDNode* n) {
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
        // left-right-root
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

};


#endif // KDT_HPP