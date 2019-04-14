/**
 * Name: Yingshan Guo
 * Email: yig152@ucsd.edu
 * Reference: Piazza
 * Date: 4/13/2019
 */
#include <iostream>
#include "BSA.hpp"
#include "BST.hpp"
#include "Timer.hpp"

using namespace std;

/** Returns a vector of random double values with range [min, max]
 *  Note that rand() returns bad random numbers, but for simplicity,
 *  we use it to serve our purpose here.
 */
vector<double> randNums(unsigned int size, int min, int max) {
    vector<double> result;
    for (unsigned int i = 0; i < size; i++) {
        result.push_back((max - min) * ( (double)rand() /
                                        (double)RAND_MAX ) + min);
    }
    return result;
}

int main () {
    const int MIN = 0; // min value of random data
    const int MAX = 100; // max value of random data
    const int NUM_INSERT = 3000; // number of random data to insert
    const int NUM_FIND = 3000; // number of random data to find
    const int NUM_RUN = 10;
    
    vector<double> insertData = randNums(NUM_INSERT, MIN, MAX);
    vector<double> findData = randNums(NUM_FIND, MIN, MAX);
    
    BSA<double> bsa;
    BST<double> bst;
    
    cout << "Insert benchmarking for BSA and BST: " << endl;
    cout << "Number of data to insert: " << NUM_INSERT << endl;
    
    Timer t;
    long long sumTime = 0;
    long long averageTime = 0;

    // run NUM_RUN times for inserting in BSA
    for(int i =0; i< NUM_RUN; i++)
    {
        t.begin_timer();
        // create new object each time
        bsa = BSA<double>();
        //cout << bsa.v.size() << endl;
        for( int j = 0; j< NUM_INSERT; j++)
        {
            //bsa.insert(insertData.at(j));
            bsa.insert((double)j);
        }
       // cout << bsa.v.size() << endl;
        sumTime = sumTime + t.end_timer();
    }
    // calculate avg time 
    averageTime = sumTime / NUM_RUN;
    
    cout << "Average time taken to insert for BSA: " << averageTime
    << " milliseconds" << endl;
    
    sumTime = 0;
    averageTime = 0;
    // run NUM_RUN times for inserting in BST
    for (int i = 0; i < NUM_RUN; i++)
    {
        t.begin_timer();
        // create new object each time
        bst = BST<double>();
        for (int j = 0; j < NUM_INSERT; j++)
        {
            //bst.insert(insertData.at(j));
            bst.insert((double)j);
        }
        sumTime = sumTime + t.end_timer();
    }
    // calculate avg time 
    averageTime = sumTime / NUM_RUN;

    //cout << "bst count " <<bst.size() << endl;


    cout << "Average time taken to insert for BST: " << averageTime
    << " milliseconds" << endl;
    
    cout << endl; // new line
    
    cout << "Find benchmarking for BSA and BST: " << endl;
    cout << "Number of data in BSA/BST: " << NUM_INSERT << endl;
    cout << "Number of data to find: " << NUM_FIND << endl;
    
    sumTime = 0;
    averageTime = 0;
    vector<double>::const_iterator a;
    vector<double> v1;

    // run NUM_RUN times for finding in BSA
    for (int i = 0; i < NUM_RUN; i++)
    {
        t.begin_timer();
        for (int j = 0; j < NUM_FIND; j++)
        {
            //a = bsa.find(findData.at(j));
            a = bsa.find(double(j));
            //if(a != bsa.v.cend())
            //{
            //    v1.push_back(*(a._Ptr));
            //}
            
        }
        sumTime = sumTime + t.end_timer();
    }
    // calculate avg time 
    averageTime = sumTime / NUM_RUN;
    
    cout << "Average time taken to find for BSA: " << averageTime
    << " milliseconds" << endl;

    BSTIterator<double> b(nullptr);
    vector<double> v2;
    sumTime = 0;
    averageTime = 0;
    // run NUM_RUN times for finding in BST
    for (int i = 0; i < NUM_RUN; i++)
    {
        t.begin_timer();
        for (int j = 0; j < NUM_FIND; j++)
        {
            //b = bst.find(findData.at(j));
            b = bst.find(double(j));
            //if(b != nullptr)
            //{
            //    v2.push_back(*b);
            //}
            
        }
        sumTime = sumTime + t.end_timer();
    }
    // calculate avg time 
    averageTime = sumTime / NUM_RUN;

    //if( v1.size() != v2.size())
    //{
    //    cout << "size not equal" << endl;
    //}
    //for(int i =0; i< v1.size(); i++)
    //{
    //    if (v1.at(i) != v2.at(i))
    //    {
    //        cout << "value not equal" << endl;
    //        break;
    //    }
    //}
    
    cout << "Average time taken to find for BST: " << averageTime
    << " milliseconds" << endl;
    
    return 0;
}
