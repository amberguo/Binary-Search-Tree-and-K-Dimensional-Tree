#ifndef KDT_HPP
#define KDT_HPP

#include <algorithm> // sort, max, min
#include <math.h>    // pow, abs
#include <vector>    // vector<typename>
#include <iostream>
#include "Point.hpp"


using namespace std;

/** A KD tree that can output K nearest neighbors of given query point */
class KDT {
    
protected:
    
    /** Inner class that defines a KDNode with certain data point 
     *  and pointers to its children and parent
     */
    class KDNode {
        
    public:
        
        KDNode * left;
        KDNode * right;
        KDNode * parent;
        Point point;
        
        KDNode() {}
        
        KDNode(Point point) : point(point) {}
    };
    
    
    KDNode * root; // root of KD tree
    unsigned int numDim; // number of dimension of data points
    unsigned int k; // number of nearest neighbors to find
    double threshold; // largest distance to query point in current KNN
    unsigned int isize;
    unsigned int iheight;
    
    //TODO: define a data structure to store current K nearest neighbors
    
public:
    
    /** TODO: Default constructor of KD tree */
    KDT()
    {
        root = nullptr;
        isize = 0;
        iheight = 0;
    }
    
    /** Build the KD tree from the given vector of Point references */
    void build(vector<Point>& points) {
        root = buildSubtree(points, 0, points.size(), 0, 0);
        // decrement the height to exclude the null node at the end of paths
        iheight--;
    }
    
    /** Find k nearest neighbors of the given query point */
    vector<Point> findKNearestNeighbors(Point queryPoint, unsigned int k) {
        // TODO
        return {};
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
    KDNode * buildSubtree(vector<Point>& points, unsigned int start, 
                    unsigned int end, unsigned int d, unsigned int height) {
        if (points.empty())
        {
            // null check
            return nullptr;
        }
        if( height > iheight)
        {
            // update max height
            iheight = height;
        }
        if(d >= numDim)
        {
            // toggle the dimension
            d = 0;
        }
        if(start == end)
        {   
            // base case for recursion
            return nullptr;
        }
        CompareValueAt comparator = CompareValueAt(d);
        // initial sorting for all the points
        sort(points.begin()+start, points.begin()+end, comparator);
        int median = (end + start) / 2;
        KDNode* newRoot = new KDNode(points.at(median));
        // recursively call, start is inclusive, end is exclusive
        newRoot->left = buildSubtree(points, 0, median, d+1, height);
        newRoot->right = buildSubtree(points, median+1, end, d+1, height);

        return newRoot;
    }
    
    /** Helper method to recursively find the K nearest neighbors */
    void findKNNHelper(KDNode *node, const Point & queryPoint, unsigned int d){
        // TODO
    }
    
    /** Helper method to update your data structure storing KNN using 
     *  the given point.
     */
    void updateKNN(Point & point) {
        // TODO
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
            for( int i = 0; i < (root->point.features.size()); i++)
            {
                cout << root->point.features.at(i) << endl;
            }
            
            inorderRec(root->right);
        }
    }
    
};


#endif // KDT_HPP
