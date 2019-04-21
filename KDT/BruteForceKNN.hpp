
#ifndef BruteForceKNN_hpp
#define BruteForceKNN_hpp

#include <algorithm> // sort, max, min
#include <math.h>    // pow, abs
#include <vector>    // vector<typename>
#include <limits>    // numeric_limits<type>::max()
#include "Point.hpp"

/** A brute force approach to find K nearest neighbors */
class BruteForceKNN {
    
protected:
    
    vector<Point> trainingPoints;
    unsigned int k;
    double threshold;
    
    struct distanceComp
    {
        bool operator()(const Point& lhs, const Point& rhs) const
        {
            return lhs.squareDistToQuery < rhs.squareDistToQuery;
        }
    };
    std::priority_queue<Point, std::vector<Point>, distanceComp > KNeighbors;
    
public:
    
    /** Default constructor */
    BruteForceKNN() {}
    
    /** Initialize the training data points */
    void build(vector<Point>& points) {

        trainingPoints = points;

    }
    
    /** Find k nearest neighbors of the given query point */
    vector<Point> findKNearestNeighbors(Point & queryPoint, unsigned int k) {
        this->k = k;
        for (unsigned int i = 0; i< trainingPoints.size(); i++)
        {
            trainingPoints.at(i).setSquareDistToQuery(queryPoint);
            updateKNN(trainingPoints.at(i));
        }
        vector<Point> res;
        while (!KNeighbors.empty()) {
            res.push_back(std::move(const_cast<Point&>(KNeighbors.top())));
            KNeighbors.pop();
        }
        return res;
    }
    
    /** Helper method to update your data structure storing KNN using
     * the given point. */
    void updateKNN(Point & point) {
        if (KNeighbors.size() < this->k)
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
    
};

#endif /* BruteForceKNN_hpp */
