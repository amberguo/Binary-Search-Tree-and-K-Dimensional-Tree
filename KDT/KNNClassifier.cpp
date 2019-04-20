#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "KDT.hpp"
#include "Point.hpp"
#include "BruteForceKNN.hpp"
#include <unordered_map>

using namespace std;

/** Read the data from file and convert them to vector of points.
 *  Each line of the file contains data in each dimension of the
 *  features (and potentially label) of one data point.
 *  The boolean withLabel indicates if the last value in a line
 *  is a label of the data point.
 */
vector<Point> readData(const char * fileName, bool withLabel) {
    
    vector<Point> result;
    string line;
    int numDim = 0;
    double data = 0;
    
    ifstream in;
    in.open(fileName, ios::binary);
    
    // count number of dimensions
    getline(in, line);
    stringstream lineStream(line);
    while (lineStream >> data) numDim++;
    if (withLabel) numDim--;
    
    //Resets the stream to beginning of file
    in.seekg(0, ios_base::beg);
    
    // read the data and convert them to data points
    while (!in.eof()) {
        vector<double> features;
        int label = 0;
        for (int i = 0; i < numDim; i++) {
            in >> data;
            if (!in.fail()) {
                features.push_back(data);
            }
        }
        if (withLabel) {
            in >> label;
        }
        if (!in.fail()) {
            result.push_back(Point(features, label));
        }
    }
    
    in.close();
    return result;
}


/** Find the most frequent label in the given set of points */
int mostFreqLabel(vector<Point>& points) {
    //int maxCount = 0;
    //int mostFrequentLabel = -1;
    //unordered_map<int, int> hash;
    //for (unsigned int i = 0; i < points.size(); i++) {
    //    hash[points.at(i).label]++;
    //    if (hash[points.at(i).label] > maxCount) {
    //        maxCount = hash[points.at(i).label];
    //        mostFrequentLabel = points.at(i).label;
    //    }
    //}
    //// check for same count/frequency
    //for (unsigned int i = 0; i < points.size(); i++) {
    //    // count is same 
    //    if(hash[points.at(i).label] == maxCount)
    //    {
    //        // label is smaller, update
    //        if(points.at(i).label < mostFrequentLabel)
    //        {
    //            mostFrequentLabel = points.at(i).label;
    //        }
    //    }
    //}
    //return mostFrequentLabel;
    if (points.at(0) < points.at(1))
    {
        return points.at(0).label;
    }

    return points.at(1).label;
}


/** Check if a given data file is valid. Note that his does not 
  * check if the data file is in correct format
  */
bool fileValid(const char * fileName) {
    
    ifstream in;
    in.open(fileName, ios::binary);
    
    // Check if input file was actually opened
    if(!in.is_open()) {
        cout<< "Invalid input file. No file was opened. Please try again.\n";
        in.close();
        return false;
    }
    
    // Check for empty file
    in.seekg(0, ios_base::end);
    int len = (int) in.tellg();
    if(len == 0) {
        cout << "The file is empty. \n";
        in.close();
        return false;
    }
    
    in.close();
    return true;
}

int main(int argc, const char * argv[]) {
    vector<double> haha;
    haha.push_back(0);

    Point a = Point(haha, 5);
    Point b = Point(haha, 5);
    Point c = Point(haha, 4);
    Point d = Point(haha, 4);
    Point e = Point(haha, 0);
    Point f = Point(haha, 0);
    vector<Point> test;
    test.push_back(a);
    test.push_back(b);
    test.push_back(c);
    test.push_back(d);
    test.push_back(e);
    test.push_back(f);
    cout << mostFreqLabel(test) << endl;
    // check if second arg is an integer
    char *endptr;
    int k = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0') {
        std::cout << "Invalid number: " << argv[1] << "\n" << endl;
        return 1;
    }
    // check the training file
    if(!fileValid(argv[2]))
    {
        return 1;
    }
    // check the input file
    if (!fileValid(argv[3]))
    {
        return 1;
    }
    // check the test/validation arg
    string val = ("validation");
    string te = ("test");


    if (val != argv[4] && te != argv[4])
    {   
        // invalid fourth arg
        cout << "Fourth argument should be either validation or test" << endl;
        return 1;
    }

    if(val == argv[4])
    {
        // validation mode
        // training the classifier using training data
        vector<Point> training = readData(argv[2], true);
        KDT tree = KDT();
        tree.build(training);

        double mismatch = 0;
        vector<Point> input = readData(argv[3], true);
        for (unsigned int i = 0; i< input.size(); i++)
        {
            vector<Point> neighbors = tree.findKNearestNeighbors(input.at(i), k);
            int prediction = mostFreqLabel(neighbors);
            if( prediction != input.at(i).label)
            {
                mismatch++;
            }
        }
        // TODO double or float
        cout << "mismatch is " << mismatch << endl;
        double error = mismatch / input.size();

        std::ofstream file;
        file.open("result.txt", std::ios_base::app);
        file << "K: " << k << ", Validation Error: " << error << endl;
        file.close();
        return 0;
    } else
    {
        // test
        // Each line of the input data file should contain only the features of 
        // this data, no label following.
        vector<Point> input = readData(argv[3], true);
        vector<Point> neighbors;
        KDT tree = KDT();
        tree.build(input);

        for (unsigned int i = 0; i < input.size(); i++)
        {
            neighbors = tree.findKNearestNeighbors(input.at(i), k);
            int label = mostFreqLabel(neighbors);
            std::ofstream file;
            file.open("result.txt", std::ios_base::app);
            file << label << endl;
            file.close();
        }
    }
    return 0;
}


