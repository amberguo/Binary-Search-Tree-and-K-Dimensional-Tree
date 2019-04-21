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
        threshold = std::numeric_limits<double>::infinity();
        k = 0;
        numDim = 0;
    }
    // destructor of KDT
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
        //vector<Point> haha;
        //for(unsigned int i =0; i< points.size()-1; i++)
        //{
        //    haha.push_back(points.at(i));
        //}
        // build the tree by building left/right subtrees using recursion
        root = buildSubtree2(points, 0, points.size(), 0, 0);
        // decrement the height to exclude the null node at the end of paths
        //iheight--;
        //inorderRec(root);
    }

    /** Find k nearest neighbors of the given query point */
    vector<Point> findKNearestNeighbors(Point queryPoint, unsigned int k) {
        this->k = k;
        threshold = std::numeric_limits<double>::infinity();
        findKNNHelperAll(root, queryPoint, 0);

        //if (KNeighbors.size() == k + 1)
        //{
        //    Point tmp = KNeighbors.top();
        //    KNeighbors.pop();
        //}
        //if(KNeighbors.size() > k)
        //{
        //    unsigned int diff = k - KNeighbors.size();
        //    for (unsigned int i = 0; i < k; i++)
        //    {
        //        KNeighbors.pop();
        //    }
        //}

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

    KDNode* buildSubtree2(vector<Point>& points, int start, int end, 
                        unsigned int d, unsigned int height) {
        if (start > end)
        {
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

        CompareValueAt comparator = CompareValueAt(d);
        // initial sorting for all the points
        sort(points.begin() + start, points.begin() + end + 1,
            comparator);
        int median = (end + start) / 2;

        KDNode* newRoot = new KDNode(points.at(median));
        isize++;
        // recursively call, start is inclusive, end is exclusive
        newRoot->left = buildSubtree2(points, start, median - 1, d + 1,
            height + 1);
        if (newRoot->left != nullptr) {
            newRoot->left->parent = newRoot;
        }

        newRoot->right = buildSubtree2(points, median + 1, end, d + 1,
            height + 1);

        if (newRoot->right != nullptr) {
            newRoot->right->parent = newRoot;
        }

        return newRoot;

    }



    /** Helper method to recursively build the subtree of KD tree. */
    KDNode* buildSubtree(vector<Point>& points, unsigned int start,
        unsigned int end, unsigned int d, unsigned int height) {
        if (start >= end)
        {
            return nullptr;
        }
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

        CompareValueAt comparator = CompareValueAt(d);
        // initial sorting for all the points
        sort(points.begin() + start, points.begin() + end, 
            comparator);
        int median = (end + start) / 2;

        KDNode* newRoot = new KDNode(points.at(median));
        isize++;
        // recursively call, start is inclusive, end is exclusive
        newRoot->left = buildSubtree(points, start, median, d + 1, 
                               height + 1);
        if (newRoot->left != nullptr) {
            newRoot->left->parent = newRoot;
        }

        newRoot->right = buildSubtree(points, median + 1, end, d + 1, 
                                     height + 1);

        if (newRoot->right != nullptr) {
            newRoot->right->parent = newRoot;
        }

        return newRoot;
    }


    void findKNNHelperAll(KDNode * node, const Point & queryPoint, unsigned int d) {
        // leaf node situation
        if (node->left == nullptr && node->right == nullptr) {
            // checking square distance

            node->point.setSquareDistToQuery(queryPoint);
            updateKNN(node->point);
            return;
        }

        if (node->left) {
            // recursively go left
            findKNNHelper(node->left, queryPoint, incrementD(d));
        }

        if (node->right) {
            // recursively go right
            findKNNHelper(node->right, queryPoint, incrementD(d));
        }

        node->point.setSquareDistToQuery(queryPoint);
        updateKNN(node->point);
     
    }


    void findKNNHelper(KDNode * node, const Point & queryPoint, unsigned int d) {
        if (node == nullptr) {
            // no neighbor!! 
            // sad lonely guy
            return;
        }
        // leaf node situation
        if (node->left == nullptr && node->right == nullptr) {
            // checking square distance
            if(KNeighbors.size() < k)
            {
                node->point.setSquareDistToQuery(queryPoint);
                updateKNN(node->point);
            } else
            {
                node->point.setSquareDistToQuery(queryPoint);
                if (node->point.squareDistToQuery < threshold) {
                    // square distance is less than threshold
                    // update threshold + update this node into queue
                    threshold = node->point.squareDistToQuery;
                    updateKNN(node->point);
                } 
            }
            
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

            if (node->right && pow((node->point.features[d] -
                queryPoint.features[d]), 2) < threshold)
            {
                findKNNHelper(node->right, queryPoint, incrementD(d));
            }

        }
        else {
            // go right
            if (node->right) {
                // recursively go right
                findKNNHelper(node->right, queryPoint, incrementD(d));
            }

            if (node->left && pow((node->point.features[d] -
                queryPoint.features[d]), 2) < threshold)
            {
                findKNNHelper(node->left, queryPoint, incrementD(d));
            }
        }

        if (KNeighbors.size() < k)
        {
            node->point.setSquareDistToQuery(queryPoint);
            updateKNN(node->point);
        }
        else
        {
            node->point.setSquareDistToQuery(queryPoint);
            if (node->point.squareDistToQuery < threshold) {
                // square distance is less than threshold
                // update threshold + update this node into queue
                threshold = node->point.squareDistToQuery;
                updateKNN(node->point);
            }
        }

    }

    unsigned int incrementD(unsigned int d) {
        return (d + 1) % numDim;
    }


    /** Helper method to update your data structure storing KNN using
     *  the given point.
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


        //KNeighbors.push(point);
        //if (KNeighbors.size() == k + 1)
        //{
        //    Point tmp = KNeighbors.top();
        //    KNeighbors.pop();
        //}
    }

    /** Helper method
     * Inorder traverse BST, print out the data of each node in ascending order
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