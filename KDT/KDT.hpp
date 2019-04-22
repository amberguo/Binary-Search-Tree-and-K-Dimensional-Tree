/**
 * Name: Yingshan Guo
 * Email: yig152@ucsd.edu
 * Reference: Piazza
 * Date: 4/20/2019
 */

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

    // using priority queue to store KNN
    struct distanceComp
    {
        bool operator()(const Point& lhs, const Point& rhs) const
        {
            return lhs.squareDistToQuery < rhs.squareDistToQuery;
        }
    };
    std::priority_queue<Point, std::vector<Point>, distanceComp > KNeighbors;

public:

    KDT()
    {
        root = nullptr;
        isize = 0;
        iheight = 0;
        KNeighbors = priority_queue<Point, std::vector<Point>, distanceComp>();
        threshold = std::numeric_limits<double>::infinity();
        k = 0;
        numDim = 0;
    }
    // destructor of KDT
    ~KDT()
    {
        isize = 0;
        iheight = 0;
        threshold = std::numeric_limits<double>::infinity();
        k = 0;
        numDim = 0;
        while(!KNeighbors.empty())
        {
            KNeighbors.pop();
        }
        deleteAll(root);
    }


    /** Build the KD tree from the given vector of Point references
     *  @param points Points to build KD tree
     */
    void build(vector<Point>& points) {
        if (points.empty())
        {
            numDim = 0;
            root = nullptr;
            isize = 0;
            iheight = 0;
            threshold = std::numeric_limits<double>::infinity();
            k = 0;
            numDim = 0;
            return;
        }
        else
        {
            numDim = points.at(0).numDim;
        }
        // build the tree by building left/right subtrees using recursion
        root = buildSubtree(points, 0, points.size()-1, 0, 0);
    }

    /** Find k nearest neighbors of the given query point 
     * @param queryPoint To point to be searched
     * @param k The number of neighbors to be stored
     * @return vector contains k neighbors
     */
    vector<Point> findKNearestNeighbors(Point queryPoint, unsigned int k) {
        this->k = k;
        threshold = std::numeric_limits<double>::infinity();
        findKNNHelper(root, queryPoint, 0);

        // grab points from pq to vector
        vector<Point> res;
        while (!KNeighbors.empty()) {
            res.push_back(std::move(const_cast<Point&>(KNeighbors.top())));
            KNeighbors.pop();
        }
        return res;
    }

    /** Return the size of the KD tree 
     * @return size of KDT
     */
    unsigned int size() {
        return isize;
    }

    /** Return the height of the KD tree 
     * @return height of KDT
     */
    unsigned int height() {
        return iheight;
    }

private:
    /** Helper method to recursively build the subtree of KD tree.
     * @param points Series of points
     * @param start Start point in the vector
     * @param end End point in the vector
     * @param d Current dimension
     * @param height Current height
     * @return new node created in this recursion
     */
    KDNode* buildSubtree(vector<Point>& points, int start, int end, 
                        unsigned int d, unsigned int height) {
        if (start > end)
        {   
            // base case
            return nullptr;
        }
        if (height > iheight)
        {
            // update max height
            iheight = height;
        }
        if (start == end)
        {
            KDNode* newRoot = new KDNode(points.at(start));
            newRoot->left = nullptr;
            newRoot->right = nullptr;
            isize++;
            return newRoot;
        }
        if (d >= numDim)
        {
            // toggle the dimension
            d = 0;
        }

        CompareValueAt comparator = CompareValueAt(d);
        // initial sorting for all the points
        sort(points.begin() + start, points.begin() + end + 1,
            comparator);
        int median = (end + start) / 2;

        KDNode* newRoot = new KDNode(points.at(median));
        isize++;
        // recursively call, start is inclusive, end is exclusive
        // build left subtree
        newRoot->left = buildSubtree(points, start, median - 1, d + 1,
            height + 1);
        // connect with parent
        if (newRoot->left != nullptr) {
            newRoot->left->parent = newRoot;
        }
        // build right subtree
        newRoot->right = buildSubtree(points, median + 1, end, d + 1,
            height + 1);
        // connect with parent
        if (newRoot->right != nullptr) {
            newRoot->right->parent = newRoot;
        }

        return newRoot;
    }


    /**
     * Helper method of findKNN
     * Recursively find KNN and update pq
     * @param node Current node
     * @param queryPoint Target point
     * @param d Current dimension
     */
    void findKNNHelper(KDNode * node, const Point & queryPoint, unsigned int d) 
    {
        if (node == nullptr) {
            // no neighbor!! 
            // sad lonely guy
            return;
        }

        if (node->left == nullptr && node->right == nullptr)
        {
            // checking square distance
            node->point.setSquareDistToQuery(queryPoint);
            updateKNN(node->point);
            return;
        }
        // check to go right or left
        // dimension d at querypoint is less than this point dimension d
        if (queryPoint.features[d] < node->point.features[d]) {

            // dimensional distance of node < that of query, go left
            if (node->left) {
                // recursively go left
                findKNNHelper(node->left, queryPoint, incrementD(d));
            }

            // update threshold only if KNeighbors is full
            if (KNeighbors.size() == k)
                threshold = KNeighbors.top().squareDistToQuery;
            // check if go to alternative branch
            if (node->right && pow((node->point.features[d] -
                queryPoint.features[d]), 2) < threshold)
                findKNNHelper(node->right, queryPoint, incrementD(d));

        } else {
            // go right
            if (node->right) {
                // recursively go right
                findKNNHelper(node->right, queryPoint, incrementD(d));
            }

            // update threshold only if KNeighbors is full
            if (KNeighbors.size() == k)
                threshold = KNeighbors.top().squareDistToQuery;

            // check if go to alternative branch
            if (node->left && pow((node->point.features[d] -
                queryPoint.features[d]), 2) < threshold)
                findKNNHelper(node->left, queryPoint, incrementD(d));
        }
        // update curr (itself)
        node->point.setSquareDistToQuery(queryPoint);
        updateKNN(node->point);
    }

    /**
     * helper method to increment d
     * @param d Dimension
     * @return value after increment
     */
    unsigned int incrementD(unsigned int d) {
        return (d + 1) % numDim;
    }


    /** Helper method to update your data structure storing KNN using
     *  the given point.
     *  @param point Point to be added into KNeighbors
     */
    void updateKNN(Point & point){
        if (KNeighbors.size() < this->k )
        {
            KNeighbors.push(point);
        }
        else
        {
            if (KNeighbors.top().squareDistToQuery >= point.squareDistToQuery)
            {
                KNeighbors.pop();
                KNeighbors.push(point);
            }
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
        deleteAll(n->left);
        deleteAll(n->right);
        delete n;
    }


};


#endif // KDT_HPP